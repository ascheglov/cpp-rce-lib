/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Tests for thiscall_hooks.h
 */
#pragma warning(push)
#pragma warning(disable: 4265 4619 4548)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <rce/thiscall_hooks.h>

#include "hooks_fixture.h"

#pragma managed(push, off)
template<int (__stdcall* FUNC)(int)>
__declspec(naked)
int __stdcall call_thiscall(int /*x*/)
{
    static const auto addr = FUNC;
    __asm {
        pop eax // return address
        pop ecx // x
        push eax // ret addr
        jmp dword ptr [addr]
    }
}

__declspec(naked)
int __stdcall call_thiscall(int (__stdcall** /*addr*/)(int), int /*x*/)
{ __asm {
        pop eax // return address
        pop edx // addr
        pop ecx // x
        push eax // ret addr
        jmp dword ptr [edx]
} }
#pragma managed(pop)

//------------------------------------------------------
// test __thiscall function splicing
namespace thiscall_splicing
{
#pragma managed(push, off)
    __declspec(naked)
    int __stdcall func(int) // emulates `int __thiscall func(int)'
    { __asm {
        lea eax, [ecx + ecx + 1] // :4
        ret // :5
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
    BOOST_FIXTURE_TEST_CASE(test_splice_thiscall, HooksFixture)
    {
        enable_write(&func);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(call_thiscall<&func>(7), ((7 + 7) * 2 + 1) * 2);
        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(call_thiscall<&func>(8), 8 * 2 + 1);
        func_hook::install();
        BOOST_CHECK_EQUAL(call_thiscall<&func>(9), ((9 + 7) * 2 + 1) * 2);
        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_thiscall<&func>(10), 10 * 2 + 1);
    }
#pragma managed(pop)
}

//------------------------------------------------------
// test redirect thiscall hook
namespace redirect_thiscall_hook
{
#pragma managed(push, off)
    __declspec(naked)
    int __stdcall callee(int) // emulates `int __thiscall func(int)'
    { __asm {
        lea eax, [ecx + ecx * 2 + 1] // :4
        ret // :5
    } }

    __declspec(naked)
    int __stdcall func(int x)
    { __asm {
        push ebp // +00
        mov ebp, esp // +01
        mov ecx, x // +03
        call callee // +06
        leave
        ret 4
    } }
#pragma managed(pop)

    struct func_hook : hook::RedirectThiscall<func_hook, local_test_func<&func>, 6>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) * 2;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_redirect_thiscall, HooksFixture)
    {
        enable_write((char*)&func + 6);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(func(5), ((5 + 2) * 3 + 1) * 2);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(func(6), 6 * 3 + 1);

        func_hook::install();
        BOOST_CHECK_EQUAL(func(7), ((7 + 2) * 3 + 1) * 2);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(func(8), 8 * 3 + 1);
    }
#pragma managed(pop)
}

//------------------------------------------------------
// test thiscall ptr hook
namespace thiscall_ptr_hook
{
#pragma managed(push, off)
    __declspec(naked)
    int __stdcall func(int) // emulates `int __thiscall func(int)'
    { __asm {
        lea eax, [ecx + ecx * 4 + 1] // :4
        ret // :5
    } }
#pragma managed(pop)

    decltype(&func) func_ptr = &func;

    struct func_hook : hook::ThiscallPtrHook<func_hook, local_test_func<&func_ptr>, 0>
    {
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) * 3;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_hook_thiscall_ptr, HooksFixture)
    {
        enable_write(&func_ptr);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(call_thiscall(&func_ptr, 5), ((5 + 2) * 5 + 1) * 3);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(call_thiscall(&func_ptr, 6), 6 * 5 + 1);

        func_hook::install();
        BOOST_CHECK_EQUAL(call_thiscall(&func_ptr, 7), ((7 + 2) * 5 + 1) * 3);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_thiscall(&func_ptr, 8), 8 * 5 + 1);
    }
#pragma managed(pop)
}

//------------------------------------------------------
// test imported by name thiscall function hook
namespace imported_by_name_thiscall_fn_hook
{
#pragma managed(push, off)
    extern "C" __declspec(dllexport, naked)
    int __stdcall exported_thiscall_func(int)
    { __asm {
        lea eax, [ecx + ecx * 8 + 1] // :4
        ret // :5
    } }
#pragma managed(pop)

    struct this_module
    {
        static void* function(const char* name)
        {
            return GetProcAddress(GetModuleHandle(nullptr), name);
        }
    };

    struct func_hook : hook::SpliceThiscallByName<func_hook, this_module>
    {
        static const char* name() { return "_exported_thiscall_func@4"; }
        static int __stdcall hook(int x)
        {
            return get_original(hook)(x + 2) * 2;
        }
    };

#pragma managed(push, off)
    BOOST_FIXTURE_TEST_CASE(test_hook_thiscall_by_name, HooksFixture)
    {
        enable_write(&exported_thiscall_func);

        func_hook::set_installed(true);
        BOOST_CHECK_EQUAL(call_thiscall<&exported_thiscall_func>(5), ((5 + 2) * 9 + 1) * 2);

        func_hook::set_installed(false);
        BOOST_CHECK_EQUAL(call_thiscall<&exported_thiscall_func>(6), 6 * 9 + 1);

        func_hook::install();
        BOOST_CHECK_EQUAL(call_thiscall<&exported_thiscall_func>(7), ((7 + 2) * 9 + 1) * 2);

        func_hook::uninstall();
        BOOST_CHECK_EQUAL(call_thiscall<&exported_thiscall_func>(8), 8 * 9 + 1);
    }
#pragma managed(pop)
}
