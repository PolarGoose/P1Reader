from machine import UART, Pin


class HardwareBoard:
    def __init__(self):
        self.p1_port_uart = UART(2, 115200, tx=17, rx=16, rxbuf=10 * 1000)
        self.p1_port_enable_transfer_gpio = Pin(12, mode=Pin.OUT, value=0)

    def get_p1_port(self) -> UART:
        return self.p1_port_uart

    def get_p1_port_enable_transfer_pin(self) -> Pin:
        return self.p1_port_enable_transfer_gpio
