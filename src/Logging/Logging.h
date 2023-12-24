#pragma once
#include "Utils/IOutputStream.h"
#include "Utils/NonCopyableAndNonMovable.h"
#include <chrono>
#include <cinttypes>
#include <optional>
#include <array>

#define PRIV_PRINT_TO_LOG(funcName, lineNumber, formatString, ...)             \
  do {                                                                         \
    std::array<char, 2056> buf;                                                \
    const auto& written =                                                      \
        std::snprintf(buf.data(), buf.size(), "%s:%i: " formatString,          \
                      funcName, lineNumber, ##__VA_ARGS__);                    \
    if (written < 0) {                                                         \
      Logging::Priv::loggingStream->Write(                                     \
          "Logging Error: failed to format the string\n");                     \
      break;                                                                   \
    }                                                                          \
    Logging::Priv::loggingStream->Write(                                       \
        std::string_view(buf.data(), written));                                \
  } while (false)

#define DEBUG(formatString, ...)                                               \
  PRIV_PRINT_TO_LOG(__func__, __LINE__, formatString, ##__VA_ARGS__)

#define SCOPED_LOG                                                             \
  Logging::Priv::ScopedLog scopedLog_UniqueName(__func__, __LINE__);

namespace Logging {

namespace Priv {

Utils::IOutputStream* loggingStream = nullptr;

class ScopedLog final : private Utils::NonCopyableAndNonMovable {
public:
  ScopedLog(const char* funcName, int lineNumber)
      : _funcName{funcName}, _lineNumber{lineNumber} {
    PRIV_PRINT_TO_LOG(_funcName, _lineNumber, "Enter\n");
  }

  ~ScopedLog() {
    PRIV_PRINT_TO_LOG(
        _funcName, _lineNumber, "%s (%" PRId64 "ms)\n",
        (isDestructorCalledBecauseOfException() ? "Exception" : "Exit"),
        timePassed().count());
  }

private:
  [[nodiscard]] bool isDestructorCalledBecauseOfException() const {
    // add header for std::uncaught_exceptions
    return std::uncaught_exceptions() > _uncaughtExceptionsWhenCreated;
  }

  [[nodiscard]] std::chrono::milliseconds timePassed() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - _creationTime);
  }

  const std::chrono::steady_clock::time_point _creationTime{
      std::chrono::steady_clock::now()};
  const int _uncaughtExceptionsWhenCreated{std::uncaught_exceptions()};
  const char* const _funcName;
  const int _lineNumber;
};

}

static void SetLoggingStream(Utils::IOutputStream& stream) {
  Priv::loggingStream = &stream;
}

}
