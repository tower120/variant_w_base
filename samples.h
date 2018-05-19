#pragma once

#include <chrono>

#include "variant_w_base.h"
#include <experimental/random>

namespace samples{
    void simple_usage(){
        using namespace tower120::utils;

        struct Base{
            int pos;
        };
        struct A : Base{
            int a = 10;
        };
        struct B : Base{
            int b = 20;
        };

        variant_w_base<Base, std::variant<std::monostate, A, B>> v = B();

        std::visit([](auto& arg){
            using Arg = std::decay_t< decltype(arg) >;
            if constexpr (std::is_same_v<Arg, A>){
                std::cout << arg.a;
            } else if constexpr (std::is_same_v<Arg, B>){
                std::cout << arg.b;
            }
        }, v);
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

        std::cout << v.base()->get();
    }

    void start(){
        simple_ussage();
        local_vclass_storage();
    }
}