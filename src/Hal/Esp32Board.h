#pragma once

#include "Hal/IBoard.h"
#include "HardwareSerial.h"
#include "Utils/NonCopyableAndNonMovable.h"

namespace Hal {

class Esp32Board : public IBoard, private Utils::NonCopyableAndNonMovable {
public:
  Esp32Board() {
    // Configure P1SerialPort: UART2
    Serial2.setRxBufferSize(8192);
    Serial2.setTxBufferSize(8192);
    Serial2.begin(115200, SERIAL_8N1,
                  /* Uart RX pin */ 16,
                  /* Uart TX pin */ 17);

    // Configure DebugSerialPort: UART0
    Serial.setRxBufferSize(8192);
    Serial.setTxBufferSize(8192);
    Serial.begin(921600, SERIAL_8N1,
                 /* Uart RX pin */ 3,
                 /* Uart TX pin */ 1);
  }

  Stream& GetP1SerialPort() override {
    return Serial2;
  }

  Stream& GetDebugSerialPort() override {
    return Serial;
  }

  int GetP1StreamEnablePinNumber() override {
    return 12;
  }
};

}
