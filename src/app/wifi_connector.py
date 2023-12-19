from network import WLAN, STA_IF
from logging import getLogger
from time import sleep

logger = getLogger(__name__)


def _read_wifi_credentials() -> tuple[str, str]:
    with open("/wifi_credentials.txt", 'r') as file:
        lines = file.readlines()
        return lines[0].strip(), lines[1].strip()


def connect_to_wifi_network():
    ssid, password = _read_wifi_credentials()
    logger.info(f"Connect to WiFi network: ssid={ssid}, password={password}")
    sta_if = WLAN(STA_IF)
    sta_if.active(True)
    sta_if.connect(ssid, password)
    while not sta_if.isconnected():
        logger.info("Waiting for WiFi connection...")
        sleep(2)
    logger.info(f"Connected to WiFi network: {sta_if.ifconfig()}")
