#pragma once
#include "Hal/External/IPin.h"
#include "Hal/External/ISerialPort.h"

namespace Hal {

class IBoard {
public:
  virtual ISerialPort& GetP1SerialPort() = 0;
  virtual ISerialPort& GetDebugSerialPort() = 0;
  virtual IPin& GetP1StreamEnablePin() = 0;

  virtual ~IBoard() {}
};

}
