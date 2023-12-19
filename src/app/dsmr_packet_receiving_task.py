from hal.hardware_board import HardwareBoard
from dsmr.dsmr_packet_receiver import DsmrPacketReceiver
from dsmr.dsmr_packet_parser import parse_dsmr_packet, DsmrDataObject
from logging import getLogger
import asyncio

logger = getLogger(__name__)
last_parsed_packet_data = None


def get_last_parsed_packet_data() -> DsmrDataObject:
    return last_parsed_packet_data

async def dsmr_packet_receiving_task(hardware_board: HardwareBoard):
    logger.info("task started")
    p1_port_reader = asyncio.StreamReader(hardware_board.get_p1_port())
    packet_receiver = DsmrPacketReceiver(4000)

    # Enable DSMR transmission
    hardware_board.get_p1_port_enable_transfer_pin().value(1)

    while True:
        try:
            bytes_from_uart = await p1_port_reader.read(2000)
            for byte in bytes_from_uart:
                packet = packet_receiver.process_byte(byte)
                if packet is not None:
                    global last_parsed_packet_data
                    last_parsed_packet_data = parse_dsmr_packet(packet.decode())
        except Exception as e:
            logger.exception(f"thread exception", exc_info=e)
