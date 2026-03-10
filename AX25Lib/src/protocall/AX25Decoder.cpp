#include "AX25Decoder.hpp"
#include <stdexcept>

#include "AX25Util.hpp"

AX25Decoder::AX25Decoder() {}

std::optional<AX25DecodedPacket> AX25Decoder::decodePacket(const std::vector<uint8_t>& ax25Frame)
{
    try {
        if (ax25Frame.empty()) return std::nullopt;

        // If FEND (0xC0) present, pick first non-empty chunk between FEND bytes
        std::vector<unsigned char> frame;
        bool has_fend = false;
        for (unsigned char b : ax25Frame) if (b == 0xC0) { has_fend = true; break; }
        if (has_fend) {
            std::size_t i = 0;
            while (i < ax25Frame.size()) {
                // skip leading FENDs
                while (i < ax25Frame.size() && ax25Frame[i] == 0xC0) ++i;
                std::size_t start = i;
                while (i < ax25Frame.size() && ax25Frame[i] != 0xC0) ++i;
                if (i > start) { frame.assign(ax25Frame.begin() + start, ax25Frame.begin() + i); break; }
            }
            if (frame.empty()) frame.clear();
        } else {
            frame = ax25Frame;
        }

        // Remove optional KISS command byte (0x00)
        if (!frame.empty() && frame[0] == 0x00) frame.erase(frame.begin());

        // Unescape KISS
        frame = AX25Util::kissUnescape(frame);

        // Minimal AX.25 length: 2 * 7 addresses + CONTROL + PID = 16
        if (frame.size() < 16) return std::nullopt;

        AddressParseResult af;
        try {
            af = m_parseAddress(frame);
        } catch (...) {
            return std::nullopt;
        }
        if (af.addresses.size() < 2) return std::nullopt;

        auto dest_raw = af.addresses[0];
        auto src_raw  = af.addresses[1];

        if (frame.size() < af.consumed + 2) return std::nullopt;
        std::vector<unsigned char> payload(frame.begin() + af.consumed + 2, frame.end());

        AX25DecodedPacket res;
        res.callsignTo = m_decodeCall(dest_raw);
        res.callsignFrom  = m_decodeCall(src_raw);
        res.textData = std::string(payload.begin(), payload.end()); // preserve bytes; UTF-8 invalid sequences kept as-is
        return res;
    } catch (...) {
        return std::nullopt;
    }
}

std::string AX25Decoder::m_decodeCall(const std::array<uint8_t, 7>& addr)
{
    std::string call;
    call.reserve(8);
    for (int i = 0; i < 6; ++i) {
        char c = static_cast<char>(addr[i] >> 1);
        if (c != ' ') call.push_back(c);
    }
    unsigned char ssid = (addr[6] >> 1) & 0x0F;
    if (ssid) {
        call.push_back('-');
        call += std::to_string(ssid);
    }
    return call;
}

AX25Decoder::AddressParseResult AX25Decoder::m_parseAddress(const std::vector<uint8_t>& ax25Frame)
{
    AddressParseResult result;
    result.consumed = 0;
    while (true) {
        if (result.consumed + 7 > ax25Frame.size()) {
            throw std::runtime_error("truncated AX.25 address field");
        }
        std::array<uint8_t, 7> addr;
        for (std::size_t i = 0; i < 7; ++i) addr[i] = ax25Frame[result.consumed + i];
        result.addresses.push_back(addr);
        result.consumed += 7;
        if (addr[6] & 0x01) break;
    }
    return result;
}


