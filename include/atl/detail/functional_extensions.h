#ifndef ATL_EXPECTED_FUNCTIONAL_EXTENSIONS_H
#define ATL_EXPECTED_FUNCTIONAL_EXTENSIONS_H

#include <type_traits>

namespace atl::detail {
struct monostate {};
template <class Exp> using exp_t = typename std::decay_t<Exp>::value_type;
template <class Exp> using err_t = typename std::decay_t<Exp>::error_type;
template <class Exp, class Ret> using ret_t = expected<Ret, err_t<Exp>>;

template <class> struct TC;
template <class Exp, class F,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           *std::declval<Exp>())),
          std::enable_if_t<!std::is_void<exp_t<Exp>>::value> * = nullptr>
auto and_then_impl(Exp &&exp, F &&f) -> Ret {
  static_assert(detail::is_expected<Ret>::value, "F must return an expected");

  return exp.has_value()
             ? std::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
             : Ret(unexpect, std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          class Ret = decltype(std::invoke(std::declval<F>())),
          std::enable_if_t<std::is_void<exp_t<Exp>>::value> * = nullptr>
constexpr auto and_then_impl(Exp &&exp, F &&f) -> Ret {
  static_assert(detail::is_expected<Ret>::value, "F must return an expected");

  return exp.has_value() ? std::invoke(std::forward<F>(f))
                         : Ret(unexpect, std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          std::enable_if_t<!std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           *std::declval<Exp>())),
          std::enable_if_t<!std::is_void<Ret>::value> * = nullptr>

constexpr auto map_impl(Exp &&exp, F &&f)
    -> ret_t<Exp, std::decay_t<Ret>> {
  using result = ret_t<Exp, std::decay_t<Ret>>;

  return exp.has_value()
             ? result(std::invoke(std::forward<F>(f), *std::forward<Exp>(exp)))
             : result(unexpect, std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          std::enable_if_t<!std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           *std::declval<Exp>())),
          std::enable_if_t<std::is_void<Ret>::value> * = nullptr>

auto map_impl(Exp &&exp, F &&f) -> expected<void, err_t<Exp>> {
  if (exp.has_value()) {
    std::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
    return {};
  }

  return unexpected<err_t<Exp>>(std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          std::enable_if_t<std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>())),
          std::enable_if_t<!std::is_void<Ret>::value> * = nullptr>

constexpr auto map_impl(Exp &&exp, F &&f)
    -> ret_t<Exp, std::decay_t<Ret>> {
  using result = ret_t<Exp, std::decay_t<Ret>>;

  return exp.has_value() ? result(std::invoke(std::forward<F>(f)))
                         : result(unexpect, std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          std::enable_if_t<std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>())),
          std::enable_if_t<std::is_void<Ret>::value> * = nullptr>

auto map_impl(Exp &&exp, F &&f) -> expected<void, err_t<Exp>> {
  if (exp.has_value()) {
    std::invoke(std::forward<F>(f));
    return {};
  }

  return unexpected<err_t<Exp>>(std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          std::enable_if_t<!std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<!std::is_void<Ret>::value> * = nullptr>
constexpr auto map_error_impl(Exp &&exp, F &&f)
    -> expected<exp_t<Exp>, std::decay_t<Ret>> {
  using result = expected<exp_t<Exp>, std::decay_t<Ret>>;

  return exp.has_value()
             ? result(*std::forward<Exp>(exp))
             : result(unexpect, std::invoke(std::forward<F>(f),
                                            std::forward<Exp>(exp).error()));
}

template <class Exp, class F,
          std::enable_if_t<!std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<std::is_void<Ret>::value> * = nullptr>
auto map_error_impl(Exp &&exp, F &&f) -> expected<exp_t<Exp>, monostate> {
  using result = expected<exp_t<Exp>, monostate>;
  if (exp.has_value()) {
    return result(*std::forward<Exp>(exp));
  }

  std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
  return result(unexpect, monostate{});
}

template <class Exp, class F,
          std::enable_if_t<std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<!std::is_void<Ret>::value> * = nullptr>
constexpr auto map_error_impl(Exp &&exp, F &&f)
    -> expected<exp_t<Exp>, std::decay_t<Ret>> {
  using result = expected<exp_t<Exp>, std::decay_t<Ret>>;

  return exp.has_value()
             ? result()
             : result(unexpect, std::invoke(std::forward<F>(f),
                                            std::forward<Exp>(exp).error()));
}

template <class Exp, class F,
          std::enable_if_t<std::is_void<exp_t<Exp>>::value> * = nullptr,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<std::is_void<Ret>::value> * = nullptr>
auto map_error_impl(Exp &&exp, F &&f) -> expected<exp_t<Exp>, monostate> {
  using result = expected<exp_t<Exp>, monostate>;
  if (exp.has_value()) {
    return result();
  }

  std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
  return result(unexpect, monostate{});
}

template <class Exp, class F,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<!std::is_void<Ret>::value> * = nullptr>
auto or_else_impl(Exp &&exp, F &&f) -> Ret {
  static_assert(detail::is_expected<Ret>::value, "F must return an expected");
  return exp.has_value()
             ? std::forward<Exp>(exp)
             : std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
}

template <class Exp, class F,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           std::declval<Exp>().error())),
          std::enable_if_t<std::is_void<Ret>::value> * = nullptr>
std::decay_t<Exp> or_else_impl(Exp &&exp, F &&f) {
  return exp.has_value()
             ? std::forward<Exp>(exp)
             : (std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error()),
                std::forward<Exp>(exp));
}
} // namespace atl::detail

#endif // ATL_EXPECTED_FUNCTIONAL_EXTENSIONS_H
