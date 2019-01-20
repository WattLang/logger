#pragma once


#include <iostream>
#include <cstdint>
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