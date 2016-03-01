This is the base class for all hook classes.

```
namespace detail {

template<typename Derived>
struct HookBase { ... };

}
```

## Typedefs ##
```
typedef Derived derived_t;
```
Ditto.

## Static data members ##
```
static unspecified_type callAddr;
```
Holds an address to original code. Use it to invoke original code as if it wasn't hooked.

## Static functions ##
```
template<typename T>
static T get_original(T) { return (T)callAddr; }
```
Casts callAddr to parameter type.

```
static void set_installed(bool state);
   { state ? derived_t::install() : derived_t::uninstall(); }
```
Ditto.