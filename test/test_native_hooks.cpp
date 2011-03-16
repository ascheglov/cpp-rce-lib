/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Tests for native_hooks.h
 */
#include <boost/test/unit_test.hpp>

#include <rce/native_hooks.h>

#include "hooks_fixture.h"

//------------------------------------------------------
// test splicing
namespace splicing
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

    BOOST_FIXTURE_TEST_CASE(test_splicing, HooksFixture)
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

//------------------------------------------------------
// test callee hook
namespace callee_hook
{
    int __stdcall func2(int x)
    {
        return x * 100;
    }

#pragma managed(push, off)
    __declspec(naked)
    int __stdcall func(int x)
    { __asm {
        push ebp // +00
        mov ebp, esp // +01
        push x // +03
        call func2 // +06
        leave
        ret 4
    } }
#pragma managed(pop)

    struct func_hook : hook::RedirectCallee<func_hook, local_test_func<&func>, 6>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) + 1;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_hook_callee, HooksFixture)
    {
        enable_write((char*)&func + 6);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(func(5), (5 + 2) * 100 + 1);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(func(6), 6 * 100);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(7), (7 + 2) * 100 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(8), 8 * 100);
    }
#pragma managed(pop)
}

//------------------------------------------------------
// test function ptr hook
namespace function_ptr_hook
{
    int __stdcall func(int x)
    {
        return x * 1000;
    }

    decltype(&func) func_ptr = &func;

    struct func_hook : hook::FnPtrHook<func_hook, local_test_func<&func_ptr>, 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) + 1;
        }
    };

    BOOST_FIXTURE_TEST_CASE(test_hook_ptr, HooksFixture)
    {
        enable_write(&func_ptr);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(func_ptr(5), (5 + 2) * 1000 + 1);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(func_ptr(6), 6 * 1000);

        func_hook::install();
        BOOST_CHECK_EQUAL(func_ptr(7), (7 + 2) * 1000 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func_ptr(8), 8 * 1000);
    }
}

//------------------------------------------------------
// test imported by name function hook
namespace imported_by_name_fn_hook
{
#pragma managed(push, off)
    extern "C" __declspec(dllexport, noinline)
    int __cdecl exported_func(int x)
    {
        return x * 200;
    }
#pragma managed(pop)

    struct this_module
    {
        static void* function(const char* name)
        {
            return GetProcAddress(GetModuleHandle(nullptr), name);
        }
    };

    struct func_hook : hook::SpliceImportByName<func_hook, this_module>
    {
        static const char* name() { return "exported_func"; }
        static int __cdecl hook(int x)
        {
            return get_original(hook)(x + 2) + 1;
        }
    };

    BOOST_FIXTURE_TEST_CASE(test_hook_import_by_name, HooksFixture)
    {
        enable_write(&exported_func);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(exported_func(5), (5 + 2) * 200 + 1);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(exported_func(6), 6 * 200);

        func_hook::install();
        BOOST_CHECK_EQUAL(exported_func(7), (7 + 2) * 200 + 1);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(exported_func(8), 8 * 200);
    }
}
