#pragma once


#include <initializer_list>
#include <array>
#include <vector>
#include <utility>


namespace ws::structure {
    template <typename T, size_t S>
    struct Pool {
        using arr_t = std::array<T, S>;

        using value_type = T;
        using size_type = typename arr_t::size_type;

        using reference = T&;
        using const_reference = const T&;

        using pointer = T*;
        using const_pointer = const T*;

        using iterator = typename arr_t::iterator;
        using const_iterator = typename arr_t::const_iterator;

        using reverse_iterator = typename arr_t::reverse_iterator;
        using const_reverse_iterator = typename arr_t::const_reverse_iterator;







        arr_t elements;
        size_type next = 0;




        constexpr Pool() = default;

        constexpr Pool(const std::array<T, S>& init):
            elements(init),
            next(S)
        {

        }

        Pool(const std::initializer_list<T>& init) {
            for (const auto& x: init)
                emplace(x);
        }







        // Const Iterators
        constexpr auto begin() const noexcept {
            return elements.begin();
        }

        constexpr auto end() const noexcept {
            return elements.begin() + next;
        }

        constexpr auto rbegin() const noexcept {
            return elements.rbegin();
        }

        constexpr auto rend() const noexcept {
            return elements.rend() + next;
        }




        // Iterators
        constexpr auto begin() noexcept {
            return elements.begin();
        }

        constexpr auto end() noexcept {
            return elements.begin() + next;
        }

        constexpr auto rbegin() noexcept {
            return elements.rbegin();
        }

        constexpr auto rend() noexcept {
            return elements.rend() + next;
        }







        // Element access
        constexpr reference at(size_type i) {
            if (i > (next - 1))
                throw std::out_of_range("element access outside of range.");

            return elements.at(i);
        }

        constexpr const_reference at(size_type i) const {
            if (i > (next - 1))
                throw std::out_of_range("element access outside of range.");

            return elements.at(i);
        }

        constexpr reference operator[](size_type i) {
            return elements[i];
        }

        constexpr const_reference operator[](size_type i) const {
            return elements[i];
        }









        // Back & front
        constexpr reference front() {
            return elements.front();
        }

        constexpr const_reference front() const {
            return elements.front();
        }

        constexpr reference back() {
            return *(elements.begin() + (next - 1));
        }

        constexpr const_reference back() const {
            return *(elements.begin() + (next - 1));
        }








        // Pointers
        constexpr pointer data() noexcept {
            return elements.data();
        }

        constexpr const_pointer data() const noexcept {
            return elements.data();
        }







        // Capacity
        constexpr bool empty() const noexcept {
            return elements.empty();
        }

        constexpr size_type size() const noexcept {
            return (next - 1);
        }

        constexpr size_type max_size() const noexcept {
            return elements.max_size();
        }





        // Modifiers
        constexpr void clear() noexcept {
            next = 0;
        }






        // Operations
        constexpr void fill(const T& value) {
            std::fill(elements.begin(), elements.end(), value);
            next = S;
        }



        constexpr auto push(const T& value) {
            return elements.at(next++) = value;
        }


        constexpr auto push(T&& value) {
            return elements.at(next++) = std::move(value);
        }



        template <typename... Ts>
        constexpr auto emplace(Ts&&... args) {
            return elements.at(next++) = T{std::forward<Ts>(args)...};
        }
    };
}