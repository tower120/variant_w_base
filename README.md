# variant with base access
c++ variant with direct base class access.


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