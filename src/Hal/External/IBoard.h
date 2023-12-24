#pragma once
#include "Utils/IOutputStream.h"

namespace Hal {

class IBoard {
public:
  virtual Utils::IOutputStream& GetP1SerialPort() = 0;
  virtual Utils::IOutputStream& GetDebugSerialPort() = 0;
  virtual int GetP1StreamEnablePinNumber() = 0;

  virtual ~IBoard() {}
};

}
