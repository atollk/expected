#include <catch2/catch.hpp>
#include <atl/expected.hpp>
#include <memory>
#include <vector>
#include <tuple>

namespace {
struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args &&... args)
      : v(l), t(std::forward<Args>(args)...) {}
};
}

TEST_CASE("Emplace", "[emplace]") {
    {
        atl::expected<std::unique_ptr<int>,int> e;
        e.emplace(new int{42});
        REQUIRE(e);
        REQUIRE(**e == 42);
    }

    {
        atl::expected<std::vector<int>,int> e;
        e.emplace({0,1});
        REQUIRE(e);
        REQUIRE((*e)[0] == 0);
        REQUIRE((*e)[1] == 1);
    }

    {
        atl::expected<std::tuple<int,int>,int> e;
        e.emplace(2,3);
        REQUIRE(e);
        REQUIRE(std::get<0>(*e) == 2);
        REQUIRE(std::get<1>(*e) == 3);
    }

    {
        atl::expected<takes_init_and_variadic,int> e = atl::make_unexpected(0);
        e.emplace({0,1}, 2, 3);
        REQUIRE(e);
        REQUIRE(e->v[0] == 0);
        REQUIRE(e->v[1] == 1);
        REQUIRE(std::get<0>(e->t) == 2);
        REQUIRE(std::get<1>(e->t) == 3);
    }
}
