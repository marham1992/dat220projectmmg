#include "Server.h"

// Constructor: Binds the listening thread to the Listen method of this class.
// Threads do not have a default empty constructor, which is why this is necessary.
Server::Server(void(*l_handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*))
        : m_listenThread(&Server::Listen, this), m_running(false)
{
    // Bind a packet handler function.
    m_packetHandler = std::bind(l_handler,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                std::placeholders::_4, std::placeholders::_5);
}

Server::~Server(){
    Stop();
}

// If a client times out, simply disconnecting the client is not always enough.
// Sometimes you may want to e.g. de-spawn entities in the game world, this feature will help with such things.
void Server::BindTimeoutHandler(void(*l_handler)(const ClientID&)){
    m_timeoutHandler = std::bind(l_handler, std::placeholders::_1);
}

/// Will write to console every time a packet is sent(Must be modified for and added to; both Send() methods and the Broadcast() to account for all packets):
//std::cout << "Sent a packet to: "<< itr->second.m_clientIP<<":"<<itr->second.m_clientPORT<<"  With ClientID: "<< itr->first << std::endl;

bool Server::Send(const ClientID& l_id, sf::Packet& l_packet){
    sf::Lock lock(m_mutex);
    auto itr = m_clients.find(l_id);
    if (itr == m_clients.end()){ return false; }
    /// Three statements for sending packets. (1):Public IP + m_outgoing. (2):Local IP + m_outgoing. (3):Public IP + m_incoming.
//    if (m_outgoing.send(l_packet, itr->second.m_clientIP, itr->second.m_clientPORT) != sf::Socket::Done) /// (1)
//    if (m_outgoing.send(l_packet, itr->second.m_clientIP.getLocalAddress(), itr->second.m_clientPORT) != sf::Socket::Done) /// (2)
    if (m_incoming.send(l_packet, itr->second.m_clientIP, itr->second.m_clientPORT) != sf::Socket::Done) /// (3)
    {
        std::cout << "Error sending a packet..." << std::endl;
        return false;
    }
    m_totalSent += l_packet.getDataSize();
    return true;
}

// Overloaded version of the Send method that does not require a client (just IP and port#)
bool Server::Send(sf::IpAddress& l_ip, const PortNumber& l_port, sf::Packet& l_packet){
    /// Three statements for sending packets. (1):Public IP + m_outgoing. (2):Local IP + m_outgoing. (3):Public IP + m_incoming.
//    if (m_outgoing.send(l_packet, l_ip, l_port) != sf::Socket::Done){ return false; } /// (1)
//    if (m_outgoing.send(l_packet, l_ip.getLocalAddress(), l_port) != sf::Socket::Done){ return false; } /// (2)
    if (m_incoming.send(l_packet, l_ip, l_port) != sf::Socket::Done){ return false; } /// (3)
    m_totalSent += l_packet.getDataSize();
    return true;
}

// Sending to all clients currently connected. (E.g. Chat messages, entity states etc.)
// The client with ID l_ignore does not receive the broadcast.
void Server::Broadcast(sf::Packet& l_packet, const ClientID& l_ignore){
    sf::Lock lock(m_mutex); // Ensures the client is not removed from the container right in the middle of a send operation.
    for (auto &itr : m_clients)
    {
        if (itr.first != l_ignore){
            /// Three statements for sending packets. (1):Public IP + m_outgoing. (2):Local IP + m_outgoing. (3):Public IP + m_incoming.
//            if (m_outgoing.send(l_packet, itr.second.m_clientIP, itr.second.m_clientPORT) /// (1)
//            if (m_outgoing.send(l_packet, itr.second.m_clientIP.getLocalAddress(), itr.second.m_clientPORT) /// (2)
            if (m_incoming.send(l_packet, itr.second.m_clientIP, itr.second.m_clientPORT) /// (3)
                != sf::Socket::Done)
            {
                std::cout << "Error broadcasting a packet to client: "
                          << itr.first << std::endl;
                continue;
            }
            m_totalSent += l_packet.getDataSize();
        }
    }
}

