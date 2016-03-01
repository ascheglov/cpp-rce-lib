## Introduction ##

C++ RCE Library allows you to hook code by different ways. In future it will contain some helper code to ease debugging, etc.

### Basic Example ###

```
#include <rce/hook.h>

struct Main_hook : hook::SpliceCodeHook<Main_hook, hook::VA<0x401000>>
{
    int __cdecl hook(int /*arg1*/, void* arg2)
    {
        return get_original(hook)(1, arg2);
    }
};

int WINAPI DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*reserved*/)
{
    if(reason == DLL_PROCESS_ATTACH)
    {
        Main_hook::install();
    }
    return 1;
}
```

## Technical information ##

### Installation ###
It's header-only library. Just copy it somewhere and add its path to compiler's include directories.

### Supported compilers, languages ###
Library will work only with Microsoft Visual C++ 2010.

Currently only supported platform is Win32.

Library works with both C++0x and C++\CLI languages.

### Dependencies ###
Library itself doesn't depends on any third-party libraries.

Test project depends on Boost.Test and Boost.Assign libraries.