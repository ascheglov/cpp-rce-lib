
```
template<int BASE>
struct VA
```

Wraps virtual address `BASE` to structure `VA`, which satisfies [Module class requirements](ModuleClassReq.md).

### Example ###
```
struct some_hook : hook::SpliceCodeHook<some_hook, hook::VA<0x401000>>
{
    // ...
};
```