#pragma once

namespace Utils {

class NonCopyableAndNonMovable {
protected:
  NonCopyableAndNonMovable() = default;
  ~NonCopyableAndNonMovable() = default;

public:
  NonCopyableAndNonMovable(NonCopyableAndNonMovable&&) = delete;
  NonCopyableAndNonMovable& operator=(NonCopyableAndNonMovable&&) = delete;

  NonCopyableAndNonMovable(const NonCopyableAndNonMovable&) = delete;
  NonCopyableAndNonMovable& operator=(const NonCopyableAndNonMovable&) = delete;
};

}
