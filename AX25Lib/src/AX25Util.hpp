#pragma once
#include <string>
#include <vector>

class AX25Util
{
public:
    static std::vector<uint8_t> kissEscape(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> kissUnescape(const std::vector<unsigned char>& in);
};
