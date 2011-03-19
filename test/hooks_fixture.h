/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 */
#pragma once

#include <Windows.h>

struct HooksFixture
{
    HooksFixture() : funcAddr() {}

    void* funcAddr;
    DWORD oldProtect;

    void enable_write(void* func)
    {
        funcAddr = func;
        VirtualProtect(funcAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    }
    ~HooksFixture()
    {
        if(funcAddr)
            VirtualProtect(funcAddr, 5, oldProtect, &oldProtect);
    }
};

template<void* addr, int offset = 0>
struct local_test_func
{
    static void* ptr(int /*rva*/) { return (char*)addr + offset; }
};
