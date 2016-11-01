// PacketTypes.cpp -- Implements a function for attaching a type to a packet.

#include "PacketTypes.h"

// Attaches a type to a packet.
void StampPacket(const PacketType& l_type, sf::Packet& l_packet){
    // convert type into sf::Int8, then feed into packet instance.
    l_packet << PacketID(l_type);
}