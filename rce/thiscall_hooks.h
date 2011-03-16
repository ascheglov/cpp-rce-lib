/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Classes for hooking __thiscall functions from native code,
 * compatible with /clr compiler option.
 */
#pragma once

#include "detail/thiscall_wrap.h"
#include "native_hooks.h"

#pragma managed(push, off)

namespace hook {

// splice __thiscall function
template<class Derived, class Module, int rva=0>
struct SpliceThiscall : SpliceCodeHook<Derived, Module, rva>, detail::ThiscallWrapMixin<Derived>
{
    using ThiscallWrapMixin::get_original;
    using ThiscallWrapMixin::hook_fn;
};

template<class Derived, class Module, int rva=0>
struct RedirectThiscall : RedirectCallee<Derived, Module, rva>, detail::ThiscallWrapMixin<Derived>
{
    using ThiscallWrapMixin::get_original;
    using ThiscallWrapMixin::hook_fn;
};

template<class Derived, class Module, int rva=0>
struct ThiscallPtrHook : FnPtrHook<Derived, Module, rva>, detail::ThiscallWrapMixin<Derived>
{
    using ThiscallWrapMixin::get_original;
    using ThiscallWrapMixin::hook_fn;
};

template<class Derived, class Module>
struct SpliceThiscallByName : SpliceImportByName<Derived, Module>, detail::ThiscallWrapMixin<Derived>
{
    using ThiscallWrapMixin::get_original;
    using ThiscallWrapMixin::hook_fn;
};

} // namespace hook

#pragma managed(pop)
