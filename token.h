#pragma once


#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <string_view>


namespace ws::token {
    using Type = std::uint16_t;

    struct Position {
        using size_type = std::uint32_t;


        size_type line = 1, column = 1;

        void nextln(size_type n = 1) {
            column = 1;
            line += n;
        }

        void next(size_type n = 1) {
            column += n;
        }
    };




    struct Token {
        Type type;
        Position pos;
        std::string_view content;



        Token() = default;


        Token(
            Type type_,
            Position pos_,
            std::string_view content_
        ):
            type(type_),
            pos(pos_),
            content(content_)
        {

        }



        Token(
            Type type_,
            Position pos_
        ):
            type(type_),
            pos(pos_)
        {

        }



        Token(
            Type type_,
            std::string_view content_
        ): type(type_), content(content_) {}



        Token(Type type_): type(type_) {}
    };











    struct Group {
        using vec_t = std::vector<Token>;

        using size_type = vec_t::size_type;

        using iterator = vec_t::iterator;
        using const_iterator = vec_t::const_iterator;

        using reverse_iterator = vec_t::reverse_iterator;
        using const_reverse_iterator = vec_t::const_reverse_iterator;






        std::string source;
        vec_t tokens;


        Group() = default;

        Group(const std::string& str):
            source(str)
        {
            tokens.reserve(100);
        }

        Group(const std::string& str, float ratio):
            source(str)
        {
            auto buffer_size = str.size() / ratio;
            tokens.reserve(buffer_size);
        }





        // Iterators
        auto begin() const noexcept {
            return tokens.begin();
        }

        auto end() const noexcept {
            return tokens.end();
        }

        auto rbegin() const noexcept {
            return tokens.rbegin();
        }

        auto rend() const noexcept {
            return tokens.rend();
        }










        // Element access.
        Token& at(size_type i) {
            return tokens.at(i);
        }

        const Token& at(size_type i) const {
            return tokens.at(i);
        }


        Token& operator[](size_type i) {
            return tokens[i];
        }

        const Token& operator[](size_type i) const {
            return tokens[i];
        }


        Token& front() {
            return tokens.front();
        }

        const Token& front() const {
            return tokens.front();
        }



        Token& back() {
            return tokens.back();
        }

        const Token& back() const {
            return tokens.back();
        }



        Token* data() noexcept {
            return tokens.data();
        }

        const Token* data() const noexcept {
            return tokens.data();
        }









        // Capacity.

        bool empty() const noexcept {
            return tokens.empty();
        }



        size_type size() const noexcept {
            return tokens.size();
        }




        size_type max_size() const noexcept {
            return tokens.max_size();
        }



        void reserve(size_type cap) {
            tokens.reserve(cap);
        }

        size_type capacity() const noexcept {
            return tokens.capacity();
        }



        void shrink_to_fit() {
            tokens.shrink_to_fit();
        }






        // Modifiers.
        void clear() noexcept {
            tokens.clear();
        }





        iterator erase(iterator pos) {
            return tokens.erase(pos);
        }

        iterator erase(const_iterator pos) {
            return tokens.erase(pos);
        }

        iterator erase(iterator first, iterator last) {
            return tokens.erase(first, last);
        }

        iterator erase(const_iterator first, const_iterator last) {
            return tokens.erase(first, last);
        }




        template <typename... Ts>
        const Token& emplace(Ts&&... args) {
            return tokens.emplace_back(std::forward<Ts>(args)...);
        }




        void resize(size_type count, Token value = Token()) {
            tokens.resize(count, value);
        }

        void resize(size_type count) {
            tokens.resize(count);
        }

        void resize(size_type count, const Token& value) {
            tokens.resize(count, value);
        }

    };







    inline std::ostream& operator<<(std::ostream& os, const Position& tok) {
        const auto& [line, column] = tok;

        os << "(" << line << ", " << column << ")";

        return os;
    }





    inline std::ostream& operator<<(std::ostream& os, const Token& tok) {
        const auto& [type, pos, content] = tok;

        os << "[" << type << ", " << pos << ", \"" << content << "\"]";

        return os;
    }

}