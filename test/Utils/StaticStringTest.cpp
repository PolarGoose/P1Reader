#include "Utils/StaticString.h"
#include "doctest.h"

TEST_CASE("StaticString tests") {
  SUBCASE("Construction with an empty string") {
    Utils::StaticString s("");
    REQUIRE(s.str() == "");
  }

  SUBCASE("Construction with a short string") {
    std::string input = "Hello";
    Utils::StaticString s(input);
    REQUIRE(s.str() == input);
  }

  SUBCASE("Construct with a string exactly 300 characters long") {
    std::string input(299, 'x');
    Utils::StaticString s(input);
    REQUIRE(s.str() == input);
  }

  SUBCASE("Construct with a string longer than 300 characters") {
    std::string input(350, 'y');
    Utils::StaticString s(input);
    REQUIRE(s.str() == std::string_view(input.data(), 299));
  }
}
