// NetworkDefinitions.h
// -- Contains the most common bits of information shared between both client and server.
//      For example: All the types and constants what both communicating sides are going to be using.

#ifndef SERVER_ADDINGGROUPS_NETWORKDEFINITIONS_H
#define SERVER_ADDINGGROUPS_NETWORKDEFINITIONS_H



enum class Network{
    HighestTimestamp = 2147483647,
    ClientTimeout = 10000,
    ServerPort = 5600,
    NullID = -1
};
using ClientID = int;
using GroupID = int;
using PortNumber = unsigned short;



#endif //SERVER_ADDINGGROUPS_NETWORKDEFINITIONS_H
