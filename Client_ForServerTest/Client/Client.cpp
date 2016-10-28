#include "Client.h"

// Constructor: Binds the listening thread to the Listen method of this class.
// Threads do not have a default empty constructor, which is why this is necessary.
Client::Client():m_listenThread(&Client::Listen, this){}

Client::~Client(){
    m_socket.unbind();
}

bool Client::Connect(){
    if (m_connected){ return false; }
    m_socket.bind(sf::Socket::AnyPort); // A specific port# would limit the number of connections a single computer can have at the same time.
    std::cout << "Bound client to port: " << m_socket.getLocalPort() << std::endl;

    // To establish a connection the client must first send something to the server.
    sf::Packet p;
    StampPacket(PacketType::Connect, p);
    p << m_playerName;
    // Rest of code is responsible for correctly handling timeouts.
    if (m_socket.send(p, m_serverIp, m_serverPort) != sf::Socket::Done){ m_socket.unbind(); return false; }
    m_socket.setBlocking(false);    // Non-blocking mode because we only use a single thread.
    p.clear();                      // Clear packet in order to use again.

    // To capture IP and port# of response
    sf::IpAddress recvIP;
    PortNumber recvPORT;
    // Clock to determine whether we've been waiting too long for a response.
    sf::Clock timer;
    timer.restart();

    std::cout << "Attempting to connect to: " << m_serverIp << ":" << m_serverPort << std::endl;
    while (timer.getElapsedTime().asMilliseconds() < CONNECT_TIMEOUT){
        sf::Socket::Status s = m_socket.receive(p, recvIP, recvPORT);
        if (s != sf::Socket::Done){ continue; }
        if (recvIP != m_serverIp){ continue; } // if wrong IP --> Skip iteration.
        PacketID id;
        if (!(p >> id)){ break; }                               // Does packet contain an ID? Else break.
        if ((PacketType)id != PacketType::Connect){ continue; } // Does the ID match connect? Else skip.
        m_packetHandler(id, p, this);           // Pass received info to packet handler.
        m_connected = true;
        m_socket.setBlocking(true);             // Socket back to blocking mode.
        m_lastHeartbeat = m_serverTime;
        m_listenThread.launch();
        return true;
    }
    std::cout << "Connection attempt failed! Server info: " << m_serverIp << ":" << m_serverPort << std::endl;
    m_socket.unbind();
    m_socket.setBlocking(true);
    return false;
}

bool Client::Disconnect(){
    if (!m_connected){ return false; }
    sf::Packet p;
    StampPacket(PacketType::Disconnect, p);
    sf::Socket::Status s = m_socket.send(p, m_serverIp, m_serverPort);
    m_connected = false;
    m_socket.unbind(); // Unbind to close the listening thread.
    if (s != sf::Socket::Done){ return false; }
    return true;
}

// Once the client is connected to the server, the listening thread is launched.
// This is what makes it tick.
void Client::Listen(){
    // After setting up some local variables to hold the packet, IP and port information,
    // the listening thread loop is entered. It runs as long as the client is connected to the server.
    sf::Packet packet;
    sf::IpAddress recvIP;
    PortNumber recvPORT;
    std::cout << "Beginning to listen..." << std::endl;
    while (m_connected){
        packet.clear();
        // Since the socket is in blocking mode, the listening thread will halt at the receive method until data comes in.
        sf::Socket::Status status = m_socket.receive(packet, recvIP, recvPORT);
        // If the returned status denotes some kind of failure:
        if (status != sf::Socket::Done){
            if (m_connected){
                std::cout << "Failed receiving a packet from "
                          << recvIP << ":" << recvPORT << ". Status: " << status << std::endl;
                continue;
            } else {
                std::cout << "Socket unbound." << std::endl;
                break;
            }
        }
        if (recvIP != m_serverIp){
            // Ignore packets not sent from the server.
            std::cout << "Invalid packet received: invalid origin." << std::endl;
            continue;
        }
        PacketID p_id;
        if (!(packet >> p_id)){
            // Non-conventional packet.
            std::cout << "Invalid packet received: unable to extract id." << std::endl;
            continue;
        }
        PacketType id = (PacketType)p_id;
        if (id < PacketType::Disconnect || id >= PacketType::OutOfBounds){
            // Invalid packet type.
            std::cout << "Invalid packet received: id is out of bounds." << std::endl;
            continue;
        }

        if (id == PacketType::Heartbeat){ // Received a heartbeat, will attempt a response.
            sf::Packet p;
            StampPacket(PacketType::Heartbeat, p);
            if (m_socket.send(p, m_serverIp, m_serverPort) != sf::Socket::Done){
                std::cout << "Failed sending a heartbeat!" << std::endl;
            }
            sf::Int32 timestamp;
            packet >> timestamp;
            SetTime(sf::milliseconds(timestamp));
            m_lastHeartbeat = m_serverTime;
        }
            // If the packet ID is something else, it gets handled by a different class.
        else if(m_packetHandler){
            m_packetHandler((PacketID)id, packet, this);
        }
    }
    std::cout << "...Listening stopped." << std::endl;
}

bool Client::Send(sf::Packet& l_packet){
    if (!m_connected){ return false; }
    if (m_socket.send(l_packet, m_serverIp, m_serverPort) != sf::Socket::Done){ return false; }
    return true;
}

const sf::Time& Client::GetTime()const{
    return m_serverTime;
}

const sf::Time& Client::GetLastHeartbeat()const{
    return m_lastHeartbeat;
}

void Client::SetTime(const sf::Time& l_time){
    m_serverTime = l_time;
}

void Client::SetServerInformation(const sf::IpAddress& l_ip, const PortNumber& l_port){
    m_serverIp = l_ip;
    m_serverPort = l_port;
}

// Main purpose: Keeping track of passing time by adding the time between updates to the server time.
void Client::Update(const sf::Time& l_time){
    if (!m_connected){ return; }
    m_serverTime += l_time;
    if (m_serverTime.asMilliseconds() < 0){
        // #milliseconds that has passed since the beginning is a 32-bit integer (max value = 2,147,483,647, after which is goes negative.)
        // Only a problem if the server were to run continuously for 25 whole days, but this fix eliminates the problem.
        m_serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
        m_lastHeartbeat = m_serverTime;
        return;
    }
    // Check for connection timeout.
    if (m_serverTime.asMilliseconds() - m_lastHeartbeat.asMilliseconds() >= sf::Int32(Network::ClientTimeout)){
        // Timeout.
        std::cout << "Server connection timed out!" << std::endl;
        Disconnect();
    }
}

bool Client::IsConnected()const{ return m_connected; }
void Client::SetPlayerName(const std::string& l_name){ m_playerName = l_name; }

sf::Mutex& Client::GetMutex(){ return m_mutex; }

// A way to provide a custom packet handler function that is to be used by this class.
//      - The member function version of this method is implemented in the header file.
//          This is the function pointer version.
void Client::Setup(void(*l_handler)(const PacketID&, sf::Packet&, Client*)){
    m_packetHandler = std::bind(l_handler,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

// To undo what Setup() does.
void Client::UnregisterPacketHandler(){
    m_packetHandler = nullptr;
}