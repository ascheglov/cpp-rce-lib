/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * hook.h test
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
    __declspec(noinline)
    int __stdcall func(int x)
    {
        return x + 100;
    }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x * 2) + 1;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_install_uninstall)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(5), 5 * 2 + 100 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(6), 6 + 100);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(7), 7 * 2 + 100 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(8), 8 + 100);
    }
}

//------------------------------------------------------
// test 5 bytes length function
namespace five_bytes_length
{
    __declspec(noinline, naked)
    char __stdcall func()
    {
        __asm nop // :1
        __asm nop // :2
        __asm mov al, 21 // :4
        __asm ret // :5
    }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static char __stdcall hook()
        {
            return get_original(hook)() * 2;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_5_byte_length)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(), 21 * 2);
    }
}

//------------------------------------------------------
// test function with jmp rel32 at start
namespace starts_jmp_rel32
{
    __declspec(noinline, naked)
    int __stdcall func()
    { __asm {
        _emit 0xE9 __asm _emit 0 __asm _emit 0 __asm _emit 0 __asm _emit 0  // :5
        mov eax, 3
        ret
    } }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static char __stdcall hook()
        {
            return get_original(hook)() * 3;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_starts_jmp_rel32)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(), 3 * 3);
    }
}

//------------------------------------------------------
// test function with call rel32 at start
namespace starts_call_rel32
{
    __declspec(noinline, naked)
    int __stdcall func()
    { __asm {
        call m1  // :5
        mov eax, 4
        ret
m1:
        ret
    } }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static char __stdcall hook()
        {
            return get_original(hook)() * 4;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_starts_call_rel32)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(), 4 * 4);
    }
}

//------------------------------------------------------
// test function with jmp rel32
namespace with_jmp_rel32
{
    __declspec(noinline, naked)
    int __stdcall func()
    { __asm {
        mov cl, 1 // :2
        _emit 0xE9 __asm _emit 0 __asm _emit 0 __asm _emit 0 __asm _emit 0  // :7
        mov eax, 5
        ret
    } }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static char __stdcall hook()
        {
            return get_original(hook)() * 5;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_with_jmp_rel32)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(), 5 * 5);
    }
}

//------------------------------------------------------
// test function with call rel32
namespace with_call_rel32
{
    __declspec(noinline, naked)
    int __stdcall func()
    { __asm {
        mov cl, 1 // :2
        call m1  // :7
        mov eax, 6
        ret
m1:
        ret
    } }

    struct  func_hook : hook::SpliceCodeHook<func_hook, RCE_TEST_LOCAL_FUNC(func), 0>
    {
        static char __stdcall hook()
        {
            return get_original(hook)() * 6;
        }
    };

    BOOST_AUTO_TEST_CASE(test_hook_with_call_rel32)
    {
        rce::MakeWriteable unlock(&func, 15);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(), 6 * 6);
    }
}