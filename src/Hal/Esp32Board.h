#pragma once

#include "Hal/External/IBoard.h"
#include "Hal/Pin.h"
#include "Hal/SerialPort.h"
#include "Utils/NonCopyableAndNonMovable.h"
#include <HardwareSerial.h>

namespace Hal {

class Esp32Board : public IBoard, private Utils::NonCopyableAndNonMovable {
private:
  SerialPort _p1Port;
  SerialPort _debugPort;
  Pin _p1StreamEnablePin{12};

public:
  Esp32Board() : _p1Port(/* UART2 */ Serial2), _debugPort(/* UART0 */ Serial1) {
    _p1Port.Raw().setRxBufferSize(8192);
    _p1Port.Raw().setTxBufferSize(8192);
    _p1Port.Raw().begin(115200, SERIAL_8N1,
                        /* Uart RX pin */ 16,
                        /* Uart TX pin */ 17);

    _debugPort.Raw().setRxBufferSize(8192);
    _debugPort.Raw().setTxBufferSize(8192);
    _debugPort.Raw().begin(921600, SERIAL_8N1,
                           /* Uart RX pin */ 3,
                           /* Uart TX pin */ 1);
  }

  SerialPort& GetP1SerialPort() override { return _p1Port; }

  SerialPort& GetDebugSerialPort() override { return _debugPort; }

  IPin& GetP1StreamEnablePin() override { return _p1StreamEnablePin; }
};

}
