# Splicing #

To hook whole function via inline patching use [SpliceCodeHook](SpliceCodeHook.md).
```
struct foo_hook : hook::SpliceCodeHook<foo_hook, hook::VA<0x410000>>
{
    static int __stdcall hook(int arg1)
    {
        return get_original(hook)(arg1);
    }
};
```

To hook `__thiscall` function, using native project (without **/clr**) hook it as if it was `__fastcall`.

```
struct Cls__Method_hook : hook::SpliceCodeHook<Cls__Method_hook, hook::VA<0x410000>>
{
    static int __fastcall hook(Cls* this1, int /*edx*/, int arg1)
    {
        return get_original(hook)(this1, 0, arg1);
    }
};
```

To hook code inside function, You can use the following pattern:

```
struct foo_hook : hook::SpliceCodeHook<foo_hook, hook::VA<0x410000>>
{
    static void __stdcall payload(int arg1)
    {
        // ... some code
    }

    static void hook();
};

__declspec(naked) foo_hook::hook()
{ __asm {
    push eax
    call payload
    jmp dword ptr [callAddr]
} }
```