#pragma once

#include <chrono>

#include "variant_w_base.h"
#include <experimental/random>

namespace samples{
    void simple_usage(){
        using namespace tower120::utils;

        struct Base{
            int pos = 1;
        };
        struct A : Base{
            int a = 10;
        };
        struct B : Base{
            int b = 20;
        };

        variant_w_base<Base, std::variant<std::monostate, A, B>> v = B();

        const auto& cv = v;
        const Base& base = cv;
        std::cout << base.pos << std::endl;

        v.visit([](auto& arg){
            using Arg = std::decay_t< decltype(arg) >;
            if constexpr (std::is_same_v<Arg, A>){
                std::cout << arg.a << std::endl;
            } else if constexpr (std::is_same_v<Arg, B>){
                std::cout << arg.b << std::endl;
            }
        });

        std::cout << v->pos << std::endl;
    }


    void local_vclass_storage(){
        using namespace tower120::utils;

        struct Base{
            virtual int get() = 0;
        };
        struct A : Base{
            virtual int get() override {
                return 1;
            }
        };
        struct B : Base{
            virtual int get() override {
                return 2;
            }
        };

        variant_w_base<Base, std::variant<std::monostate, A, B>> v = B();

        std::cout << v.base()->get() << std::endl;
    }

    void start(){
        simple_usage();
        local_vclass_storage();
    }
}