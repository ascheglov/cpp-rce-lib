/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * RWX memory allocation
 */
#pragma once

#include <intrin.h>
#include <Windows.h> // TODO: remove it

#pragma managed(push, off)

namespace hook {
namespace detail {

struct global_rwx_memory_pool
{
    typedef BYTE MemBlock[24];
    // In worst case there will be 4 bytes of instructions, followed by 15-bytes instruction
    // also we have to write "jmp rel32" at end of copied code,
    // so we have 4 + 15 + 5 = 24 bytes max.

    static const auto NBLOCKS = 1024u;
    static void* alloc_pool()
    {
        if(auto mem = VirtualAlloc(NULL, sizeof(MemBlock) * NBLOCKS, MEM_COMMIT, PAGE_EXECUTE_READWRITE))
            return mem;
        __debugbreak(); __assume(0); // see crash dump :)
    }

    static BYTE* get_rwx_mem()
    {
        static auto const first = (MemBlock*)alloc_pool();
        static volatile long current = 0;
        auto idx = _InterlockedIncrement(&current) - 1;
        if(idx < NBLOCKS)
            return first[idx];
        __debugbreak(); __assume(0); // see crash dump :)
    }
};

} // namespace detail
} // namespace hook

#pragma managed(pop)
