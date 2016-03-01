# C++/CLI compatibility issues #

## `__thiscall` functions hooking ##

When project is compiled with **/clr**, it cannot contain `__fastcall` functions. Use [ThiscallWrap](ThiscallWrap.md) to avoid this when hooking `__thiscall` functions. It will generate code to convert `__thiscall` to `__stdcall`, so `hook()` function must be `__stdcall` (library will check it in compile-time).

```
struct Cls__Method_hook : hook::ThiscallWrap<hook::SpliceCodeHook, Cls__Method_hook, hook::VA<0x410000>>
{
    static int __stdcall hook(Cls* this1, int arg1)
    {
        return get_original(hook)(this1, arg1);
    }
};
```

## Compiler warning C4793 ##

Wrap functions with `__declspec(naked)` and functions calling them with
`#pragma managed(push, off)` / `#pragma managed(pop)` pair. It prevents the warning about functions compiled as native.