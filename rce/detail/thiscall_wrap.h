/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 *
 * __thiscall hooks compatible with /clr compiler option
 */
#pragma once

#include <type_traits>

#pragma managed(push, off)

namespace hook {
namespace detail {

// compile-time checker
#pragma warning(push)
#pragma warning(error: 4440)
template<typename F> inline void must_be_stdcall(F*) { typedef F __stdcall* T; }
#pragma warning(pop)

template<class Derived>
struct ThiscallWrapMixin
{
    template<typename T> static T get_original(T) { return (T)(void*)call_wrap; }
    static void* hook_fn() { return (void*)&hook_wrap; }
    static void hook_wrap();
    static void call_wrap();
};

template<class Derived>
__declspec(naked) void ThiscallWrapMixin<Derived>::hook_wrap()
{
    static_assert(!std::is_member_pointer<decltype(&Derived::hook)>::value, "hook() must be static");
    detail::must_be_stdcall(&Derived::hook);
    __asm{
        pop eax // return address
        push ecx // this
        push eax // return address
        jmp Derived::hook
    }
}
template<class Derived>
__declspec(naked) void ThiscallWrapMixin<Derived>::call_wrap()
{__asm{
    pop eax // return address
    pop ecx // this
    push eax // return address
    jmp dword ptr[Derived::callAddr]
}}

} // namespace detail
} // namespace hook

#pragma managed(pop)
