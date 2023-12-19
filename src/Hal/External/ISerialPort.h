#pragma once

namespace Hal {

class ISerialPort {
public:
  virtual void Write(const char* msg) = 0;
  virtual ~ISerialPort() {}
};

}
