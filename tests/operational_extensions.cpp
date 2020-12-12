#include <catch2/catch.hpp>
#include <atl/expected.hpp>

TEST_CASE("expected::ok", "[extensions.ok]") {
  SECTION("lvalue expected") {
    const atl::expected<int, int> good(1);
    const atl::expected<int, int> bad(atl::unexpect, -1);
    REQUIRE(good.ok());
    REQUIRE(!bad.ok());
  }
  SECTION("rvalue expected") {
    REQUIRE(atl::expected<int, int>(1).ok());
    REQUIRE(!atl::expected<int, int>(atl::unexpect, -1).ok());
  }
}