/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Splicing engine
 */
#pragma once

#include <Windows.h> // TODO: remove it

#include "instructions.h"

#pragma managed(push, off)

namespace hook {
namespace detail {

/*	Catchy32 v1.6 - Length Disassembler Engine 32bit
 *	(c) sars [HI-TECH] 2003
 *
 *	Ported to MS VC++, adapted to __cdecl, returns 0 on error.
 */
__declspec(naked)
inline int __cdecl instruction_length(const void* /*instructionPtr*/)
{__asm{
#define DB __asm _emit
DB 0x8b DB 0x4c DB 0x24 DB 0x04 // fastcall to cdecl
DB 0x60 DB 0x89 DB 0xce DB 0xe8 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x5d DB 0x83 DB 0xed DB 0x8 DB 0x31 DB 0xc9 DB 0x31 DB 0xc0
DB 0x31 DB 0xdb DB 0x99 DB 0xac DB 0x88 DB 0xc1 DB 0x3c DB 0xf DB 0x74 DB 0xf DB 0x66 DB 0x81 DB 0x7e DB 0xff DB 0xcd DB 0x20
DB 0x75 DB 0xa DB 0x46 DB 0xad DB 0xe9 DB 0x1 DB 0x1 DB 0x0 DB 0x0 DB 0xac DB 0xfe DB 0xc4 DB 0xd1 DB 0xe8 DB 0x8a DB 0x84
DB 0x5 DB 0x41 DB 0x1 DB 0x0 DB 0x0 DB 0x72 DB 0x3 DB 0xc1 DB 0xe8 DB 0x4 DB 0x83 DB 0xe0 DB 0xf DB 0x93 DB 0x80 DB 0xfb
DB 0xe DB 0xf DB 0x84 DB 0xf2 DB 0x0 DB 0x0 DB 0x0 DB 0x80 DB 0xfb DB 0xf DB 0x74 DB 0x4b DB 0x9 DB 0xdb DB 0xf DB 0x84
DB 0xd6 DB 0x0 DB 0x0 DB 0x0 DB 0xf DB 0xba DB 0xf3 DB 0x0 DB 0x72 DB 0x5b DB 0xf DB 0xba DB 0xf3 DB 0x1 DB 0xf DB 0x82
DB 0xc0 DB 0x0 DB 0x0 DB 0x0 DB 0xf DB 0xba DB 0xf3 DB 0x2 DB 0xf DB 0x82 DB 0xb5 DB 0x0 DB 0x0 DB 0x0 DB 0x80 DB 0xe3
DB 0xf7 DB 0x80 DB 0xf9 DB 0xa0 DB 0x72 DB 0x13 DB 0x80 DB 0xf9 DB 0xa3 DB 0x77 DB 0xe DB 0xf6 DB 0xc5 DB 0x2 DB 0xf DB 0x85
DB 0x9f DB 0x0 DB 0x0 DB 0x0 DB 0xe9 DB 0x98 DB 0x0 DB 0x0 DB 0x0 DB 0xf6 DB 0xc5 DB 0x1 DB 0xf DB 0x84 DB 0x8f DB 0x0
DB 0x0 DB 0x0 DB 0xe9 DB 0x8c DB 0x0 DB 0x0 DB 0x0 DB 0x80 DB 0xf9 DB 0x66 DB 0x74 DB 0x11 DB 0x80 DB 0xf9 DB 0x67 DB 0xf
DB 0x85 DB 0x69 DB 0xff DB 0xff DB 0xff DB 0x80 DB 0xcd DB 0x2 DB 0xe9 DB 0x61 DB 0xff DB 0xff DB 0xff DB 0x80 DB 0xcd DB 0x1
DB 0xe9 DB 0x59 DB 0xff DB 0xff DB 0xff DB 0xac DB 0x80 DB 0xf9 DB 0xf7 DB 0x74 DB 0x5 DB 0x80 DB 0xf9 DB 0xf6 DB 0x75 DB 0x12
DB 0xa8 DB 0x38 DB 0x75 DB 0xe DB 0xf6 DB 0xc1 DB 0x1 DB 0x74 DB 0x8 DB 0xf6 DB 0xc5 DB 0x1 DB 0x75 DB 0x2 DB 0x46 DB 0x46
DB 0x46 DB 0x46 DB 0x89 DB 0xc2 DB 0x24 DB 0x7 DB 0xf6 DB 0xc2 DB 0xc0 DB 0x74 DB 0x13 DB 0xf DB 0x8a DB 0x5d DB 0xff DB 0xff
DB 0xff DB 0x78 DB 0x32 DB 0xf6 DB 0xc5 DB 0x2 DB 0x75 DB 0x3c DB 0x3c DB 0x4 DB 0x74 DB 0x37 DB 0xeb DB 0x36 DB 0xf6 DB 0xc5
DB 0x2 DB 0x74 DB 0x9 DB 0x3c DB 0x6 DB 0x74 DB 0x2c DB 0xe9 DB 0x42 DB 0xff DB 0xff DB 0xff DB 0x3c DB 0x4 DB 0x75 DB 0xc
DB 0xac DB 0x24 DB 0x7 DB 0x3c DB 0x5 DB 0x74 DB 0x1a DB 0xe9 DB 0x32 DB 0xff DB 0xff DB 0xff DB 0x3c DB 0x5 DB 0x74 DB 0x11
DB 0xe9 DB 0x29 DB 0xff DB 0xff DB 0xff DB 0xf6 DB 0xc5 DB 0x2 DB 0x75 DB 0x9 DB 0x3c DB 0x4 DB 0x74 DB 0x2 DB 0xeb DB 0x1
DB 0x46 DB 0x46 DB 0x46 DB 0x46 DB 0x46 DB 0xe9 DB 0x14 DB 0xff DB 0xff DB 0xff DB 0x2b DB 0x74 DB 0x24 DB 0x18 DB 0x83 DB 0xfe
DB 0xf DB 0x77 DB 0x6 DB 0x89 DB 0x74 DB 0x24 DB 0x1c DB 0xeb DB 0x6 DB 0x31 DB 0xc0 DB 0x89 DB 0x44 DB 0x24 DB 0x1c DB 0x61
DB 0xc3 DB 0x11 DB 0x11 DB 0x28 DB 0x0 DB 0x11 DB 0x11 DB 0x28 DB 0x0 DB 0x11 DB 0x11 DB 0x28 DB 0x0 DB 0x11 DB 0x11 DB 0x28
DB 0x0 DB 0x11 DB 0x11 DB 0x28 DB 0xf0 DB 0x11 DB 0x11 DB 0x28 DB 0xf0 DB 0x11 DB 0x11 DB 0x28 DB 0xf0 DB 0x11 DB 0x11 DB 0x28
DB 0xf0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0
DB 0x0 DB 0x0 DB 0x11 DB 0xff DB 0xff DB 0x89 DB 0x23 DB 0x0 DB 0x0 DB 0x22 DB 0x22 DB 0x22 DB 0x22 DB 0x22 DB 0x22 DB 0x22
DB 0x22 DB 0x39 DB 0x33 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0xc0 DB 0x0
DB 0x0 DB 0x88 DB 0x88 DB 0x0 DB 0x0 DB 0x28 DB 0x0 DB 0x0 DB 0x0 DB 0x22 DB 0x22 DB 0x22 DB 0x22 DB 0x88 DB 0x88 DB 0x88
DB 0x88 DB 0x33 DB 0x40 DB 0x11 DB 0x39 DB 0x60 DB 0x40 DB 0x2 DB 0x0 DB 0x11 DB 0x11 DB 0x22 DB 0x0 DB 0x11 DB 0x11 DB 0x11
DB 0x11 DB 0x22 DB 0x22 DB 0x22 DB 0x22 DB 0x88 DB 0xc2 DB 0x0 DB 0x0 DB 0xf0 DB 0xff DB 0x0 DB 0x11 DB 0x0 DB 0x0 DB 0x0
DB 0x11 DB 0x11 DB 0x11 DB 0xe0 DB 0x0 DB 0x0 DB 0xee DB 0xe1 DB 0x3 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x1e DB 0xee DB 0xee
DB 0xee DB 0x11 DB 0x11 DB 0x1e DB 0x1e DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x0 DB 0x0 DB 0x0 DB 0xee DB 0xee DB 0xee DB 0xee
DB 0xee DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11
DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x33 DB 0x33 DB 0x11 DB 0x10 DB 0x11 DB 0x11 DB 0x11
DB 0x11 DB 0x88 DB 0x88 DB 0x88 DB 0x88 DB 0x88 DB 0x88 DB 0x88 DB 0x88 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11
DB 0x11 DB 0x0 DB 0x1 DB 0x31 DB 0x11 DB 0x0 DB 0x1 DB 0x31 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0xee DB 0x31 DB 0x11
DB 0x11 DB 0x11 DB 0x31 DB 0x33 DB 0x31 DB 0x0 DB 0x0 DB 0x0 DB 0x0 DB 0xe1 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11
DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0xe1 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11 DB 0x11
#undef DB
}}

inline void copy_5bytes_to(BYTE* src, BYTE* dst)
{
    // TODO: handle jxx rel8, jxx rel32, w/prefixes...
    for(auto end = src + 5; src < end; )
    {
        auto cb = instruction_length(src);
        switch(src[0])
        {
        case 0xE9: // jmp rel32, 5 bytes
            write_jmp(dst, get_jmp_dest(src));
            return; // don't need to write trailing jmp to src

        case 0xE8: // call rel32, 5 bytes
            write_call(dst, get_jmp_dest(src));
            write_jmp(dst + 5, src + 5);
            return;

        case 0x0F: // jxx near
        case 0xEB: // jmp rel8, 2 bytes
            __debugbreak(); // not supported yet

        default:
            memcpy(dst, src, cb);
            src += cb;
            dst += cb;
        }
    }
    write_jmp(dst, src);
}

inline void splice(BYTE* addr, const void* hookFn, BYTE* bufSaved)
{
    copy_5bytes_to(addr, bufSaved);
    write_jmp(addr, hookFn);
}

} // namespace detail
} // namespace hook

#pragma managed(pop)
