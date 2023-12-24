#include "Logging/Logging.h"
#include "doctest.h"

class MockOutputStream : public Utils::IOutputStream {
public:
  std::vector<doctest::String> _logMessages;

  void Write(std::string_view str) override {
    _logMessages.emplace_back(str.data());
    std::cout << str;
  }
};

TEST_CASE("Logging") {
  MockOutputStream mockOutputStream;
  Logging::SetLoggingStream(mockOutputStream);

  SUBCASE("Debug macros should print log messages") {
    DEBUG("Test printing without arguments\n");
    DEBUG("Test printing with arguments: %s", "arg1\n");
  
    REQUIRE(mockOutputStream._logMessages.size() == 2);
    REQUIRE(mockOutputStream._logMessages[0] ==
            doctest::Contains("Test printing without arguments"));
    REQUIRE(mockOutputStream._logMessages[1] ==
            doctest::Contains("Test printing with arguments: arg1"));
  }
  
  SUBCASE("SCOPED_LOG should print entering and exiting from the function") {
    {
      SCOPED_LOG
    }
    REQUIRE(mockOutputStream._logMessages.size() == 2);
    REQUIRE(mockOutputStream._logMessages[0] ==
            doctest::Contains(": Enter"));
    REQUIRE(mockOutputStream._logMessages[1] ==
            doctest::Contains(": Exit ("));
  }
  
  SUBCASE("SCOPED_LOG should indicate if an exception is thrown from the "
          "function") {
    try {
      SCOPED_LOG
      throw std::exception();
    } catch (const std::exception&) {
      REQUIRE(mockOutputStream._logMessages.size() == 2);
      REQUIRE(mockOutputStream._logMessages[0] ==
              doctest::Contains(": Enter"));
      REQUIRE(mockOutputStream._logMessages[1] ==
              doctest::Contains(": Exception ("));
    }
  }

  SUBCASE("Log message longer than 2056 symbols should be truncated") {
    // Arrange
    std::string veryLongMessage;
    std::generate_n(std::back_inserter(veryLongMessage), 2056,
                    [n = 0]() mutable { return static_cast<char>('a' + n++ % 26); });

    // Act
    DEBUG("%s", veryLongMessage.c_str());

    // Assert
    REQUIRE(mockOutputStream._logMessages.size() == 1);
    REQUIRE(mockOutputStream._logMessages[0].size() == 2055);
  }
}
