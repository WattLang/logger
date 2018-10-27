#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace ws::token {


    using type_t = std::uint16_t;


    struct Token {
        type_t type;
        std::uint32_t line;
        std::uint32_t column;
        std::string content;
    };

    /*
        8 bytes: content's size
        <size> bytes: content
        4 bytes: type
        8 bytes: line
        8 bytes: column
    */
    inline std::ostream& serialize(std::ostream& os, Token const& token) {
        os.write(reinterpret_cast<const char*>(&token),               offsetof(Token, content));
        std::size_t size = token.content.size();
        os.write(reinterpret_cast<const char*>(&size               ), sizeof(size            ));
        os.write(                               token.content.data(),        size             );
        return os;
    }

    inline std::istream& deserialize(std::istream& is, Token& token) {
        auto read = [] (std::istream& is, auto* prop, std::streamsize size) {
            if (is.readsome(reinterpret_cast<char*>(prop), size) != size) {
                is.setstate(std::ios::failbit);
                return false;
            }
            return true;
        };
        is.clear(); // remove flags

        if (!read(is, &token, offsetof(Token, content)))
            return is;

        std::size_t size;
        if (!read(is, &size, sizeof(size)))
            return is;

        token.content.resize(size);
        read(is, token.content.data(), size);

        return is;
    }



}