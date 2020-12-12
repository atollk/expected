#ifndef ATL_EXPECTED_EXPECTED_T_H
#define ATL_EXPECTED_EXPECTED_T_H

#include <utility>

#include "detail/expected_five_bases.h"
#include "detail/expected_operations_base.h"
#include "detail/expected_storage_base.h"

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define ATL_EXPECTED_EXCEPTIONS_ENABLED
#endif

#define FUNCTIONAL_METHOD(NAME)                                                \
  template <class F> constexpr auto NAME(F &&f) & {                            \
    return NAME##_impl(*this, std::forward<F>(f));                             \
  }                                                                            \
  template <class F> constexpr auto NAME(F &&f) && {                           \
    return NAME##_impl(std::move(*this), std::forward<F>(f));                  \
  }                                                                            \
  template <class F> constexpr auto NAME(F &&f) const & {                      \
    return NAME##_impl(*this, std::forward<F>(f));                             \
  }                                                                            \
  template <class F> constexpr auto NAME(F &&f) const && {                     \
    return NAME##_impl(std::move(*this), std::forward<F>(f));                  \
  }

namespace atl {

/// An `expected<T, E>` object is an object that contains the storage for
/// another object and manages the lifetime of this contained object `T`.
/// Alternatively it could contain the storage for another unexpected object
/// `E`. The contained object may not be initialized after the expected object
/// has been initialized, and may not be destroyed before the expected object
/// has been destroyed. The initialization state of the contained object is
/// tracked by the expected object.
template <class T, class E>
class expected : private detail::expected_move_assign_base<T, E>,
                 private detail::expected_delete_ctor_base<T, E>,
                 private detail::expected_delete_assign_base<T, E>,
                 private detail::expected_default_ctor_base<T, E> {
  static_assert(!std::is_reference<T>::value, "T must not be a reference");
  static_assert(!std::is_same<T, std::remove_cv<std::in_place_t>>::value,
                "T must not be in_place_t");
  static_assert(!std::is_same<T, std::remove_cv<unexpect_t>>::value,
                "T must not be unexpect_t");
  static_assert(!std::is_same<T, std::remove_cv<unexpected<E>>>::value,
                "T must not be unexpected<E>");
  static_assert(!std::is_reference<E>::value, "E must not be a reference");

  T *valptr() { return std::addressof(this->m_val); }
  const T *valptr() const { return std::addressof(this->m_val); }
  unexpected<E> *errptr() { return std::addressof(this->m_unexpect); }
  const unexpected<E> *errptr() const {
    return std::addressof(this->m_unexpect);
  }

  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr U &val() {
    return this->m_val;
  }
  constexpr unexpected<E> &err() { return this->m_unexpect; }

  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr const U &val() const {
    return this->m_val;
  }
  constexpr const unexpected<E> &err() const { return this->m_unexpect; }

  using impl_base = detail::expected_move_assign_base<T, E>;
  using ctor_base = detail::expected_default_ctor_base<T, E>;

public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  FUNCTIONAL_METHOD(and_then)
  FUNCTIONAL_METHOD(map)
  FUNCTIONAL_METHOD(transform)
  FUNCTIONAL_METHOD(map_error)
  FUNCTIONAL_METHOD(or_else)

  constexpr expected() = default;
  constexpr expected(const expected &rhs) = default;
  constexpr expected(expected &&rhs) = default;
  expected &operator=(const expected &rhs) = default;
  expected &operator=(expected &&rhs) = default;

  template <
      class... Args,
      std::enable_if_t<std::is_constructible<T, Args &&...>::value> * = nullptr>
  constexpr expected(std::in_place_t, Args &&...args)
      : impl_base(std::in_place, std::forward<Args>(args)...),
        ctor_base(detail::default_constructor_tag{}) {}

  template <class U, class... Args,
            std::enable_if_t<std::is_constructible<
                T, std::initializer_list<U> &, Args &&...>::value> * = nullptr>
  constexpr expected(std::in_place_t, std::initializer_list<U> il,
                     Args &&...args)
      : impl_base(std::in_place, il, std::forward<Args>(args)...),
        ctor_base(detail::default_constructor_tag{}) {}

  template <
      class G = E,
      std::enable_if_t<std::is_constructible<E, const G &>::value> * = nullptr,
      std::enable_if_t<!std::is_convertible<const G &, E>::value> * = nullptr>
  explicit constexpr expected(const unexpected<G> &e)
      : impl_base(unexpect, e.value()),
        ctor_base(detail::default_constructor_tag{}) {}

  template <
      class G = E,
      std::enable_if_t<std::is_constructible<E, const G &>::value> * = nullptr,
      std::enable_if_t<std::is_convertible<const G &, E>::value> * = nullptr>
  constexpr expected(unexpected<G> const &e)
      : impl_base(unexpect, e.value()),
        ctor_base(detail::default_constructor_tag{}) {}

  template <class G = E,
            std::enable_if_t<std::is_constructible<E, G &&>::value> * = nullptr,
            std::enable_if_t<!std::is_convertible<G &&, E>::value> * = nullptr>
  explicit constexpr expected(unexpected<G> &&e) noexcept(
      std::is_nothrow_constructible<E, G &&>::value)
      : impl_base(unexpect, std::move(e.value())),
        ctor_base(detail::default_constructor_tag{}) {}

  template <class G = E,
            std::enable_if_t<std::is_constructible<E, G &&>::value> * = nullptr,
            std::enable_if_t<std::is_convertible<G &&, E>::value> * = nullptr>
  constexpr expected(unexpected<G> &&e) noexcept(
      std::is_nothrow_constructible<E, G &&>::value)
      : impl_base(unexpect, std::move(e.value())),
        ctor_base(detail::default_constructor_tag{}) {}

  template <
      class... Args,
      std::enable_if_t<std::is_constructible<E, Args &&...>::value> * = nullptr>
  constexpr explicit expected(unexpect_t, Args &&...args)
      : impl_base(unexpect, std::forward<Args>(args)...),
        ctor_base(detail::default_constructor_tag{}) {}

  template <class U, class... Args,
            std::enable_if_t<std::is_constructible<
                E, std::initializer_list<U> &, Args &&...>::value> * = nullptr>
  constexpr explicit expected(unexpect_t, std::initializer_list<U> il,
                              Args &&...args)
      : impl_base(unexpect, il, std::forward<Args>(args)...),
        ctor_base(detail::default_constructor_tag{}) {}

  template <
      class U, class G,
      std::enable_if_t<!(std::is_convertible<U const &, T>::value &&
                         std::is_convertible<G const &, E>::value)> * = nullptr,
      detail::expected_enable_from_other<T, E, U, G, const U &, const G &> * =
          nullptr>
  explicit constexpr expected(const expected<U, G> &rhs)
      : ctor_base(detail::default_constructor_tag{}) {
    if (rhs.has_value()) {
      this->construct(*rhs);
    } else {
      this->construct_error(rhs.error());
    }
  }

  template <
      class U, class G,
      std::enable_if_t<(std::is_convertible<U const &, T>::value &&
                        std::is_convertible<G const &, E>::value)> * = nullptr,
      detail::expected_enable_from_other<T, E, U, G, const U &, const G &> * =
          nullptr>
  constexpr expected(const expected<U, G> &rhs)
      : ctor_base(detail::default_constructor_tag{}) {
    if (rhs.has_value()) {
      this->construct(*rhs);
    } else {
      this->construct_error(rhs.error());
    }
  }

  template <
      class U, class G,
      std::enable_if_t<!(std::is_convertible<U &&, T>::value &&
                         std::is_convertible<G &&, E>::value)> * = nullptr,
      detail::expected_enable_from_other<T, E, U, G, U &&, G &&> * = nullptr>
  explicit constexpr expected(expected<U, G> &&rhs)
      : ctor_base(detail::default_constructor_tag{}) {
    if (rhs.has_value()) {
      this->construct(std::move(*rhs));
    } else {
      this->construct_error(std::move(rhs.error()));
    }
  }

  template <
      class U, class G,
      std::enable_if_t<(std::is_convertible<U &&, T>::value &&
                        std::is_convertible<G &&, E>::value)> * = nullptr,
      detail::expected_enable_from_other<T, E, U, G, U &&, G &&> * = nullptr>
  constexpr expected(expected<U, G> &&rhs)
      : ctor_base(detail::default_constructor_tag{}) {
    if (rhs.has_value()) {
      this->construct(std::move(*rhs));
    } else {
      this->construct_error(std::move(rhs.error()));
    }
  }

  template <class U = T,
            std::enable_if_t<!std::is_convertible<U &&, T>::value> * = nullptr,
            detail::expected_enable_forward_value<T, E, U> * = nullptr>
  explicit constexpr expected(U &&v)
      : expected(std::in_place, std::forward<U>(v)) {}

  template <class U = T,
            std::enable_if_t<std::is_convertible<U &&, T>::value> * = nullptr,
            detail::expected_enable_forward_value<T, E, U> * = nullptr>
  constexpr expected(U &&v) : expected(std::in_place, std::forward<U>(v)) {}

  template <class U = T, class G = T,
            std::enable_if_t<std::is_nothrow_constructible<T, U &&>::value> * =
                nullptr,
            std::enable_if_t<!std::is_void<G>::value> * = nullptr,
            std::enable_if_t<(
                !std::is_same_v<expected<T, E>, std::decay_t<U>> &&
                !std::conjunction_v<std::is_scalar<T>,
                                    std::is_same<T, std::decay_t<U>>> &&
                std::is_constructible_v<T, U> && std::is_assignable_v<G &, U> &&
                std::is_nothrow_move_constructible_v<E>)> * = nullptr>
  expected &operator=(U &&v) {
    if (has_value()) {
      val() = std::forward<U>(v);
    } else {
      err().~unexpected<E>();
      ::new (valptr()) T(std::forward<U>(v));
      this->m_has_val = true;
    }

    return *this;
  }

  template <class U = T, class G = T,
            std::enable_if_t<!std::is_nothrow_constructible<T, U &&>::value> * =
                nullptr,
            std::enable_if_t<!std::is_void<U>::value> * = nullptr,
            std::enable_if_t<
                (!std::is_same<expected<T, E>, std::decay_t<U>>::value &&
                 !std::conjunction<std::is_scalar<T>,
                                   std::is_same<T, std::decay_t<U>>>::value &&
                 std::is_constructible<T, U>::value &&
                 std::is_assignable<G &, U>::value &&
                 std::is_nothrow_move_constructible<E>::value)> * = nullptr>
  expected &operator=(U &&v) {
    if (has_value()) {
      val() = std::forward<U>(v);
    } else {
      auto tmp = std::move(err());
      err().~unexpected<E>();

#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
      try {
        ::new (valptr()) T(std::forward<U>(v));
        this->m_has_val = true;
      } catch (...) {
        err() = std::move(tmp);
        throw;
      }
#else
      ::new (valptr()) T(std::forward<U>(v));
      this->m_has_val = true;
#endif
    }

    return *this;
  }

  template <class G = E,
            std::enable_if_t<std::is_nothrow_copy_constructible<G>::value &&
                             std::is_assignable<G &, G>::value> * = nullptr>
  expected &operator=(const unexpected<G> &rhs) {
    if (!has_value()) {
      err() = rhs;
    } else {
      this->destroy_val();
      ::new (errptr()) unexpected<E>(rhs);
      this->m_has_val = false;
    }

    return *this;
  }

  template <class G = E,
            std::enable_if_t<std::is_nothrow_move_constructible<G>::value &&
                             std::is_move_assignable<G>::value> * = nullptr>
  expected &operator=(unexpected<G> &&rhs) noexcept {
    if (!has_value()) {
      err() = std::move(rhs);
    } else {
      this->destroy_val();
      ::new (errptr()) unexpected<E>(std::move(rhs));
      this->m_has_val = false;
    }

    return *this;
  }

  template <class... Args, std::enable_if_t<std::is_nothrow_constructible<
                               T, Args &&...>::value> * = nullptr>
  void emplace(Args &&...args) {
    if (has_value()) {
      val() = T(std::forward<Args>(args)...);
    } else {
      err().~unexpected<E>();
      ::new (valptr()) T(std::forward<Args>(args)...);
      this->m_has_val = true;
    }
  }

  template <class... Args, std::enable_if_t<!std::is_nothrow_constructible<
                               T, Args &&...>::value> * = nullptr>
  void emplace(Args &&...args) {
    if (has_value()) {
      val() = T(std::forward<Args>(args)...);
    } else {
      auto tmp = std::move(err());
      err().~unexpected<E>();

#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
      try {
        ::new (valptr()) T(std::forward<Args>(args)...);
        this->m_has_val = true;
      } catch (...) {
        err() = std::move(tmp);
        throw;
      }
#else
      ::new (valptr()) T(std::forward<Args>(args)...);
      this->m_has_val = true;
#endif
    }
  }

  template <class U, class... Args,
            std::enable_if_t<std::is_nothrow_constructible<
                T, std::initializer_list<U> &, Args &&...>::value> * = nullptr>
  void emplace(std::initializer_list<U> il, Args &&...args) {
    if (has_value()) {
      T t(il, std::forward<Args>(args)...);
      val() = std::move(t);
    } else {
      err().~unexpected<E>();
      ::new (valptr()) T(il, std::forward<Args>(args)...);
      this->m_has_val = true;
    }
  }

  template <class U, class... Args,
            std::enable_if_t<!std::is_nothrow_constructible<
                T, std::initializer_list<U> &, Args &&...>::value> * = nullptr>
  void emplace(std::initializer_list<U> il, Args &&...args) {
    if (has_value()) {
      T t(il, std::forward<Args>(args)...);
      val() = std::move(t);
    } else {
      auto tmp = std::move(err());
      err().~unexpected<E>();

#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
      try {
        ::new (valptr()) T(il, std::forward<Args>(args)...);
        this->m_has_val = true;
      } catch (...) {
        err() = std::move(tmp);
        throw;
      }
#else
      ::new (valptr()) T(il, std::forward<Args>(args)...);
      this->m_has_val = true;
#endif
    }
  }

private:
  using t_is_void = std::true_type;
  using t_is_not_void = std::false_type;
  using t_is_nothrow_move_constructible = std::true_type;
  using move_constructing_t_can_throw = std::false_type;
  using e_is_nothrow_move_constructible = std::true_type;
  using move_constructing_e_can_throw = std::false_type;

  void swap_where_both_have_value(expected & /*rhs*/, t_is_void) noexcept {
    // swapping void is a no-op
  }

  void swap_where_both_have_value(expected &rhs, t_is_not_void) {
    using std::swap;
    swap(val(), rhs.val());
  }

  void swap_where_only_one_has_value(expected &rhs, t_is_void) noexcept(
      std::is_nothrow_move_constructible<E>::value) {
    ::new (errptr()) unexpected_type(std::move(rhs.err()));
    rhs.err().~unexpected_type();
    std::swap(this->m_has_val, rhs.m_has_val);
  }

  void swap_where_only_one_has_value(expected &rhs, t_is_not_void) {
    swap_where_only_one_has_value_and_t_is_not_void(
        rhs, typename std::is_nothrow_move_constructible<T>::type{},
        typename std::is_nothrow_move_constructible<E>::type{});
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected &rhs, t_is_nothrow_move_constructible,
      e_is_nothrow_move_constructible) noexcept {
    auto temp = std::move(val());
    val().~T();
    ::new (errptr()) unexpected_type(std::move(rhs.err()));
    rhs.err().~unexpected_type();
    ::new (rhs.valptr()) T(std::move(temp));
    std::swap(this->m_has_val, rhs.m_has_val);
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected &rhs, t_is_nothrow_move_constructible,
      move_constructing_e_can_throw) {
    auto temp = std::move(val());
    val().~T();
#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
    try {
      ::new (errptr()) unexpected_type(std::move(rhs.err()));
      rhs.err().~unexpected_type();
      ::new (rhs.valptr()) T(std::move(temp));
      std::swap(this->m_has_val, rhs.m_has_val);
    } catch (...) {
      val() = std::move(temp);
      throw;
    }
#else
    ::new (errptr()) unexpected_type(std::move(rhs.err()));
    rhs.err().~unexpected_type();
    ::new (rhs.valptr()) T(std::move(temp));
    std::swap(this->m_has_val, rhs.m_has_val);
#endif
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected &rhs, move_constructing_t_can_throw,
      t_is_nothrow_move_constructible) {
    auto temp = std::move(rhs.err());
    rhs.err().~unexpected_type();
#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
    try {
      ::new (rhs.valptr()) T(val());
      val().~T();
      ::new (errptr()) unexpected_type(std::move(temp));
      std::swap(this->m_has_val, rhs.m_has_val);
    } catch (...) {
      rhs.err() = std::move(temp);
      throw;
    }
#else
    ::new (rhs.valptr()) T(val());
    val().~T();
    ::new (errptr()) unexpected_type(std::move(temp));
    std::swap(this->m_has_val, rhs.m_has_val);
#endif
  }

public:
  template <class OT = T, class OE = E>
  std::enable_if_t<detail::is_swappable<OT>::value &&
                   detail::is_swappable<OE>::value &&
                   (std::is_nothrow_move_constructible<OT>::value ||
                    std::is_nothrow_move_constructible<OE>::value)>
  swap(expected &rhs) noexcept(
      std::is_nothrow_move_constructible<T>::value
          &&detail::is_nothrow_swappable<T>::value
              &&std::is_nothrow_move_constructible<E>::value
                  &&detail::is_nothrow_swappable<E>::value) {
    if (has_value() && rhs.has_value()) {
      swap_where_both_have_value(rhs, typename std::is_void<T>::type{});
    } else if (!has_value() && rhs.has_value()) {
      rhs.swap(*this);
    } else if (has_value()) {
      swap_where_only_one_has_value(rhs, typename std::is_void<T>::type{});
    } else {
      using std::swap;
      swap(err(), rhs.err());
    }
  }

  constexpr const T *operator->() const { return valptr(); }
  constexpr T *operator->() { return valptr(); }

  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr const U &operator*() const & {
    return val();
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr U &operator*() & {
    return val();
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr const U &&operator*() const && {
    return std::move(val());
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr U &&operator*() && {
    return std::move(val());
  }

  constexpr bool has_value() const noexcept { return this->m_has_val; }
  constexpr explicit operator bool() const noexcept { return this->m_has_val; }

  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr const U &value() const & {
    if (!has_value())
      detail::throw_exception(bad_expected_access<E>(err().value()));
    return val();
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr U &value() & {
    if (!has_value())
      detail::throw_exception(bad_expected_access<E>(err().value()));
    return val();
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr const U &&value() const && {
    if (!has_value())
      detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
    return std::move(val());
  }
  template <class U = T, std::enable_if_t<!std::is_void<U>::value> * = nullptr>
  constexpr U &&value() && {
    if (!has_value())
      detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
    return std::move(val());
  }

  constexpr const E &error() const & { return err().value(); }
  constexpr E &error() & { return err().value(); }
  constexpr const E &&error() const && { return std::move(err().value()); }
  constexpr E &&error() && { return std::move(err().value()); }

  template <class U> constexpr T value_or(U &&v) const & {
    static_assert(std::is_copy_constructible<T>::value &&
                      std::is_convertible<U &&, T>::value,
                  "T must be copy-constructible and convertible to from U&&");
    return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
  }
  template <class U> constexpr T value_or(U &&v) && {
    static_assert(std::is_move_constructible<T>::value &&
                      std::is_convertible<U &&, T>::value,
                  "T must be move-constructible and convertible to from U&&");
    return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
  }
};

template <class T, class E, class U, class F>
constexpr bool operator==(const expected<T, E> &lhs,
                          const expected<U, F> &rhs) {
  return (lhs.has_value() != rhs.has_value())
             ? false
             : (!lhs.has_value() ? lhs.error() == rhs.error() : *lhs == *rhs);
}
template <class T, class E, class U, class F>
constexpr bool operator!=(const expected<T, E> &lhs,
                          const expected<U, F> &rhs) {
  return (lhs.has_value() != rhs.has_value())
             ? true
             : (!lhs.has_value() ? lhs.error() != rhs.error() : *lhs != *rhs);
}

template <class T, class E, class U>
constexpr bool operator==(const expected<T, E> &x, const U &v) {
  return x.has_value() ? *x == v : false;
}
template <class T, class E, class U>
constexpr bool operator==(const U &v, const expected<T, E> &x) {
  return x.has_value() ? *x == v : false;
}
template <class T, class E, class U>
constexpr bool operator!=(const expected<T, E> &x, const U &v) {
  return x.has_value() ? *x != v : true;
}
template <class T, class E, class U>
constexpr bool operator!=(const U &v, const expected<T, E> &x) {
  return x.has_value() ? *x != v : true;
}

template <class T, class E>
constexpr bool operator==(const expected<T, E> &x, const unexpected<E> &e) {
  return x.has_value() ? false : x.error() == e.value();
}
template <class T, class E>
constexpr bool operator==(const unexpected<E> &e, const expected<T, E> &x) {
  return x.has_value() ? false : x.error() == e.value();
}
template <class T, class E>
constexpr bool operator!=(const expected<T, E> &x, const unexpected<E> &e) {
  return x.has_value() ? true : x.error() != e.value();
}
template <class T, class E>
constexpr bool operator!=(const unexpected<E> &e, const expected<T, E> &x) {
  return x.has_value() ? true : x.error() != e.value();
}

template <class T, class E,
          std::enable_if_t<(std::is_void<T>::value ||
                            std::is_move_constructible<T>::value) &&
                           detail::is_swappable<T>::value &&
                           std::is_move_constructible<E>::value &&
                           detail::is_swappable<E>::value> * = nullptr>
void swap(expected<T, E> &lhs,
          expected<T, E> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

} // namespace atl

#endif // ATL_EXPECTED_EXPECTED_T_H
