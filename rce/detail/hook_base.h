/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 *
 * Base class for all hook classes
 */
#pragma once

#pragma managed(push, off)

namespace hook {
namespace detail {

template<typename Derived>
struct HookBase
{
    typedef Derived derived_t;

    typedef unsigned long /* unspecified-type */ call_addr_t;
    static call_addr_t callAddr;

    template<typename T>
    static T get_original(T)
    {
        return (T)callAddr;
    }

    static void set_installed(bool state)
    {
        state ? derived_t::install() : derived_t::uninstall();
    }

    static void* hook_fn()
    {
        return (void*)derived_t::hook;
    }
};

template<typename Derived>
typename HookBase<Derived>::call_addr_t HookBase<Derived>::callAddr;

} // namespace detail
} // namespace hook

#pragma managed(pop)
