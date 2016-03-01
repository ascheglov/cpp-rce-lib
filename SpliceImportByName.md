Generates code for hooking via inline patching.

```
template<
    class Derived,
    class Module,
    int rva = 0
    >
struct SpliceImportByName :
    detail::HookBase<Derived>
```

## Template parameters ##

  * `Derived` - most derived class, must provide static `hook` and `name` functions.
  * `Module` - class compliant to [Module class requirements](ModuleClassReq.md).
  * `rva` - offset from Module base address.

## Typedefs ##
```
typedef Derived derived_t;
```
Ditto. Inherited from [HookBase](HookBase.md).

## Member function ##

```
static void install()
```
Installs hook.
```
static void uninstall()
```
Uninstalls hook.
```
static void set_installed(bool state)
```
Installs hook if `state` is `true`, otherwise uninstalls hook.
Inherited from [HookBase](HookBase.md).
```
template<typename T>
static T get_original(T);
```
Returns function pointer to original code. Result type is copied from argument type.
Inherited from [HookBase](HookBase.md).

## Member variables ##

```
static unspecified-type callAddr;
```
Address to original code.
Inherited from [HookBase](HookBase.md).

## Example ##

```
struct some_module
{
    static void* function(const char* name)
    {
        return GetProcAddress(GetModuleHandleA("some_module"), name);
    }    
};

struct func_hook : hook::SpliceImportByName<func_hook, some_module>
{
    static const char* name() { return "_func@4"; }
    static int __stdcall hook(int x)
    {
        return get_original(hook)(x);
    }
};
```