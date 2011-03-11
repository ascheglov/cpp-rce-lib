/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Tests for thiscall_hooks.h
 */
#include <boost/test/unit_test.hpp>

#include <rce/thiscall_hooks.h>

#include "hooks_fixture.h"

//------------------------------------------------------
// test __thiscall function splicing
namespace splicing
{
#pragma managed(push, off)
    __declspec(naked)
    int __stdcall func() // emulates `int __thiscall func(int)'
    { __asm {
        lea eax, [ecx * 2 + 1] // :4
        ret // :5
    } }

    __declspec(naked)
    int __stdcall call_func(int /*x*/)
    { __asm {
        pop eax // return address
        pop ecx // x
        push eax // ret addr
        jmp func
    } }
#pragma managed(pop)

    struct func_hook : hook::SpliceThiscall<func_hook, local_test_func<&func>, 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 7) * 2;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_hook_thiscall, HooksFixture)
    {
        enable_write(&func);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(call_func(7), ((7 + 7) * 2 + 1) * 2);
        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(call_func(8), 8 * 2 + 1);
        func_hook::install();
        BOOST_CHECK_EQUAL(call_func(9), ((9 + 7) * 2 + 1) * 2);
        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_func(10), 10 * 2 + 1);
    }
#pragma managed(pop)
}
