#pragma once

#include <variant>
#include <cassert>

namespace tower120::utils{

    template<class Base, class Variant>
    class variant_w_base{
        using Self = variant_w_base<Base, Variant>;

        template<class Visitor, class Base_, class Variant_>
        friend constexpr decltype(auto) std::visit(Visitor&&, variant_w_base<Base_, Variant_>&);

        template<class T, class Base_, class Variant_>
        friend constexpr decltype(auto) std::get(variant_w_base<Base_, Variant_>&);

        template<std::size_t I, class Base_, class Variant_>
        friend constexpr decltype(auto) std::get(variant_w_base<Base_, Variant_>&);


        Base* m_base;
        Variant m_variant;

        void update_base(){
            m_base = std::visit([](auto&& arg) -> Base* {
                using Arg = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Arg, std::monostate>){
                    return nullptr;
                }

                return static_cast<Base*>(&arg);
            }, m_variant);
        }

        template<class T>
        using is_not_self = std::enable_if_t< !std::is_same_v< std::decay_t<T>, Self> >;

    public:
        variant_w_base(){
            update_base();
        }
        variant_w_base(const variant_w_base& other)
            : m_variant(other.m_variant)
        {
            update_base();
        }
        variant_w_base(variant_w_base&& other)
            : m_variant(std::move(other.m_variant))
        {
            update_base();
        }
        template<class T, typename = is_not_self<T>>
        variant_w_base(T&& value)
            : m_variant(std::forward<T>(value))
        {
            update_base();
        }
        variant_w_base(const Variant& var)
            : m_variant(var)
        {
            update_base();
        }
        variant_w_base(Variant&& var)
            : m_variant(std::move(var))
        {
            update_base();
        }


        variant_w_base& operator=(const variant_w_base& other){
            m_variant = other.m_variant;
            update_base();
            return *this;
        }
        variant_w_base& operator=(variant_w_base&& other){
            m_variant = std::move(other.m_variant);
            update_base();
            return *this;
        }

        template<class T, typename = is_not_self<T>>
        variant_w_base& operator=(T&& value){
            m_variant = std::forward<T>(value);
            update_base();
            return *this;
        }
        variant_w_base& operator=(const Variant& var){
            m_variant = var;
            update_base();
            return *this;
        }
        variant_w_base& operator=(Variant&& var){
            m_variant = std::move(var);
            update_base();
            return *this;
        }


        constexpr bool operator==(const variant_w_base& other) const {
            return m_variant == other.m_variant;
        }
        constexpr bool operator!=(const variant_w_base& other) const {
            return m_variant != other.m_variant;
        }


        Base* base(){
            return m_base;
        }
        const Base* base() const {
            return m_base;
        }

        const Variant& variant() const{
            return m_variant;
        }
    private:
        // hide, to keep variant type change tracked.
        Variant& variant(){
            return m_variant;
        }
    };


}

namespace std{

    template<class Visitor, class Base, class Variant>
    constexpr decltype(auto) visit(Visitor&& vis, tower120::utils::variant_w_base<Base, Variant>& var){
        return std::visit(std::forward<Visitor>(vis), var.variant());
    }

    template<class T, class Base, class Variant>
    constexpr decltype(auto) get(tower120::utils::variant_w_base<Base, Variant>& var){
        if constexpr (std::is_same_v<T, Base>){
            if (var.base() == nullptr){
                throw std::bad_variant_access();
            }
            //assert(var.base() != nullptr);
            return *var.base();
        } else {
            return std::get<T>(var.variant());
        }
    }

    template<std::size_t I, class Base, class Variant>
    constexpr decltype(auto) get(tower120::utils::variant_w_base<Base, Variant>& var){
        return std::get<I>(var.variant());
    }

    // TODO: get_if, swap
}