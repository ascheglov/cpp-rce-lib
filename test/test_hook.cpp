/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Function hooking tests
 */
#include <boost/test/unit_test.hpp>

#include <rce/hook.h>
#include <rce/memory.h>

//------------------------------------------------------
// helpers
template<typename F, F* addr>
struct local_test_func
{
    static void* ptr(int /*va*/) { return (void*)addr; }
};

#define RCE_TEST_LOCAL_FUNC(name) local_test_func<decltype(name), &name>

//------------------------------------------------------
// test install/uninstall
namespace install_uninstall
{
#pragma managed(push, off) // can't hook managed code
    __declspec(noinline)
    int __stdcall func(int x)
    {
        return x * 100;
    }
#pragma managed(pop)

    struct func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) + 1;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_install_uninstall)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(5), (5 + 2) * 100 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(6), 6 * 100);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(7), (7 + 2) * 100 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(8), 8 * 100);
    }
}

//------------------------------------------------------
// test __thiscall function
namespace thiscall_wrap
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

    struct func_hook : hook::SpliceThiscall<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 7) * 2;
        }
    };

#pragma managed(push, off)
    BOOST_AUTO_TEST_CASE(test_hook_thiscall)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(call_func(7), ((7 + 7) * 2 + 1) * 2);
        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_func(8), 8 * 2 + 1);
        func_hook::install();
        BOOST_CHECK_EQUAL(call_func(9), ((9 + 7) * 2 + 1) * 2);
        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_func(10), 10 * 2 + 1);
    }
#pragma managed(pop)
}
