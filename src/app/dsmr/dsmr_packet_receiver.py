from logging import getLogger

logger = getLogger(__name__)

class DsmrPacketReceiver:
    def __init__(self, buffer_size: int):
        self.packet_buf = bytearray()
        self.max_buf_size = buffer_size
        self.state = WaitingForPacketStartSymbol(self)

    def process_byte(self, byte: int) -> bytearray:
        if byte == ord('/'):
            self.packet_buf = bytearray()
            self.packet_buf.append(byte)
            if not isinstance(self.state, WaitingForPacketStartSymbol):
                logger.warning("Packet is lost")
            self.set_state(WaitingForPacketEndSymbol(self))
            return

        if len(self.packet_buf) > self.max_buf_size:
            self.packet_buf = bytearray()
            self.set_state(WaitingForPacketStartSymbol(self))
            return

        return self.state.process_byte(byte)

    def set_state(self, state) -> None:
        self.state = state


class WaitingForPacketStartSymbol:
    def __init__(self, state_ctx: DsmrPacketReceiver):
        self.state_ctx = state_ctx

    def process_byte(self, character: str) -> str:
        pass


class WaitingForPacketEndSymbol:
    def __init__(self, state_ctx: DsmrPacketReceiver):
        self.state_ctx = state_ctx

    def process_byte(self, byte: int) -> bytearray:
        self.state_ctx.packet_buf.append(byte)
        if byte == ord('!'):
            self.state_ctx.set_state(WaitingForCrc(self.state_ctx));


class WaitingForCrc:
    def __init__(self, state_ctx: DsmrPacketReceiver):
        self.state_ctx = state_ctx
        self.crc = 0
        self.amount_of_crc_bytes_received = 0

    def process_byte(self, byte: int) -> bytearray:
        if not self._is_crc_byte(byte):
            self.state_ctx.set_state(WaitingForPacketStartSymbol(self))
            return

        self._add_to_crc(byte)

        if self.amount_of_crc_bytes_received != 4:
            return

        self.state_ctx.set_state(WaitingForPacketStartSymbol(self))

        if self.crc == self._calculate_crc16(self.state_ctx.packet_buf):
            return self.state_ctx.packet_buf
        else:
            logger.warning("Packet CRC is invalid")
            return

    @staticmethod
    def _is_crc_byte(byte: int) -> bool:
        return (ord('0') <= byte <= ord('9')) or (ord('A') <= byte <= ord('F'))

    def _add_to_crc(self, byte: int) -> None:
        if ord('0') <= byte <= ord('9'):
            byte = byte - ord('0')
        elif ord('A') <= byte <= ord('F'):
            byte = byte - ord('A') + 10

        self.crc = (self.crc << 4) | (byte & 0xF)
        self.amount_of_crc_bytes_received += 1

    @staticmethod
    def _calculate_crc16(packet_buf: bytearray) -> int:
        crc = 0
        data = packet_buf

        for byte in data:
            crc ^= byte
            for _ in range(8):
                if crc & 1:
                    crc = (crc >> 1) ^ 0xa001
                else:
                    crc = crc >> 1

        return crc
