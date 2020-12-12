#ifndef ATL_EXPECTED_MACROS_H
#define ATL_EXPECTED_MACROS_H

#define ATL_TRY(VAL)                                                           \
  if (auto _atl_macro_propagate_error = (VAL);                                 \
      !_atl_macro_propagate_error.has_value())                                 \
    return _atl_macro_propagate_error;

#define ATL_TRY_ASSIGN(TARGET, VAL)                                            \
  _ATL_TRY_ASSIGN(TARGET, VAL, _UNIQUE_TMP_ID(_atl_macro_assign_or_propagate))

namespace detail {

#define _ATL_TRY_ASSIGN(TARGET, VAL, TMP)                                      \
  auto TMP = (VAL);                                                            \
  if (!TMP.has_value())                                                        \
    return static_cast<decltype(TMP)::unexpected_type>(TMP);                   \
  TARGET = std::move(TMP).value();

#define _CONCAT1(X, Y) X##Y
#define _CONCAT2(X, Y) _CONCAT1(X, Y)
#ifdef __COUNTER__
#define _UNIQUE_TMP_ID(X) _CONCAT2(X, __COUNTER__)
#else
#define _UNIQUE_TMP_ID(X) _CONCAT2(X, __LINE__)
#endif

} // namespace detail

#endif // ATL_EXPECTED_MACROS_H
