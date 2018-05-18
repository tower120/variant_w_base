#pragma once

#include <chrono>

#include "variant_w_base.h"
#include <experimental/random>

namespace test_variant_w_base{

    template<class TimeUnit = std::chrono::milliseconds, class Closure>
    static auto measure(Closure&& closure){
        using namespace std::chrono;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        closure();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        return duration_cast<TimeUnit>(t2-t1).count();
    }

    template<class TimeUnit = std::chrono::milliseconds, class Closure>
    static auto benchmark(unsigned int times, Closure&& closure){
        std::size_t t = 0;
        for (unsigned int i = 0; i < times; ++i) {
            t += measure<TimeUnit>(std::forward<Closure>(closure));
        }
        return t;
    }


    struct Base{
        int pos;
    };

    struct A : Base{
        int a = 10;
    };
    struct B : Base{
        int b = 20;
    };
    struct C : Base{
        int c = 30;
    };

    using Variant = std::variant<A,B,C>;

    Variant get_random_variant(){
        const int rnd = std::experimental::randint(0, 2);
        switch(rnd){
            case 0: return {A()};
            case 1: return {B()};
            case 2: return {C()};
            default: throw std::runtime_error("");
        }
    }


    void performance_test_variant(int count = 100'000, int repeats = 100){
        using Element = Variant;
        std::vector<Element> vec;
        for(int i=0;i<count;++i){
            Element& element = vec.emplace_back(get_random_variant());
            std::visit([i](Base& base){
                base.pos = i;
            }, element);
        }

        Element *found_element = nullptr;
        auto t = benchmark(repeats, [&]() {
            //const int rnd = std::experimental::randint(0, count);
            const int rnd = count-1;
            for (Element &element: vec) {
                const bool found = std::visit([&](Base &base) {
                    return base.pos == rnd;
                }, element);

                if (found) {
                    found_element = &element;
                    break;
                }
            }
        });

        std::cout << found_element << " " << t << std::endl;
    }


    void performance_test_variant_w_base(int count = 100'000, int repeats = 100){
        using namespace tower120::utils;

        using Element = variant_w_base<Base, Variant>;
        std::vector<Element> vec;
        for(int i=0;i<count;++i){
            Element& element = vec.emplace_back(get_random_variant());
            std::visit([i](Base& base){
                base.pos = i;
            }, element);
        }

        Element *found_element = nullptr;
        auto t = benchmark(repeats, [&]() {
            //const int rnd = std::experimental::randint(0, count);
            const int rnd = count-1;
            for (Element &element: vec) {
                const bool found = element.base()->pos == rnd;

                if (found) {
                    found_element = &element;
                    break;
                }
            }
        });

        std::cout << found_element << " " << t << std::endl;
    }



    void simple_test(){
        using namespace tower120::utils;

        variant_w_base<Base, std::variant<A, B>> v = B();

        std::visit([](auto& arg){
            using Arg = std::decay_t< decltype(arg) >;
            if constexpr (std::is_same_v<Arg, A>){
                std::cout << arg.a;
            } else if constexpr (std::is_same_v<Arg, B>){
                std::cout << arg.b;
            }
        }, v);
    }

    void test(){
        const int count   = 100'000;
        const int repeats = 100;
        performance_test_variant(count, repeats);
        performance_test_variant_w_base(count, repeats);
    }
}