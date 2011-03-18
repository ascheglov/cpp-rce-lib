/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 *
 * Patching functions
 */
#pragma once

#pragma managed(push, off)

#include "detail/types.h"
#include "detail/instructions.h"
#include "detail/splicing.h"

namespace hook {

inline void splice(void* addr, const void* hookFn, void* bufSaved)
{
    detail::splice((detail::BYTE*)addr, hookFn, (detail::BYTE*)bufSaved);
}

inline const void* redirect_call_or_jmp(void* addr, const void* target)
{
    return detail::redirect_jmp((detail::BYTE*)addr, target);
}

inline void write_jmp(void* addr, const void* target)
{
    return detail::write_jmp((detail::BYTE*)addr, target);
}

} // namespace hook

#pragma managed(pop)
