// Server.h -- The server application.

#ifndef SERVER_ADDINGGROUPS_SERVER_H
#define SERVER_ADDINGGROUPS_SERVER_H

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <unordered_map>
#include <functional>
#include <iostream>

#include "../Shared/NetworkDefinitions.h"
#include "../Shared/PacketTypes.h"

// Server will send hearbeats every second and retry five times before timing out the client.
#define HEARTBEAT_INTERVAL 1000 // ms.
#define HEARTBEAT_RETRIES 5


struct ClientInfo{
    sf::IpAddress m_clientIP;
    PortNumber m_clientPORT;
    sf::Time m_lastHeartbeat;
    sf::Time m_heartbeatSent;
    bool m_heartbeatWaiting; // Is the server waiting for a heartbeat response?
    unsigned short m_heartbeatRetry;
    unsigned int m_ping;    // Keeping track of latency.
    GroupID m_groupID;

    ClientInfo(const sf::IpAddress& l_ip, const PortNumber& l_port, const sf::Time& l_heartbeat)
            : m_clientIP(l_ip), m_clientPORT(l_port),
              m_lastHeartbeat(l_heartbeat), // Store timestamp, used to determine when to dispatch next heartbeat.
              m_heartbeatWaiting(false),
              m_heartbeatRetry(0),
              m_ping(0),
              m_groupID(-1)
    {}

    // set equal to l_rhs (right-hand-side)
    ClientInfo& operator=(const ClientInfo& l_rhs){
        m_clientIP			= l_rhs.m_clientIP;
        m_clientPORT		= l_rhs.m_clientPORT;
        m_lastHeartbeat		= l_rhs.m_lastHeartbeat;
        m_heartbeatSent		= l_rhs.m_heartbeatSent;
        m_heartbeatWaiting	= l_rhs.m_heartbeatWaiting;
        m_heartbeatRetry	= l_rhs.m_heartbeatRetry;
        m_ping				= l_rhs.m_ping;
        return *this;
    }
};

using Clients = std::unordered_map<ClientID, ClientInfo>;
using ClientsOfGrp = std::unordered_map<ClientID, ClientInfo*>;


struct GroupInfo{
    ClientsOfGrp m_clientsOfGrp;

    GroupInfo(){};
};

using Groups = std::unordered_map<GroupID, GroupInfo>;


class Server;
// Custom function for handling incoming packets.
using PacketHandler = std::function<void(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*)>;
// To process client timeouts outside of this class.
using TimeoutHandler = std::function<void(const ClientID&)>;

class Server{
public:
    // Two constructors: For supporting both member functions and regular functions being used as packet handlers.
    template <class T>
    Server(void(T::*l_handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*),
           T* l_instance): m_listenThread(&Server::Listen, this)
    {
        m_packetHandler = std::bind(l_handler, l_instance,
                                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                    std::placeholders::_4, std::placeholders::_5);
    }

    Server(void(*l_handler)(sf::IpAddress&, const PortNumber&, const PacketID&, sf::Packet&, Server*));
    ~Server();

    // Support both member functions and regular functions being used as timeout handlers.
    template<class T>
    void BindTimeoutHandler(void(T::*l_handler)(const ClientID&), T* l_instance){
        m_timeoutHandler = std::bind(l_handler, l_instance, std::placeholders::_1);
    }
    void BindTimeoutHandler(void(*l_handler)(const ClientID&));

    bool Send(const ClientID& l_id, sf::Packet& l_packet);
    bool Send(sf::IpAddress& l_ip, const PortNumber& l_port, sf::Packet& l_packet);
    void Broadcast(sf::Packet& l_packet, const ClientID& l_ignore = ClientID(Network::NullID));

    void Listen();
    void Update(const sf::Time& l_time);

    ClientID AddClient(const sf::IpAddress& l_ip, const PortNumber& l_port);
    ClientID GetClientID(const sf::IpAddress& l_ip, const PortNumber& l_port);
    bool HasClient(const ClientID& l_id);
    bool HasClient(const sf::IpAddress& l_ip, const PortNumber& l_port);
    bool GetClientInfo(const ClientID& l_id, ClientInfo& l_info);
    bool RemoveClient(const ClientID& l_id);
    bool RemoveClient(const sf::IpAddress& l_ip, const PortNumber& l_port);

    void DisconnectAll();
    bool Start();
    bool Stop();

    bool IsRunning();

    unsigned int GetClientCount();
    std::string GetClientList();

    sf::Mutex& GetMutex();

    GroupID CreateGroup();
    bool AddToGroup(const GroupID& l_groupID, const ClientID& l_clientID);
    bool RemoveFromGroup(const ClientID& l_clientID);

    // ***Testing*** Prints all members all groups.
    void PrintGroups();
private:
    void Setup();
    ClientID m_lastID;
    GroupID m_nextGrpID;

    // Two sockets: one for sending, one for receiving.
    sf::UdpSocket m_incoming;
    sf::UdpSocket m_outgoing;

    PacketHandler m_packetHandler;
    TimeoutHandler m_timeoutHandler;

    Clients m_clients;
    Groups m_groups;
    sf::Time m_serverTime;

    bool m_running;

    sf::Thread m_listenThread;
    sf::Mutex m_mutex;

    // Keeping track of sent/received data.
    size_t m_totalSent;
    size_t m_totalReceived;
};



#endif //SERVER_ADDINGGROUPS_SERVER_H
