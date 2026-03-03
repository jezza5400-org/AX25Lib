#include "AX25Util.hpp"

std::vector<uint8_t> AX25Util::kissEscape(const std::vector<uint8_t>& data)
{
    std::vector<uint8_t> out;
    out.reserve(data.size() * 2); // worst-case
    for (uint8_t b : data) {
        if (b == 0xDB) { out.push_back(0xDB); out.push_back(0xDD); }
        else if (b == 0xC0) { out.push_back(0xDB); out.push_back(0xDC); }
        else { out.push_back(b); }
    }
    return out;
}

std::vector<unsigned char> AX25Util::kissUnescape(const std::vector<unsigned char>& in) {
    std::vector<unsigned char> out;
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        unsigned char b = in[i];
        if (b == 0xDB && i + 1 < in.size()) {
            unsigned char n = in[++i];
            if (n == 0xDC) out.push_back(0xC0);
            else if (n == 0xDD) out.push_back(0xDB);
            else out.push_back(n);
        } else {
            out.push_back(b);
        }
    }
    return out;
}