#include <atl/expected.hpp>
#include <catch2/catch.hpp>

#include <string>
#include <type_traits>
#include <vector>

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args &&...args)
      : v(l), t(std::forward<Args>(args)...) {}
};

TEST_CASE("Constructors", "[constructors]") {
  {
    atl::expected<int, int> e;
    REQUIRE(e);
    REQUIRE(e == 0);
  }

  {
    atl::expected<int, int> e = atl::make_unexpected(0);
    REQUIRE(!e);
    REQUIRE(e.error() == 0);
  }

  {
    atl::expected<int, int> e(atl::unexpect, 0);
    REQUIRE(!e);
    REQUIRE(e.error() == 0);
  }

  {
    atl::expected<int, int> e(std::in_place, 42);
    REQUIRE(e);
    REQUIRE(e == 42);
  }

  {
    atl::expected<std::vector<int>, int> e(std::in_place, {0, 1});
    REQUIRE(e);
    REQUIRE((*e)[0] == 0);
    REQUIRE((*e)[1] == 1);
  }

  {
    atl::expected<std::tuple<int, int>, int> e(std::in_place, 0, 1);
    REQUIRE(e);
    REQUIRE(std::get<0>(*e) == 0);
    REQUIRE(std::get<1>(*e) == 1);
  }

  {
    atl::expected<takes_init_and_variadic, int> e(std::in_place, {0, 1}, 2, 3);
    REQUIRE(e);
    REQUIRE(e->v[0] == 0);
    REQUIRE(e->v[1] == 1);
    REQUIRE(std::get<0>(e->t) == 2);
    REQUIRE(std::get<1>(e->t) == 3);
  }

  {
    atl::expected<int, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_constructible_v<decltype(e)>);
    REQUIRE(std::is_trivially_copy_assignable_v<decltype(e)>);
  }

  {
    atl::expected<int, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible_v<decltype(e)>);
    REQUIRE(!std::is_trivially_copy_assignable_v<decltype(e)>);
  }

  {
    atl::expected<std::string, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible_v<decltype(e)>);
    REQUIRE(!std::is_trivially_copy_assignable_v<decltype(e)>);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    atl::expected<std::string, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible_v<decltype(e)>);
    REQUIRE(!std::is_trivially_copy_assignable_v<decltype(e)>);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    atl::expected<void, int> e;
    REQUIRE(e);
  }

  {
    atl::expected<void, int> e(atl::unexpect, 42);
    REQUIRE(!e);
    REQUIRE(e.error() == 42);
  }
}
