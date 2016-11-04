#include <iostream>
#include <SFML/Network.hpp>
#include "Server.h"

// Packet handler:
// We'll provide a pointer to this function to our Server class --> The fingerprint/signature must match exactly.
void Handler(sf::IpAddress& l_ip, const PortNumber& l_port, const PacketID& l_id, sf::Packet& l_packet, Server* l_server){
    std::cout << "Handling packet from: " << l_ip << ":" << l_port << " PacketID: " << (int)l_id << std::endl;
    ClientID clientID = l_server->GetClientID(l_ip, l_port);
    // If the client ID for the specified IP and port# exists:
    if (clientID >= 0){
        // Only interested in two types of packets: Disconnect and Message.
        if ((PacketType)l_id == PacketType::Disconnect){
            l_server->RemoveClient(l_ip, l_port);
            sf::Packet p;
            StampPacket(PacketType::Message, p);
            std::string message;
            message = "Client left! " + l_ip.toString() + ":" + std::to_string(l_port);
            p << message;
            l_server->Broadcast(p, clientID);
        } else if ((PacketType)l_id == PacketType::Message){
            std::string receivedMessage;
            l_packet >> receivedMessage;


            std::string message = l_ip.toString() + ":" + std::to_string(l_port) + " :" + receivedMessage;
            sf::Packet p;
            StampPacket(PacketType::Message, p);
            p << message;
//            l_server->Broadcast(p, clientID); // excludes sender
            l_server->Broadcast(p); // Does not exclude sender
        }
        else if ((PacketType)l_id == PacketType::JoinGroup){
            sf::Int32 sfInt32;
            l_packet >> sfInt32;
            int groupID = int(sfInt32);

            if (groupID == -1) { // -1 is not an ID, used for creating a group.
                groupID = l_server->CreateGroup();
                l_server->AddToGroup(groupID, clientID);
            } else {
                l_server->AddToGroup(groupID, clientID);
            }
        }
    }
        // If the client was not found we are only interested in Connect packets.
    else {
        if ((PacketType)l_id == PacketType::Connect){
            std::cout << "Received Connect packet from a new client: " << l_ip<<":"<<l_port << std::endl;
            ClientID id = l_server->AddClient(l_ip, l_port);
            sf::Packet packet;
            StampPacket(PacketType::Connect, packet);
            l_server->Send(id, packet);
        }
    }
}

// Runs in a separate thread and processes user input.
void CommandProcess(Server* l_server){
    while (l_server->IsRunning()){
        std::string str;
        std::getline(std::cin, str);
        if (str == "!quit"){
            l_server->Stop();
            break;
        } else if (str == "dc"){
            l_server->DisconnectAll();
            std::cout << "DC..." << std::endl;
        } else if (str == "list"){
            std::cout << l_server->GetClientCount() << " clients online:" << std::endl;
            std::cout << l_server->GetClientList() << std::endl;
        } else if (str == "printGroups"){ // ***Testing***
            l_server->PrintGroups();
        }
    }
}

int main(){
    // Create an instance of the server class,
    // passing it a function pointer that will handling packets.
    Server server(Handler);

    if (server.Start()){
        // Set up and launch command thread.
        sf::Thread c(&CommandProcess, &server);
        c.launch();

        sf::Clock clock;
        clock.restart();
        // Main loop
        while (server.IsRunning()){
            server.Update(clock.restart());
        }
        std::cout << "Stopping server..." << std::endl;
    }

    system("PAUSE");
    return 0;
}