// Client.h -- Networking code for the client side i placed in the Client class.

#ifndef CLIENT_WITHCHATBOX_CLIENT_H
#define CLIENT_WITHCHATBOX_CLIENT_H


#include <SFML/Network.hpp>
#include "../Shared/PacketTypes.h"
#include "../Shared/NetworkDefinitions.h"
#include <functional>
#include <iostream>

// The time it takes for a client to realize it's no longer connected to a server.
#define CONNECT_TIMEOUT 5000 // ms = 5 sec

class Client;
// A function type used to handle packets on the client side.
using PacketHandler = std::function<void(const PacketID&, sf::Packet&, Client*)>;

class Client{
public:
    Client();
    ~Client();

    bool Connect();
    bool Disconnect();

    void Listen();
    bool Send(sf::Packet& l_packet);

    const sf::Time& GetTime() const;
    const sf::Time& GetLastHeartbeat() const;
    void SetTime(const sf::Time& l_time);
    void SetServerInformation(const sf::IpAddress& l_ip, const PortNumber& l_port);

    // We want to support both regular functions and member functions to serve as packet handlers,
    // hence two Setup methods.
    template<class T>
    void Setup(void(T::*l_handler)(const PacketID&, sf::Packet&, Client*), T* l_instance){
        m_packetHandler = std::bind(l_handler, l_instance,
                                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }

    void Setup(void(*l_handler)(const PacketID&, sf::Packet&, Client*));
    void UnregisterPacketHandler();

    void Update(const sf::Time& l_time);

    bool IsConnected() const;
    void SetPlayerName(const std::string& l_name);

    sf::Mutex& GetMutex();
private:
    std::string m_playerName;

    sf::UdpSocket m_socket;
    sf::IpAddress m_serverIp;
    PortNumber m_serverPort;
    PacketHandler m_packetHandler;
    bool m_connected;
    sf::Time m_serverTime;
    sf::Time m_lastHeartbeat; // Holds the latest timestamp received from the server and can be checked for timeouts.

    // The class keeps and manages its own instances of sf::Mutex and sf::Thread.
    // This way, we can provide a common interface of thread synchronization to the outside code.
    sf::Thread m_listenThread;
    sf::Mutex m_mutex;
};


#endif //CLIENT_WITHCHATBOX_CLIENT_H
