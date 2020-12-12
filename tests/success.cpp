#include <atl/expected.hpp>
#include <catch2/catch.hpp>

TEST_CASE("success", "[extensions.success]") {
  SECTION("void-expected") {
    auto f = []() -> atl::expected<void, int> { return atl::success; };
    REQUIRE(f());
  }

  SECTION("int-expected") {
    auto f = []() -> atl::expected<int, int> { return atl::success(123); };
    REQUIRE(f().value() == 123);
  }

  SECTION("string-expected") {
    auto f = []() -> atl::expected<std::string, int> {
      return atl::success("foo");
    };
    REQUIRE(f().value() == "foo");
  }
}
