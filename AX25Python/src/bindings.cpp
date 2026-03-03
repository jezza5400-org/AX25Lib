#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "AX25Decoder.hpp"
#include "AX25FrameBuilder.hpp"
#include "AX25Config.hpp"

namespace py = pybind11;

namespace {
std::vector<uint8_t> bytes_to_vec(const py::bytes& b) {
    std::string s = b; // pybind11 provides conversion to std::string
    return std::vector<uint8_t>(s.begin(), s.end());
}

py::bytes vec_to_bytes(const std::vector<uint8_t>& v) {
    return py::bytes(reinterpret_cast<const char*>(v.data()), v.size());
}
}

PYBIND11_MODULE(AX25Python, m) {
    m.doc() = "AX.25 helpers";

    py::class_<AX25DecodedPacket>(m, "AX25DecodedPacket")
        .def_readonly("callsign_to", &AX25DecodedPacket::callsignTo)
        .def_readonly("callsign_from", &AX25DecodedPacket::callsignFrom)
        .def_readonly("text", &AX25DecodedPacket::textData);

    py::class_<AX25Decoder>(m, "AX25Decoder")
        .def(py::init<>())
        .def("decode_packet", [](AX25Decoder& self, const py::bytes& frame_bytes) {
            auto frame = bytes_to_vec(frame_bytes);
            auto decoded = self.decodePacket(frame);
            if (decoded) return py::cast(*decoded);
            return py::object(py::none());
        }, py::arg("frame"));

    m.def("build_ax25_frame", [](const std::string& to, int ssid_to,
                                   const std::string& from, int ssid_from,
                                   const py::bytes& payload_bytes) {
        AX25Config cfg{to.c_str(), from.c_str(), ssid_to, ssid_from};
        AX25FrameBuilder builder(cfg);
        auto frame = builder.buildAx25Frame(bytes_to_vec(payload_bytes));
        return vec_to_bytes(frame);
    }, py::arg("to"), py::arg("ssid_to"), py::arg("from"), py::arg("ssid_from"), py::arg("payload"));
}
