#ifndef ATL_EXPECTED_BAD_EXPECTED_ACCESS_H
#define ATL_EXPECTED_BAD_EXPECTED_ACCESS_H

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define ATL_EXPECTED_EXCEPTIONS_ENABLED
#endif

namespace atl {
template <class E> class bad_expected_access : public std::exception {
public:
  explicit bad_expected_access(E e) : m_val(std::move(e)) {}

  virtual const char *what() const noexcept override {
    return "Bad expected access";
  }

  const E &error() const & { return m_val; }
  E &error() & { return m_val; }
  const E &&error() const && { return std::move(m_val); }
  E &&error() && { return std::move(m_val); }

private:
  E m_val;
};

namespace detail {
template <typename E> [[noreturn]] constexpr void throw_exception(E &&e) {
#ifdef ATL_EXPECTED_EXCEPTIONS_ENABLED
  throw std::forward<E>(e);
#else
#ifdef _MSC_VER
  __assume(0);
#else
  __builtin_unreachable();
#endif
#endif
}
} // namespace detail

} // namespace atl

#endif // ATL_EXPECTED_BAD_EXPECTED_ACCESS_H
