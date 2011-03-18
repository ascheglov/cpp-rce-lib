/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 *
 * Helpers for some assembly instructions
 */
#pragma once

#include "types.h"

#pragma managed(push, off)

namespace hook {
namespace detail {

inline void set_jmp_dest(BYTE* addr, const void* target)
{
    *(int*)(addr + 1) = (BYTE*)target - (addr + 5);
}

inline const BYTE* get_jmp_dest(const BYTE* addr)
{
    return (addr + 5) + *(int*)(addr + 1);
}

inline void write_jmp(BYTE* addr, const void* target)
{
    addr[0] = 0xE9;
    set_jmp_dest(addr, target);
}

inline void write_call(BYTE* addr, const void* target)
{
    addr[0] = 0xE8;
    set_jmp_dest(addr, target);
}

inline const BYTE* redirect_jmp(BYTE* addr, const void* target)
{
    auto dest = get_jmp_dest(addr);
    set_jmp_dest(addr, target);
    return dest;
}

} // namespace detail
} // namespace hook

#pragma managed(pop)
