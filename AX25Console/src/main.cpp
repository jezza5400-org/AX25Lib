#include <chrono>
#include <stdio.h>

#include "AX25Config.hpp"
#include "AX25FrameBuilder.hpp"
#include "AX25Decoder.hpp"

int main()
{
    AX25Config config;
    config.callsignFrom = "VK3ABC";
    config.ssidFrom = 0;
    
    config.callsignTo = "VK3XYZ";
    config.ssidTo = 0;
    
    AX25FrameBuilder builder(config);
    AX25Decoder decoder;
    
    // Example payload (Hello World)
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    auto ax25Frame = builder.buildAx25Frame(payload);
    
    // Print the AX.25 frame in hex
    printf("AX.25 Frame: ");
    for (uint8_t byte : ax25Frame) {
        printf("%02X ", byte);
    }
    printf("\n");
    printf("-----------\n");
    
    auto decoded = decoder.decodePacket(ax25Frame);
    if (decoded) {
        printf("Decoded Packet:\n");
        printf("To: %s\n", decoded->callsignTo.c_str());
        printf("From: %s\n", decoded->callsignFrom.c_str());
        printf("Text: %s\n", decoded->textData.c_str());
    } else {
        printf("Failed to decode packet.\n");
    }
    
    return 0;
}
