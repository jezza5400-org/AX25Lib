#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "protocall/AX25Decoder.hpp"
#include "protocall/AX25FrameBuilder.hpp"
#include "protocall/AX25Config.hpp"
#include "Direwolf.h"

namespace py = pybind11;

namespace
{
    std::vector<uint8_t> bytes_to_vec(const py::bytes& b)
    {
        std::string s = b; // pybind11 provides conversion to std::string
        return std::vector<uint8_t>(s.begin(), s.end());
    }

    py::bytes vec_to_bytes(const std::vector<uint8_t>& v)
    {
        return py::bytes(reinterpret_cast<const char*>(v.data()), v.size());
    }
}

PYBIND11_MODULE(AX25Python, m)
{
    m.doc() = "AX.25 helpers";

    //Config Struct
    py::class_<AX25Config>(m, "AX25Config")
        .def(py::init<>())
        .def(py::init([](const std::string& callsign_to, int ssid_to, const std::string& callsign_from, int ssid_from)
        {
            AX25Config cfg;
            cfg.callsignTo = callsign_to;
            cfg.callsignFrom = callsign_from;
            cfg.ssidTo = ssid_to;
            cfg.ssidFrom = ssid_from;
            return cfg;
        }), py::arg("callsign_to") = "", py::arg("callsign_from") = "", py::arg("ssid_to") = 0,
            py::arg("ssid_from") = 0)
        .def_readwrite("callsign_to", &AX25Config::callsignTo)
        .def_readwrite("callsign_from", &AX25Config::callsignFrom)
        .def_readwrite("ssid_to", &AX25Config::ssidTo)
        .def_readwrite("ssid_from", &AX25Config::ssidFrom);

    py::class_<AX25DecodedPacket>(m, "AX25DecodedPacket")
        .def_readonly("callsign_to", &AX25DecodedPacket::callsignTo)
        .def_readonly("callsign_from", &AX25DecodedPacket::callsignFrom)
        .def_readonly("text", &AX25DecodedPacket::textData);

    py::class_<AX25Decoder>(m, "AX25Decoder")
        .def(py::init<>())
        .def("decode_packet", [](AX25Decoder& self, const py::bytes& frame_bytes)
        {
            auto frame = bytes_to_vec(frame_bytes);
            auto decoded = self.decodePacket(frame);
            if (decoded) return py::cast(*decoded);
            return py::object(py::none());
        }, py::arg("frame"));

    py::class_<AX25FrameBuilder>(m, "AX25FrameBuilder")
        .def(py::init<const AX25Config&>())
        .def("buildAx25Frame", [](AX25FrameBuilder& self, const py::bytes& payload_bytes)
        {
            auto payload = bytes_to_vec(payload_bytes);
            auto frame = self.buildAx25Frame(payload);
            return vec_to_bytes(frame);
        }, py::arg("payload"))
        .def("buildKissFrame", [](AX25FrameBuilder& self, const py::bytes& payload_bytes)
        {
            auto payload = bytes_to_vec(payload_bytes);
            auto frame = self.buildKissFrame(payload);
            return vec_to_bytes(frame);
        }, py::arg("payload"));

    py::class_<DirewolfConnectionInfo>(m, "DirewolfConnectionInfo")
        .def(py::init<>())
        .def(py::init<const std::string&, int>(), py::arg("host") = "127.0.0.1", py::arg("port") = 8001)
        .def_readwrite("host", &DirewolfConnectionInfo::host)
        .def_readwrite("port", &DirewolfConnectionInfo::port);

    py::class_<DirewolfPacketInfo>(m, "DirewolfPacketInfo")
        .def(py::init<>())
        .def(py::init([](const std::string& to_callsign, int to_ssid, const std::string& from_callsign, int from_ssid)
        {
            DirewolfPacketInfo info;
            info.toCallsign = to_callsign;
            info.toSSID = to_ssid;
            info.fromCallsign = from_callsign;
            info.fromSSID = from_ssid;
            return info;
        }),
            py::arg("to_callsign") = "NOCALL",
            py::arg("to_ssid") = 0,
            py::arg("from_callsign") = "NOCALL",
            py::arg("from_ssid") = 0)
        .def_readwrite("to_callsign", &DirewolfPacketInfo::toCallsign)
        .def_readwrite("to_ssid", &DirewolfPacketInfo::toSSID)
        .def_readwrite("from_callsign", &DirewolfPacketInfo::fromCallsign)
        .def_readwrite("from_ssid", &DirewolfPacketInfo::fromSSID);

    py::class_<DirewolfConfig>(m, "DirewolfConfig")
        .def(py::init<>())
        .def(py::init([](const DirewolfConnectionInfo& connection_info, const DirewolfPacketInfo& packet_info)
        {
            DirewolfConfig config;
            config.connectionInfo = connection_info;
            config.packetInfo = packet_info;
            return config;
        }), py::arg("connection_info"), py::arg("packet_info"))
        .def_readwrite("connection_info", &DirewolfConfig::connectionInfo)
        .def_readwrite("packet_info", &DirewolfConfig::packetInfo);

    py::class_<Direwolf>(m, "Direwolf")
        .def(py::init<>())
        .def(py::init<DirewolfConfig>(), py::arg("config"))
        .def("listen", &Direwolf::listen)
        .def("send_packet", [](Direwolf& self, int packet_type, const py::bytes& payload_bytes)
        {
            self.sendPacket(packet_type, bytes_to_vec(payload_bytes));
        }, py::arg("packet_type"), py::arg("payload"))
        .def("on_packet_received", [](Direwolf& self, int packet_type, py::function callback)
        {
            self.onPacketReceived<std::vector<uint8_t>>(packet_type, [callback = std::move(callback)](std::vector<uint8_t> payload)
            {
                py::gil_scoped_acquire acquire;
                callback(vec_to_bytes(payload));
            });
        }, py::arg("packet_type"), py::arg("callback"));
}
