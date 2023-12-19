#pragma once
#include "Hal/External/IPin.h"
#include "Utils/NonCopyableAndNonMovable.h"
#include <HardwareSerial.h>

namespace Hal {

class Pin : public IPin, private Utils::NonCopyableAndNonMovable {
private:
  uint8_t _rawPinNumber;

public:
  Pin(uint8_t rawPinNumber) : _rawPinNumber(rawPinNumber) {}
  virtual uint8_t Raw() const override { return _rawPinNumber; }
};

}
