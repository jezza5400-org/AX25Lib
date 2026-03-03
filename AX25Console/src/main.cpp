#include <chrono>
#include <stdio.h>

#include "AX25Config.hpp"
#include "AX25FrameBuilder.hpp"
#include "AX25Decoder.hpp"

void printHex(const std::vector<uint8_t>& data) {
    for (uint8_t byte : data) {
        printf("%02X ", byte);
    }
    printf("\n");
}

void encodeAndDecodeframe(std::vector<uint8_t>& data, AX25FrameBuilder& builder, AX25Decoder& decoder)
{
    auto ax25Frame = builder.buildAx25Frame(data);
    printHex(ax25Frame);
    
    auto decoded = decoder.decodePacket(ax25Frame);
    if (decoded) {
        printf("Decoded Packet:\n");
        printf("To: %s\n", decoded->callsignTo.c_str());
        printf("From: %s\n", decoded->callsignFrom.c_str());
        printf("Text: %s\n", decoded->textData.c_str());
    } else {
        printf("Failed to decode packet.\n");
    }
}

int main()
{
    AX25Config config;
    config.callsignFrom = "VK3ABC";
    config.ssidFrom = 0;
    
    config.callsignTo = "VK3XYZ";
    config.ssidTo = 0;
    
    AX25FrameBuilder builder(config);
    AX25Decoder decoder;
    
    // Example payload (Hello World
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    encodeAndDecodeframe(payload, builder, decoder);
    
    return 0;
}
