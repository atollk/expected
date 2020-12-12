#ifndef ATL_EXPECTED_SUCCESS_T_H
#define ATL_EXPECTED_SUCCESS_T_H

namespace atl {

namespace detail {

template <typename T> struct typed_success_t {
  T t;

  template <typename T2, typename E> constexpr operator expected<T2, E>() const & {
    return expected<T2, E>(t);
  }

  template <typename T2, typename E> constexpr operator expected<T2, E>() const && {
    return expected<T2, E>(std::move(t));
  }
};

struct success_t {
  template <typename T> constexpr typed_success_t<T> operator()(T &&t) const {
    return typed_success_t<T>{std::forward<T>(t)};
  }

  template <typename E> constexpr operator expected<void, E>() const {
    return expected<void, E>();
  }
};

} // namespace detail

static constexpr auto success = detail::success_t{};

} // namespace atl

#endif // ATL_EXPECTED_SUCCESS_T_H
