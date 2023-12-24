#include <HardwareSerial.h>

class SerialPort : public Utils::IOutputStream, private Utils::NonCopyableAndNonMovable {
private:
  HardwareSerial& _port;

public:
  SerialPort(HardwareSerial& port) : _port(port) {}

  void Write(std::string_view msg) override {
    _port.write(msg.data());
  }

  HardwareSerial& Raw() {
    return _port;
  }
};
