#pragma once
#include <array>
#include <cstring>
#include <string_view>
#include <Utils/NonCopyableAndNonMovable.h>

namespace Utils {

class StaticString : NonCopyableAndNonMovable {
private:
  const size_t _strLength;
  const std::array<char, 300> _buf; // Contains null terminated string

  std::array<char, 300> CreateBuffer(std::string_view str, const size_t length) {
    std::array<char, 300> buf;
    std::memcpy(buf.data(), str.data(), length);
    buf[length] = '0';
    return buf;
  }

public:
  StaticString(std::string_view str)
      : _strLength{std::min(str.size(), _buf.size() - 1)},
        _buf{CreateBuffer(str, _strLength)} {
  }

  std::string_view str() { return {_buf.data(), _strLength}; }
};

}
