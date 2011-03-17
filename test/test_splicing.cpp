/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Splicing tests
 */
#pragma warning(push)
#pragma warning(disable: 4265 4619 4548)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <rce/detail/splicing.h>

#include <algorithm>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4242 4244 4512)
#include <boost/assign/std/vector.hpp>
#pragma warning(pop)

#include "dump.h"

using namespace boost::assign;

struct SplicingFixture
{
    static const auto HOOK_FN_ADDR = 0;

    typedef std::vector<BYTE> vector_t;
    vector_t expected_;
    vector_t code_;
    vector_t saved_;

    SplicingFixture() : expected_(), code_(), saved_(24, 0x55) {}

    vector_t& expected() { return expected_; }
    vector_t& code() { return code_; }

    // don't add data to code_ after calling this : code_'s data may relocate
    void expected_write_rel32(int fromEndIdx, int destIdx)
    {
        *(int*)&expected_[fromEndIdx - 4] = &code_[destIdx] - &saved_[fromEndIdx];
    }

    void do_splice()
    {
        hook::detail::splice(&code_[0], (void*)HOOK_FN_ADDR, &saved_[0]);
    }

    boost::test_tools::predicate_result check() const
    {
        vector_t savedCode(saved_.begin(), std::find(saved_.begin(), saved_.end(), 0x55));
        if(savedCode == expected_)
            return true;

        boost::test_tools::predicate_result res(false);
        res.message()
            << "Saved not expected_ bytes:"
            << "\n  saved:    " << dump(savedCode)
            << "\n  expected: " << dump(expected_);
        return res;
    }
};

BOOST_FIXTURE_TEST_CASE(test_spliced_code, SplicingFixture)
{
    code() += 0x90, 0x90, 0x90, 0x90, 0x90, 0x90;

    do_splice();

    BOOST_CHECK_EQUAL(code_[0], 0xE9);
    BOOST_CHECK_EQUAL(*(int*)&code_[1], (BYTE*)HOOK_FN_ADDR - &code_[5]);
    BOOST_CHECK_EQUAL(code_[5], 0x90);
}

BOOST_FIXTURE_TEST_CASE(test_splice_fn_5_bytes_length, SplicingFixture)
{
    code() +=
        0x90, 0x90, 0x90, 0x90, // 0:4
        0xC3, // 4:5
        0x55; // added to make index "5" valid
    expected() +=
        0x90, 0x90, 0x90, 0x90, // 0:4
        0xC3, // 4:5
        0xE9, 0, 0, 0, 0; // 5:10
    expected_write_rel32(10, 5);

    do_splice();
    BOOST_CHECK(check());
}

BOOST_FIXTURE_TEST_CASE(test_splice_fn_with_call_rel32, SplicingFixture)
{
    code() +=
        0xE8, 0x01, 0x00, 0x00, 0x00, // 0:5
        0xC3, // 5:6
        0xC3; // 6:7
    expected() +=
        0xE8, 0, 0, 0, 0, // 0:5
        0xE9, 0, 0, 0, 0; // 5:10
    expected_write_rel32(5, 6);
    expected_write_rel32(10, 5);

    do_splice();
    BOOST_CHECK(check());
}

BOOST_FIXTURE_TEST_CASE(test_splice_fn_with_jmp_rel32, SplicingFixture)
{
    code() +=
        0x90, // 0:1
        0xE9, 0x00, 0x00, 0x00, 0x00, // 1:6
        0xC3; // 6:7
    expected() +=
        0x90, // 0:1
        0xE9, 0, 0, 0, 0; // 1:6
    expected_write_rel32(6, 6);

    do_splice();
    BOOST_CHECK(check());
}
