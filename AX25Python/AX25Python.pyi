from typing import Callable, Optional

class AX25Config:
    callsign_to: str
    callsign_from: str
    ssid_to: int
    ssid_from: int
    def __init__(self, callsign_to: str = "", ssid_to: int = 0,
                 callsign_from: str = "", ssid_from: int = 0) -> None: ...

class AX25DecodedPacket:
    callsign_to: str
    callsign_from: str
    text: str

class AX25Decoder:
    def __init__(self) -> None: ...
    def decode_packet(self, frame: bytes) -> Optional[AX25DecodedPacket]: ...

class AX25FrameBuilder:
    def __init__(self, cfg: AX25Config) -> None: ...
    def buildAx25Frame(self, payload: bytes) -> bytes: ...
    def buildKissFrame(self, payload: bytes) -> bytes: ...

class DirewolfConnectionInfo:
    host: str
    port: int
    def __init__(self, host: str = "127.0.0.1", port: int = 8001) -> None: ...

class DirewolfPacketInfo:
    to_callsign: str
    to_ssid: int
    from_callsign: str
    from_ssid: int
    def __init__(self, to_callsign: str = "NOCALL", to_ssid: int = 0,
                 from_callsign: str = "NOCALL", from_ssid: int = 0) -> None: ...

class DirewolfConfig:
    connection_info: DirewolfConnectionInfo
    packet_info: DirewolfPacketInfo
    def __init__(self, connection_info: DirewolfConnectionInfo = ..., packet_info: DirewolfPacketInfo = ...) -> None: ...

class Direwolf:
    def __init__(self, config: DirewolfConfig = ...) -> None: ...
    def listen(self) -> None: ...
    def send_packet(self, packet_type: int, payload: bytes) -> None: ...
    def on_packet_received(self, packet_type: int, callback: Callable[[bytes], object]) -> None: ...
