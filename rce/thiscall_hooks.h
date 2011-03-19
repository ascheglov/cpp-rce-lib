/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 *
 * Classes for hooking __thiscall functions from native code,
 * compatible with /clr compiler option.
 */
#pragma once

#include "detail/thiscall_wrap.h"
#include "native_hooks.h"

#pragma managed(push, off)

namespace hook {

template<template<class, class, int> class HookCls, class Derived, class Module>
struct ThiscallWrap : HookCls<Derived, Module, 0>, detail::ThiscallWrapMixin<Derived>
{
    using ThiscallWrapMixin::get_original;
    using ThiscallWrapMixin::hook_fn;
};

} // namespace hook

#pragma managed(pop)
