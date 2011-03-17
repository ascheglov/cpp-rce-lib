/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Classes for hooking functions from native code
 */
#pragma once

#include "patching.h"
#include "detail/types.h"
#include "detail/rwx_memory_pool.h"
#include "detail/hook_base.h"

#pragma managed(push, off)

namespace hook {

// splice any code
template<class Derived, class Module, int rva=0>
struct SpliceCodeHook : detail::HookBase<Derived>
{
    static void* hook_addr()
    {
        return Module::ptr(rva);
    }

    static void install()
    {
        auto addr = (detail::BYTE*)Derived::hook_addr();
        if(addr[0] == 0xE9) // jmp rel32
        {
            callAddr = (call_addr_t)redirect_call_or_jmp(addr, Derived::hook_fn());
        }
        else
        {
            callAddr = (call_addr_t)detail::global_rwx_memory_pool::get_rwx_mem();
            splice(addr, Derived::hook_fn(), (void*)callAddr);
        }
    }

    static void uninstall()
    {
        write_jmp(Derived::hook_addr(), (void*)callAddr);
    }
};

template<class Derived, class Module, int rva=0>
struct RedirectCallee : detail::HookBase<Derived>
{
    static void install()
    {
        auto addr = (detail::BYTE*)Module::ptr(rva);
#if defined(_DEBUG)
        if(addr[0] != 0xE8) // call rel32
            __debugbreak();
#endif
        callAddr = (call_addr_t)redirect_call_or_jmp(addr, Derived::hook_fn());
    }

    static void uninstall()
    {
        redirect_call_or_jmp(Module::ptr(rva), (void*)callAddr);
    }
};

template<class Derived, class Module, int rva=0>
struct FnPtrHook : detail::HookBase<Derived>
{
    static void install()
    {
        auto ptr = (void**)Module::ptr(rva);
        callAddr = (call_addr_t)*ptr;
        *ptr = Derived::hook_fn();
    }

    static void uninstall()
    {
        *(call_addr_t*)Module::ptr(rva) = callAddr;
    }
};

template<class Derived, class Module>
struct SpliceImportByName : SpliceCodeHook<Derived, Module>
{
    static void* hook_addr()
    {
        return Module::function(Derived::name());
    }
};

} // namespace hook

#pragma managed(pop)
