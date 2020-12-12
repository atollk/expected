#ifndef ATL_EXPECTED_NO_INIT_T_H
#define ATL_EXPECTED_NO_INIT_T_H

namespace atl::detail {
struct no_init_t {};
static constexpr no_init_t no_init{};
} // namespace detail

#endif // ATL_EXPECTED_NO_INIT_T_H
