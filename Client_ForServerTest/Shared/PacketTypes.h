// PacketTypes.h -- Defines packet types that are going to be exchanged between the server and client.
//               -- Defines the type of the packet identifier: sf::Int8

#ifndef CLIENT_FORSERVERTEST_PACKETTYPES_H
#define CLIENT_FORSERVERTEST_PACKETTYPES_H



#include <SFML/Network/Packet.hpp>
using PacketID = sf::Int8;
enum class PacketType{
    Disconnect = -1,
    Connect,
    JoinGroup,
// A mechanism checking if a client or the server has stopped responding, and therefore is deemed to have timed out, is called a 'Heartbeat mechanism'
            Heartbeat,
    Snapshot,
    Player_Update,
    Message,
    Hurt,
    OutOfBounds // Not an actual packet type. Exists for convenience when checking whether a packet type is valid or not.
};

// Attaches a type to a packet.
void StampPacket(const PacketType& l_type, sf::Packet& l_packet);


#endif //CLIENT_FORSERVERTEST_PACKETTYPES_H
