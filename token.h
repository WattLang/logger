#pragma once


#include <cstdint>
#include <string>


namespace ws::token {
    using type_t = std::uint16_t;




    struct Token {
        type_t type;
        std::uint32_t line;
        std::uint32_t column;
        std::string content;



        Token() = default;


        Token(
            type_t type_,
            std::uint32_t line_,
            std::uint32_t column_,
            const std::string& content_
        ):
            type(type_),
            line(line_),
            column(column_),
            content(content_)
        {

        }
    };




    inline std::ostream& operator<<(std::ostream& os, const Token& tok) {
        const auto& [type, line, column, content] = tok;

        os << "[" << type << ", (" << line << ", " << column << "), \"" << content << "\"]";

        return os;
    }

}