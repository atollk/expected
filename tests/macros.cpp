#include <catch2/catch.hpp>
#include <atl/expected.hpp>

TEST_CASE("ATL_TRY", "[extensions.macros]") {
  SECTION("void-expected") {
    const auto good = []() { return atl::expected<void, int>(); };
    const auto bad = []() { return atl::expected<void, int>(atl::unexpect, 0); };
    const auto propagate_good = [good]() -> atl::expected<void, int> {
      ATL_TRY(good());
      return atl::expected<void, int>(atl::unexpect, 1);
    };
    const auto propagate_bad = [bad]() -> atl::expected<void, int> {
      ATL_TRY(bad());
      return atl::expected<void, int>();
    };
    REQUIRE(propagate_good().error() == 1);
    REQUIRE(propagate_bad().error() == 0);
  }
  SECTION("string-expected") {
    const auto good = []() { return atl::expected<std::string, int>("foo"); };
    const auto bad = []() { return atl::expected<std::string, int>(atl::unexpect, 0); };
    const auto propagate_good = [good]() -> atl::expected<std::string, int> {
      ATL_TRY(good());
      return atl::expected<std::string, int>(atl::unexpect, 1);
    };
    const auto propagate_bad = [bad]() -> atl::expected<std::string, int> {
      ATL_TRY(bad());
      return atl::expected<std::string, int>("bar");
    };
    REQUIRE(propagate_good().error() == 1);
    REQUIRE(propagate_bad().error() == 0);
  }
  SECTION("Double propagation") {
    const auto good = [](const std::string& s) { return atl::expected<std::string, int>(s); };
    const auto bad = []() { return atl::expected<std::string, int>(atl::unexpect, 0); };
    const auto propagate = [good, bad]() -> atl::expected<std::string, int> {
      ATL_TRY(good("foo"));
      ATL_TRY(good("bar"));
      ATL_TRY(bad());
      return atl::expected<std::string, int>(atl::unexpect, 1);
    };
    REQUIRE(propagate().error() == 0);
  }
}


TEST_CASE("ATL_TRY_ASSIGN", "[extensions.macros]") {
  SECTION("string-expected") {
    const auto good = []() { return atl::expected<std::string, int>("foo"); };
    const auto bad = []() { return atl::expected<std::string, int>(atl::unexpect, 0); };
    const auto propagate_good = [good]() -> atl::expected<std::string, int> {
      ATL_TRY_ASSIGN(const auto x, good());
      return x;
    };
    const auto propagate_bad = [bad]() -> atl::expected<std::string, int> {
      ATL_TRY_ASSIGN(auto x, bad());
      return x;
    };
    REQUIRE(propagate_good().value() == "foo");
    REQUIRE(propagate_bad().error() == 0);
  }
  SECTION("Double propagation") {
    const auto good = [](const std::string& s) { return atl::expected<std::string, int>(s); };
    const auto bad = []() { return atl::expected<std::string, int>(atl::unexpect, 0); };
    const auto propagate = [good, bad]() -> atl::expected<std::string, int> {
      ATL_TRY_ASSIGN(auto x, good("foo"));
      ATL_TRY_ASSIGN(x, good("bar"));
      ATL_TRY_ASSIGN(x, bad());
      return x;
    };
    REQUIRE(propagate().error() == 0);
  }
}