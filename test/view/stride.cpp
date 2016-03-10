// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

#include <list>
#include <vector>
#include <sstream>
#include <range/v3/core.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/numeric.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"

int main()
{
    using namespace ranges;

    std::vector<int> v(50);
    iota(v, 0);

#if !defined(_MSC_VER) // MS ABI has limited EBO
    static_assert(
        sizeof((v|view::stride(3)).begin()) ==
        sizeof(void*)+sizeof(v.begin())+sizeof(std::ptrdiff_t),"");
#endif
    ::check_equal(v | view::stride(3) | view::reverse,
                  {48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0});

    std::stringstream str;
    copy(v, ostream_iterator<int>{str, " "});
    auto x = istream<int>(str) | view::stride(3);
    CONCEPT_ASSERT(InputView<decltype(x)>());
    CONCEPT_ASSERT(!InputView<decltype(x) const>());
    CONCEPT_ASSERT(!ForwardView<decltype(x)>());
    check_equal(x, {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48});

    std::list<int> li;
    copy(v, back_inserter(li));
    ::check_equal(li | view::stride(3),
                  {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48});
    ::check_equal(li | view::stride(3) | view::reverse,
                  {48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0});

    for(int i : li | view::stride(3) | view::reverse)
        std::cout << i << ' ';
    std::cout << '\n';

    auto x2 = v | view::stride(3);
    CHECK(ranges::distance(x2) == 17);

    auto it0 = x2.begin();
    auto it1 = std::next(it0, 10);
    CHECK((it1 - it0) == 10);
    CHECK((it0 - it1) == -10);
    CHECK((it0 - it0) == 0);
    CHECK((it1 - it1) == 0);

    return ::test_result();
}
