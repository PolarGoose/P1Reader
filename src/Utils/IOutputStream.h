#pragma once
#include <string_view>

namespace Utils {

class IOutputStream {
public:
  virtual void Write(std::string_view msg) = 0;
  virtual ~IOutputStream() {}
};

}