// Separate thread for processing incoming data.
void Server::Listen(){
    sf::IpAddress ip;
    PortNumber port;
    sf::Packet packet;
    std::cout << "Beginning to listen..." << std::endl;
    while (m_running){
        packet.clear();
        sf::Socket::Status status = m_incoming.receive(packet, ip, port); // .receive function blocks.
        if (status != sf::Socket::Done){
            if (m_running){
                std::cout << "Error receiving a packet from: "
                          << ip << ":" << port << ". Code: " << status << std::endl;
                continue;
            } else {
                std::cout << "Socket unbound." << std::endl;
                break;
            }
        }

        m_totalReceived += packet.getDataSize();

        PacketID p_id;
        if (!(packet >> p_id)){
            std::cout << "Invalid packet received: unable to extract id." << std::endl;
            continue;
        } // Non-conventional packet.
        PacketType id = (PacketType)p_id;
        if (id < PacketType::Disconnect || id >= PacketType::OutOfBounds){
            std::cout << "Invalid packet received: id is out of bounds." << std::endl;
            continue;
        } // Invalid packet type.

        if (id == PacketType::Heartbeat){
            bool ClientFound = false;
            sf::Lock lock(m_mutex); // Lock mutex because we're starting to iterate over a list of data.
            for (auto &itr : m_clients){
                if (itr.second.m_clientIP != ip || itr.second.m_clientPORT != port){ continue; }
                ClientFound = true;
                if (!itr.second.m_heartbeatWaiting){
                    std::cout << "Invalid heartbeat packet received!" << std::endl;
                    break;
                }
                itr.second.m_ping = m_serverTime.asMilliseconds() - itr.second.m_heartbeatSent.asMilliseconds();
                itr.second.m_lastHeartbeat = m_serverTime;
                itr.second.m_heartbeatWaiting = false;
                itr.second.m_heartbeatRetry = 0;
                break;
            }
            if (!ClientFound){
                std::cout << "Heartbeat from unknown client received..." << std::endl;
            }
        } else if (m_packetHandler){
            m_packetHandler(ip, port, (PacketID)id, packet, this); // Handle the packet.
        }
    }
    std::cout << "...Listening stopped." << std::endl;
}

void Server::Update(const sf::Time& l_time){
    m_serverTime += l_time;
    if (m_serverTime.asMilliseconds() < 0){ // If the int32 grows too large it goes negative (if server is up 24+ days)
        m_serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
        sf::Lock lock(m_mutex);
        for (auto &itr : m_clients){
            itr.second.m_lastHeartbeat = sf::milliseconds(std::abs(itr.second.m_lastHeartbeat.asMilliseconds() -
                                                                   sf::Int32(Network::HighestTimestamp)));
        }
    }

    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin(); itr != m_clients.end();){
        sf::Int32 elapsed = m_serverTime.asMilliseconds() - itr->second.m_lastHeartbeat.asMilliseconds();
        if (elapsed >= HEARTBEAT_INTERVAL){
            if (elapsed >= sf::Int32(Network::ClientTimeout) || itr->second.m_heartbeatRetry > HEARTBEAT_RETRIES){
                // Remove client.
                std::cout << "Client " << itr->first << " has timed out." << std::endl;
                if (m_timeoutHandler){ m_timeoutHandler(itr->first); }
                itr = m_clients.erase(itr);
                continue;
            }
            if (!itr->second.m_heartbeatWaiting || (elapsed >= HEARTBEAT_INTERVAL * (itr->second.m_heartbeatRetry + 1))){
                // Heartbeat
                if (itr->second.m_heartbeatRetry >= 3){
                    std::cout << "Re-try(" << itr->second.m_heartbeatRetry
                              << ") heartbeat for client " << itr->first << std::endl;
                }
                sf::Packet Heartbeat;
                StampPacket(PacketType::Heartbeat, Heartbeat);
                Heartbeat << m_serverTime.asMilliseconds();
                Send(itr->first, Heartbeat);
                if (itr->second.m_heartbeatRetry == 0){
                    itr->second.m_heartbeatSent = m_serverTime;
                }
                itr->second.m_heartbeatWaiting = true;
                ++itr->second.m_heartbeatRetry;

                m_totalSent += Heartbeat.getDataSize();
            }
        }
        ++itr;
    }
}

