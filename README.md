# variant with base class access

[Live](http://coliru.stacked-crooked.com/a/a93c75c3217d5657)

`std::variant` does not allow you to access base class directly:
```c++
    struct Base{
        int pos;
    };
    struct A : Base{
        int a = 10;
    };
    struct B : Base{
        int b = 20;
    };

    std::variant<A,B> var;
    Base& base = std::get<Base>(var);    // can't do this

    // can do this, but this will cost you.
    Base& base = std::visit([](Base& base) -> Base& {
        return base;
    }, var);
```

With `variant_w_base` you can access base class almost as no-op.

```c++
    variant_w_base<Base, std::variant<A,B>> var;
    Base& base = *var.base();
    Base& base = std::get<Base>(var);
```

Interface similliar to `std::variant`, so it should be drop-in replacement.

# Performance

performance-wise `variant_w_base`'s base class access 5-10 times faster than with `std::visit`. See `test.h`.

[More](https://tower120.github.io/2018/05/18/variant_with_base.html)