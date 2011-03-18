/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010-2011 Abyx. MIT License.
 */
#pragma once
#include <intsafe.h> // TODO: remove it
#include <Windows.h> // TODO: remove it

namespace rce {

struct MakeWriteable
{
    const void* address;
    size_t size;
    DWORD oldProtect;

    MakeWriteable() : address(nullptr) {}

    MakeWriteable(const void* address, size_t size)
        : address(address), size(size)
    {
        SetProtect();
    }

    template<typename T>
    MakeWriteable(const T* address, size_t size = sizeof(T))
        : address(address), size(size)
    {
        SetProtect();
    }

    void SetProtect(DWORD value = PAGE_EXECUTE_READWRITE)
    {
        VirtualProtect((void*)address, size, value, &oldProtect);
    }

    ~MakeWriteable()
    {
        SetProtect(oldProtect);
    }

    MakeWriteable(const MakeWriteable&&) {} // movable
private:
    MakeWriteable(const MakeWriteable&); // noncopyable
    const MakeWriteable& operator=(const MakeWriteable&); // noncopyable
};

struct MemPtr
{
    typedef unsigned char BYTE;
    typedef unsigned short WORD;
    typedef unsigned long DWORD;
    typedef unsigned long long QWORD;

    void* address;

    MemPtr() : address(nullptr) {}
    MemPtr(void* address) : address(address) {}

    void* va(ptrdiff_t offset = 0) const { return ((char*)address) + offset; }
    MemPtr ptr(ptrdiff_t offset = 0) const { return MemPtr(va(offset)); }

    BYTE* byte_va(ptrdiff_t offset = 0) const { return (BYTE*)va(offset); }
    WORD* word_va(ptrdiff_t offset = 0) const { return (WORD*)va(offset); }
    DWORD* dword_va(ptrdiff_t offset = 0) const { return (DWORD*)va(offset); }
    QWORD* qword_va(ptrdiff_t offset = 0) const { return (QWORD*)va(offset); }
    template<typename T> T* value_va(ptrdiff_t offset = 0) const { return (T*)va(offset); }

    BYTE& byte(ptrdiff_t offset = 0) const { return *byte_va(offset); }
    WORD& word(ptrdiff_t offset = 0) const { return *word_va(offset); }
    DWORD& dword(ptrdiff_t offset = 0) const { return *dword_va(offset); }
    QWORD& qword(ptrdiff_t offset = 0) const { return *qword_va(offset); }
    template<typename T> T& value(ptrdiff_t offset = 0) const { return *value_va<T>(offset); }

    MakeWriteable enableWrite(size_t size) const {return MakeWriteable(address, size); }
};

} // namespace rce
