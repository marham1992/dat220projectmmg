#include <iostream>
#include <SFML/Network.hpp>
#include "Client.h"

// Packet handler
void HandlePacket(const PacketID& l_id, sf::Packet& l_packet, Client* l_client){
    // The client responds to two types of packets: Messages and Disconnect.
    if ((PacketType)l_id == PacketType::Message){
        // Extract message and print to console.
        std::string message;
        l_packet >> message;
        std::cout << message << std::endl;
    } else if ((PacketType)l_id == PacketType::Disconnect){
        l_client->Disconnect();
    }
}

// Runs in a command thread.
// Since std::getline is a blocking function,
// a user will have to press enter once if the client gets disconnected by the server,
// just to provide some input and get things moving again in order for the thread to be closed.
void CommandProcess(Client* l_client){
    while (l_client->IsConnected()){
        std::string str;
        std::getline(std::cin, str);
        if (str != ""){
            if (str == "!quit"){
                l_client->Disconnect();
                break;
            }
//            sf::Packet p;
//            StampPacket(PacketType::Message, p);
//            p << str;

            sf::Int32 myInt = std::stoi(str);
            sf::Packet p;
            StampPacket(PacketType::JoinGroup, p);
            p << myInt;


            l_client->Send(p);
        }
    }
}

// Main loop of chat client.
int main(int argc, char** argv){
    sf::IpAddress ip;   // Server IP
    PortNumber port;    // Server Port#
//    sf::IpAddress ip("192.168.11.129");   // Server IP
//    PortNumber port(5600);    // Server Port#


    // Adding support for command line arguments.
    // If none are provided, the user is simply prompted to enter the server information inside the console window.
    // Otherwise, the command-line arguments are read in and used for the same purpose.
    if (argc == 1){
        std::cout << "Enter Server IP: ";
        std::cin >> ip;
        std::cout << "Enter Server Port: ";
        std::cin >> port;
    } else if (argc == 3){
        ip = argv[1];
        port = atoi(argv[2]);
    } else {
        return 0;
    }

    // Create and setup a client instance with the provided server info.
    Client client;
    client.SetServerInformation(ip, port);
    client.Setup(&HandlePacket);            // Register packet handler.
    sf::Thread c(&CommandProcess, &client); // Prepare command thread.

    if (client.Connect()){  // Try to connect
        c.launch();         // Launch command thread.
        sf::Clock clock;
        clock.restart();
        while (client.IsConnected()){ // Main loop, where the client gets updated.
            client.Update(clock.restart());
        }
    } else {
        std::cout << "Failed to connect." << std::endl;
    }
    std::cout << "Quitting..." << std::endl;
    sf::sleep(sf::seconds(1.f));
}