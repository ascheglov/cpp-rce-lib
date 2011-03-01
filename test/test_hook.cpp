/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Function hooking tests
 */
#include <boost/test/unit_test.hpp>

#include <rce/hook.h>

//------------------------------------------------------
// helpers

struct HooksFixture
{
    HooksFixture() : funcAddr() {}

    void* funcAddr;
    DWORD oldProtect;

    void enable_write(void* func)
    {
        funcAddr = func;
        VirtualProtect(funcAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    }
    ~HooksFixture()
    {
        if(funcAddr)
            VirtualProtect(funcAddr, 5, oldProtect, &oldProtect);
    }
};

template<void* addr>
struct local_test_func
{
    static void* ptr(int /*va*/) { return addr; }
};

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

    struct func_hook : hook::SpliceCodeHook<func_hook, local_test_func<&func>, 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) + 1;
        }
    };

    BOOST_FIXTURE_TEST_CASE(test_hook_install_uninstall, HooksFixture)
    {
        enable_write(&func);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(func(5), (5 + 2) * 100 + 1);

        func_hook::set_installed(false);
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

//------------------------------------------------------
// test function with jmp rel32 at start
// in this case jmp is just redirected to hook
namespace starts_jmp_rel32
{
#pragma managed(push, off)
    __declspec(naked)
    int __stdcall func()
    { __asm {
        _emit 0xE9 __asm _emit 0 __asm _emit 0 __asm _emit 0 __asm _emit 0  // :5
        mov eax, 3
        ret
    } }
#pragma managed(pop)

    struct func_hook : hook::SpliceCodeHook<func_hook, local_test_func<&func>, 0>
    {
        static int __stdcall hook()
        {
            return get_original(hook)() * 3;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_hook_starts_jmp_rel32, HooksFixture)
    {
        enable_write(&func);

        func_hook::install();
        BOOST_CHECK_EQUAL(func_hook::callAddr, (DWORD)&func + 5);
        BOOST_CHECK_EQUAL(func(), 3 * 3);
    }
#pragma managed(pop)
}
