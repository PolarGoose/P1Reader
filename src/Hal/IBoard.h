#pragma once

#include <Stream.h>

namespace Hal {

class IBoard {
public:
  virtual Stream& GetP1SerialPort() = 0;
  virtual Stream& GetDebugSerialPort() = 0;
  virtual int GetP1StreamEnablePinNumber() = 0;

  virtual ~IBoard() {}
};

}
