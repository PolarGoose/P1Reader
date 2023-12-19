from hal.hardware_board import HardwareBoard
from hal.system_info import get_total_file_system_size_in_kb, get_free_file_system_size_in_kb, get_free_ram_size_in_kb, get_used_ram_size_in_kb, get_total_ram_size_in_kb
from wifi_connector import connect_to_wifi_network
from dsmr_packet_receiving_task import dsmr_packet_receiving_task
from web_server import start_web_server
import asyncio
import logging

logging.basicConfig(level=logging.DEBUG, force=True)
logger = logging.getLogger()

logger.info(f"""
main.py started
Total file system size = {get_total_file_system_size_in_kb():.1f}KB
Free file system size = {get_free_file_system_size_in_kb():.1f}KB
Total RAM = {get_total_ram_size_in_kb():.1f}KB
Free RAM = {get_free_ram_size_in_kb():.1f}KB
Used RAM = {get_used_ram_size_in_kb():.1f}KB
""")

connect_to_wifi_network()
asyncio.create_task(dsmr_packet_receiving_task(HardwareBoard()))
start_web_server()

asyncio.get_event_loop().run_forever()