ClientID Server::AddClient(const sf::IpAddress& l_ip, const PortNumber& l_port){
    sf::Lock lock(m_mutex); // Since we're modifying client data.
    for (auto &itr : m_clients){
        if (itr.second.m_clientIP == l_ip && itr.second.m_clientPORT == l_port){
            return ClientID(Network::NullID); // If the specified IP and port# combination already exists.
        }
    }
    ClientID id = m_lastID; // Assign new ID
    ClientInfo info(l_ip, l_port, m_serverTime);
    m_clients.emplace(id, info);
    ++m_lastID; // Increment

    // ID of commonGroup is always 0.
    AddToGroup(0, id);

    return id;
}

// Obtain ID of a client by providing their IP and port#
ClientID Server::GetClientID(const sf::IpAddress& l_ip, const PortNumber& l_port){
    sf::Lock lock(m_mutex); // For safe iteration.
    for (auto &itr : m_clients){
        if (itr.second.m_clientIP == l_ip && itr.second.m_clientPORT == l_port)
            return itr.first;
    }
    return ClientID(Network::NullID);
}

bool Server::HasClient(const ClientID& l_id){
    return (m_clients.find(l_id) != m_clients.end());
}

bool Server::HasClient(const sf::IpAddress& l_ip, const PortNumber& l_port){
    return(GetClientID(l_ip, l_port) >= 0);
}

bool Server::GetClientInfo(const ClientID& l_id, ClientInfo& l_info){
    sf::Lock lock(m_mutex);
    for (auto &itr : m_clients){
        if (itr.first == l_id){
            l_info = itr.second;
            return true;
        }
    }
    return false;
}

// Remove client by ID
bool Server::RemoveClient(const ClientID& l_id){
    sf::Lock lock(m_mutex);
    auto itr = m_clients.find(l_id);
    if (itr == m_clients.end()){ return false; }
    sf::Packet p;
    StampPacket(PacketType::Disconnect, p);
    Send(l_id, p);
    m_clients.erase(itr);
    return true;
}

// Remove client by IP and port#
bool Server::RemoveClient(const sf::IpAddress& l_ip, const PortNumber& l_port){
    sf::Lock lock(m_mutex);
    for (auto itr = m_clients.begin();
         itr != m_clients.end(); ++itr)
    {
        if (itr->second.m_clientIP == l_ip && itr->second.m_clientPORT == l_port){
            sf::Packet p;
            StampPacket(PacketType::Disconnect , p);
            Send(itr->first, p);
            m_clients.erase(itr);
            return true;
        }
    }
    return false;
}

GroupID Server::CreateGroup() {
    std::cout << "CreateGroup() called" << std::endl;
    GroupInfo groupInfo;
    m_groups.emplace(m_nextGrpID, groupInfo);

    for (auto itr = m_groups.begin(); itr != m_groups.end(); ++itr){
        std::cout <<"Group: " << itr->first << std::endl;
    }

    m_nextGrpID++;
    return m_nextGrpID-1;
}

bool Server::AddToGroup(const GroupID& l_groupID, const ClientID& l_clientID){
    if(m_groups.find(l_groupID) == m_groups.end()){
        std::cout << "Group not found, groupID: " << l_groupID << std::endl;
        return false;
    }
    ClientInfo& clientInfo = m_clients.at(l_clientID);
    if (clientInfo.m_groupID != -1){ // If already in a group, remove from current group.
        RemoveFromGroup(l_clientID);
    }
    m_groups.find(l_groupID)->second.m_clientsOfGrp.emplace(l_clientID, &clientInfo);
    clientInfo.m_groupID = l_groupID;
    return true;
}

