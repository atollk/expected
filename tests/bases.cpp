#include <atl/expected.hpp>
#include <catch2/catch.hpp>

#include <string>

TEST_CASE("Triviality", "[bases.triviality]") {
  REQUIRE(std::is_trivially_copy_constructible<atl::expected<int, int>>::value);
  REQUIRE(std::is_trivially_copy_assignable<atl::expected<int, int>>::value);
  REQUIRE(std::is_trivially_move_constructible<atl::expected<int, int>>::value);
  REQUIRE(std::is_trivially_move_assignable<atl::expected<int, int>>::value);
  REQUIRE(std::is_trivially_destructible<atl::expected<int, int>>::value);

  REQUIRE(
      std::is_trivially_copy_constructible<atl::expected<void, int>>::value);
  REQUIRE(
      std::is_trivially_move_constructible<atl::expected<void, int>>::value);
  REQUIRE(std::is_trivially_destructible<atl::expected<void, int>>::value);

  {
    struct T {
      T(const T &) = default;
      T(T &&) = default;
      T &operator=(const T &) = default;
      T &operator=(T &&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_trivially_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_trivially_copy_assignable<atl::expected<T, int>>::value);
    REQUIRE(std::is_trivially_move_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_trivially_move_assignable<atl::expected<T, int>>::value);
    REQUIRE(std::is_trivially_destructible<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T &) {}
      T(T &&) {}
      T &operator=(const T &) {}
      T &operator=(T &&) {}
      ~T() {}
    };
    REQUIRE(
        !std::is_trivially_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_copy_assignable<atl::expected<T, int>>::value);
    REQUIRE(
        !std::is_trivially_move_constructible<atl::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_move_assignable<atl::expected<T, int>>::value);
    REQUIRE(!std::is_trivially_destructible<atl::expected<T, int>>::value);
  }
}

TEST_CASE("Deletion", "[bases.deletion]") {
  REQUIRE(std::is_copy_constructible<atl::expected<int, int>>::value);
  REQUIRE(std::is_copy_assignable<atl::expected<int, int>>::value);
  REQUIRE(std::is_move_constructible<atl::expected<int, int>>::value);
  REQUIRE(std::is_move_assignable<atl::expected<int, int>>::value);
  REQUIRE(std::is_destructible<atl::expected<int, int>>::value);

  {
    struct T {
      T() = default;
    };
    REQUIRE(std::is_default_constructible<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(int) {}
    };
    REQUIRE(!std::is_default_constructible<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T &) = default;
      T(T &&) = default;
      T &operator=(const T &) = default;
      T &operator=(T &&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_copy_assignable<atl::expected<T, int>>::value);
    REQUIRE(std::is_move_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_move_assignable<atl::expected<T, int>>::value);
    REQUIRE(std::is_destructible<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T &) = delete;
      T(T &&) = delete;
      T &operator=(const T &) = delete;
      T &operator=(T &&) = delete;
    };
    REQUIRE(!std::is_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(!std::is_copy_assignable<atl::expected<T, int>>::value);
    REQUIRE(!std::is_move_constructible<atl::expected<T, int>>::value);
    REQUIRE(!std::is_move_assignable<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T &) = delete;
      T(T &&) = default;
      T &operator=(const T &) = delete;
      T &operator=(T &&) = default;
    };
    REQUIRE(!std::is_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(!std::is_copy_assignable<atl::expected<T, int>>::value);
    REQUIRE(std::is_move_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_move_assignable<atl::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T &) = default;
      T(T &&) = delete;
      T &operator=(const T &) = default;
      T &operator=(T &&) = delete;
    };
    REQUIRE(std::is_copy_constructible<atl::expected<T, int>>::value);
    REQUIRE(std::is_copy_assignable<atl::expected<T, int>>::value);
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
    REQUIRE(std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_move_assignable<decltype(e)>::value);
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
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
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
}

