Adds `__thiscall` to `__stdcall` conversion to `hook()` and `get_original()` functions.

```
template<
    template<class, class, int> class HookCls,
    class Derived,
    class Module
    >
struct ThiscallWrap : HookCls<Derived, Module, 0>, ...
```

## Template parameters ##

  * `HookCls` - hook class, for example SpliceCodeHook.
  * `Derived` - most derived class, must provide static `hook` function.
  * `Module`- class compliant to [Module class requirements](ModuleClassReq.md).

## Example ##

```
struct thiscallfunc_hook : hook::ThiscallWrap<hook::SpliceCodeHook, thiscallfunc_hook, hook::VA<0x401000>>
{
    static int __stdcall hook(void* this1)
    {
        return get_original(hook)(this1);
    }
};
```