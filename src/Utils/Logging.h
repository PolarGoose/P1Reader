#pragma once

#include "Utils/NonCopyableAndNonMovable.h"
#include <Stream.h>
#include <optional>
#include <chrono>
#include <cinttypes>

#define PRIV_UTILS_NUMBER_TO_STR_HELPER(x) #x
#define PRIV_UTILS_NUMBER_TO_STR(x) PRIV_UTILS_NUMBER_TO_STR_HELPER(x)
#define PRIV_UTILS_LINE_STR PRIV_UTILS_NUMBER_TO_STR(__LINE__)
#define PRIV_UTILS_LINE_INFO __FILE__ ":" PRIV_UTILS_LINE_STR ": "

#define DEBUG_F(...) Utils::Logging::Priv::loggingStream->printf(PRIV_UTILS_LINE_INFO __VA_ARGS__);
#define DEBUG(arg) Utils::Logging::Priv::loggingStream->println(arg);
#define SCOPED_LOG Utils::Logging::Priv::ScopedLog scopedLog_UniqueName(PRIV_UTILS_LINE_INFO);

namespace Utils::Logging {

namespace Priv {
Print* loggingStream = nullptr;

class ScopedLog final : private Utils::NonCopyableAndNonMovable {
public:
  ScopedLog(const char* const _lineInfo) : _lineInfo{_lineInfo} {
    loggingStream->printf("%s Enter", _lineInfo);
  }

  ~ScopedLog() {
    loggingStream->printf(
        "%s %s (%" PRId64 "ms)", _lineInfo,
        (isDestructorCalledBecauseOfException() ? "Exception" : "Exit"),
        timePassed().count());
  }

private:
  [[nodiscard]] bool isDestructorCalledBecauseOfException() const {
    return std::uncaught_exceptions() > _uncaughtExceptionsWhenCreated;
  }

  [[nodiscard]] std::chrono::milliseconds timePassed() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - _creationTime);
  }

  const std::chrono::steady_clock::time_point _creationTime{
      std::chrono::steady_clock::now()};
  const int _uncaughtExceptionsWhenCreated{std::uncaught_exceptions()};
  const char* const _lineInfo;
};
}

static void SetLoggingStream(Print& stream) { Priv::loggingStream = &stream; }

}
