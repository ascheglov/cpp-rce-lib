/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Classes for hooking functions from native code
 */
#pragma once

#include <Windows.h> // TODO: remove it

#include "detail/instructions.h"
#include "detail/splicing.h"
#include "detail/rwx_memory_pool.h"
#include "detail/hook_base.h"

#pragma managed(push, off)

namespace hook {

// splice any code
template<class Derived, class Module, int rva=0>
struct SpliceCodeHook : detail::HookBase<Derived>
{
    static BYTE* hook_addr()
    {
        return (BYTE*)Module::ptr(rva);
    }

    static void install()
    {
        auto addr = Derived::hook_addr();
        if(addr[0] == 0xE9) // jmp rel32
        {
            callAddr = (call_addr_t)detail::redirect_jmp(addr, Derived::hook_fn());
        }
        else
        {
            callAddr = (call_addr_t)detail::global_rwx_memory_pool::get_rwx_mem();
            detail::splice(addr, Derived::hook_fn(), (BYTE*)callAddr);
        }
    }

    static void uninstall()
    {
        detail::write_jmp(Derived::hook_addr(), (void*)callAddr);
    }
};

template<class Derived, class Module, int rva=0>
struct RedirectCallee : detail::HookBase<Derived>
{
    static void install(const void* hookFn)
    {
        auto addr = (BYTE*)Module::ptr(rva);
#if defined(_DEBUG)
        if(addr[0] != 0xE8) // call rel32
            __debugbreak();
#endif
        callAddr = (call_addr_t)detail::redirect_jmp(addr, hookFn);
    }

    static void install()
    {
        install(&Derived::hook);
    }

    static void uninstall()
    {
        detail::redirect_jmp((BYTE*)Module::ptr(rva), (void*)callAddr);
    }
};

template<class Derived, class Module, int rva=0>
struct FnPtrHook : detail::HookBase<Derived>
{
    static void install()
    {
        auto ptr = (DWORD*)Module::ptr(rva);
        callAddr = (call_addr_t)*ptr;
        *ptr = (DWORD)&Derived::hook;
    }

    static void uninstall()
    {
        *(call_addr_t*)Module::ptr(rva) = callAddr;
    }
};

template<class Derived, class Module>
struct SpliceImportByName : SpliceCodeHook<Derived, Module>
{
    static BYTE* hook_addr()
    {
        return (BYTE*)Module::function(Derived::name());
    }
};

} // namespace hook

#pragma managed(pop)
