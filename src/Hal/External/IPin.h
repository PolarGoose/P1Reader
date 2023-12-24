#pragma once
#include <cstdint>

namespace Hal {

class IPin {
public:
  virtual uint8_t Raw() const;

  virtual ~IPin() {}
};

}