bool Server::RemoveFromGroup(const ClientID& l_clientID){
    ClientInfo& clientInfo = m_clients.at(l_clientID);
    auto itr = m_groups.find(clientInfo.m_groupID);
    if (itr == m_groups.end()){ return false; } // Did not find group.
    auto itr2 = itr->second.m_clientsOfGrp.find(l_clientID);
    if (itr2 == itr->second.m_clientsOfGrp.end()) {return false;} // Did not find client in group.

    itr->second.m_clientsOfGrp.erase(itr2);
    clientInfo.m_groupID = -1;
    return true;
}

// Remove all clients.
void Server::DisconnectAll(){
    if (!m_running){ return; }
    sf::Packet p;
    StampPacket(PacketType::Disconnect, p);
    Broadcast(p);
    sf::Lock lock(m_mutex);
    m_clients.clear();
}

// Starting the server.
bool Server::Start(){
    if (m_running){ return false; }
    // Bind sockets to ports.
    if (m_incoming.bind((unsigned short)Network::ServerPort) != sf::Socket::Done)
        return false;                       // If binding incoming socket fails.
    m_outgoing.bind(sf::Socket::AnyPort);   // Else bind outgoing
    Setup();                                // Just initializes some data.
    std::cout << "Incoming port: " <<
              m_incoming.getLocalPort() << ". Outgoing port: "
              << m_outgoing.getLocalPort() << std::endl;
    // Launch thread.
    m_listenThread.launch();
    m_running = true;
    return true;
}

// Stopping the server
bool Server::Stop(){
    if (!m_running){ return false; }
    DisconnectAll();     // Drop all clients.
    m_running = false;
    m_incoming.unbind(); // Stops the listening thread.
    return true;
}

bool Server::IsRunning(){ return m_running; }

unsigned int Server::GetClientCount(){ return m_clients.size(); }
std::string Server::GetClientList(){
    std::string list;
    std::string delimiter = "--------------------------------------";
    list = delimiter;
    list += '\n';
    list += "ID";
    list += '\t';
    list += "Client IP:PORT";
    list += '\t'; list += '\t';
    list += "Ping";
    list += '\n';
    list += delimiter;
    list += '\n';
    for (auto itr = m_clients.begin(); itr != m_clients.end(); ++itr){
        list += std::to_string(itr->first);
        list += '\t';
        list += itr->second.m_clientIP.toString() + ":" + std::to_string(itr->second.m_clientPORT);
        list += '\t';
        list += std::to_string(itr->second.m_ping) + "ms.";
        list += '\n';
    }
    list += delimiter;
    list += '\n';
    list += "Total data sent: " + std::to_string(m_totalSent / 1000) + "kB. Total data received: " + std::to_string(m_totalReceived / 1000) + "kB";
    return list;
}

sf::Mutex& Server::GetMutex(){ return m_mutex; }

// Invoked on server start.
void Server::Setup(){
    m_lastID = 0;
    m_running = false;
    m_totalSent = 0;
    m_totalReceived = 0;

    m_nextGrpID = 1;
    GroupInfo groupInfo;
    m_groups.emplace(0, groupInfo);
}


// ***Testing*** Prints all members all groups.
void Server::PrintGroups(){
    for (auto itr = m_groups.begin(); itr != m_groups.end(); ++itr){
        std::cout << "Member of group with ID:" << itr->first << std::endl;
        for (auto itr2 = itr->second.m_clientsOfGrp.begin(); itr2 != itr->second.m_clientsOfGrp.end(); ++itr2){
            std::cout << "  " << "Client ID: " << itr2->first << "  ClientInfo.GroupID: " << itr2->second->m_groupID << std::endl;
        }
    }
}