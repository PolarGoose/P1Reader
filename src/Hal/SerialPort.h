#pragma once
#include "Hal/External/ISerialPort.h"
#include "Utils/NonCopyableAndNonMovable.h"
#include <HardwareSerial.h>

namespace Hal {

class SerialPort : public ISerialPort, private Utils::NonCopyableAndNonMovable {
private:
  HardwareSerial& _port;

public:
  SerialPort(HardwareSerial& port) : _port(port) {}
  void Write(const char* msg) override { _port.write(msg); }
  HardwareSerial& Raw() { return _port; }
};

}
