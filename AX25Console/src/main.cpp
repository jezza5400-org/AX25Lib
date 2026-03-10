#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <inicpp.h>

#include "Direwolf.h"
#include "Utill.h"

enum PacketTypes
{
    e_MessagePacket
};

struct MessagePacket
{
    const char* name;
    const char* message;
    
};

int main(int argc, char** argv)
{
    DirewolfConfig config = {};
    config.connectionInfo.host = "127.0.0.1";
    config.connectionInfo.port = 8001;
    
    config.packetInfo.fromCallsign = "NOCALL";
    config.packetInfo.fromSSID = 0;
    config.packetInfo.toCallsign = "NOCALL";
    config.packetInfo.toSSID = 0;
    
    Direwolf* direwolf = new Direwolf(config);

    direwolf->onPacketReceived<MessagePacket>(PacketTypes::e_MessagePacket, [] (MessagePacket packet) {
        std::cout << "Received message packet: " << packet.name << " says " << packet.message << "\n";
    });
    
    direwolf->listen(); // Starts listening for packets (blocking call)

    direwolf->sendPacket(PacketTypes::e_MessagePacket, MessagePacket{"Alice", "Hello, Direwolf!"});
    while (true)
    {
    }
    
}
