Generates code for hooking via inline patching

```
template<
    class Derived,
    class Module,
    int rva = 0
    >
struct SpliceCodeHook :
    detail::HookBase<Derived>
```

## Template parameters ##

  * `Derived` - most derived class, must provide static `hook` function.
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