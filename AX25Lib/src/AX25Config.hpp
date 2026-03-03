#pragma once

struct AX25Config
{
    const char* callsignTo; // To send to
    const char* callsignFrom; // From us
    int ssidTo; // SSID for destination
    int ssidFrom; // SSID for source
};
