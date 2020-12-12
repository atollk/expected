///
// expected - An implementation of std::expected with extensions
// Started in 2017 by Simon Brand (simonrbrand@gmail.com, @TartanLlama)
// Extended in 2020 by Andreas Tollkötter (@atollk)
//
// Documentation available at
//
// To the extent possible under law, the author(s) have dedicated all
// copyright and related and neighboring rights to this software to the
// public domain worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication
// along with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
///

#ifndef ATL_EXPECTED_HPP
#define ATL_EXPECTED_HPP

#define ATL_EXPECTED_VERSION_MAJOR 1
#define ATL_EXPECTED_VERSION_MINOR 1
#define ATL_EXPECTED_VERSION_PATCH 0

#include <exception>
#include <functional>
#include <type_traits>
#include <utility>

namespace atl {
template <class T, class E> class expected;
} // namespace atl

#include "unexpected_t.h"
#include "detail/traits.h"
#include "detail/expected_storage_base.h"
#include "detail/expected_operations_base.h"
#include "detail/expected_five_bases.h"
#include "bad_expected_access.h"
#include "expected_t.h"
#include "detail/functional_extensions.h"
#include "success.h"

#ifndef ATL_EXPECTED_DISABLE_MACROS
#include "macros.h"
#endif

#endif
