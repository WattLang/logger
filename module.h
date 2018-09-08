#ifndef WS_MODULE_H
#define WS_MODULE_H


#include <cstdint>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <array>
#include <utility>
#include <random>


#include "rang/include/rang.hpp"


namespace ws::module {
    // Receive data in chunks.
    // Callback = (const std::string& buffer, int chunk_id, bool eof);
    inline void receive(
        uintmax_t buffer_size,
        std::function<void(const std::string&, int, bool)> callback
    ) {
        std::string buffer(buffer_size, '\0');
        int packet_id = 0;


        while (std::cin.read(&buffer[0], buffer_size)) {
            callback(buffer, packet_id, false);
            ++packet_id;
        }


        buffer.erase(buffer.begin() + std::cin.gcount(), buffer.end());
        callback(buffer, packet_id, true);
    }



    // Accumulate all data and return a single string.
    inline std::string receive_all(uintmax_t buffer_size) {
        std::string accumulator;


        receive(
            buffer_size,

            [&] (const std::string& buffer, int, bool) {
                accumulator += buffer;
            }
        );


        return accumulator;
    }






    // Colours.
    #define NEW_COLOUR(name, val) constexpr auto name = val;


    namespace colour {
        namespace fg {
            NEW_COLOUR(black,   rang::fg::black);
            NEW_COLOUR(red,     rang::fg::red);
            NEW_COLOUR(green,   rang::fg::green);
            NEW_COLOUR(yellow,  rang::fg::yellow);
            NEW_COLOUR(blue,    rang::fg::blue);
            NEW_COLOUR(magenta, rang::fg::magenta);
            NEW_COLOUR(cyan,    rang::fg::cyan);
            NEW_COLOUR(grey,    rang::fg::gray);

            namespace bright {
                NEW_COLOUR(black,   rang::fgB::black);
                NEW_COLOUR(red,     rang::fgB::red);
                NEW_COLOUR(green,   rang::fgB::green);
                NEW_COLOUR(yellow,  rang::fgB::yellow);
                NEW_COLOUR(blue,    rang::fgB::blue);
                NEW_COLOUR(magenta, rang::fgB::magenta);
                NEW_COLOUR(cyan,    rang::fgB::cyan);
                NEW_COLOUR(grey,    rang::fgB::gray);
            }
        }


        namespace bg {
            NEW_COLOUR(black,   rang::bg::black);
            NEW_COLOUR(red,     rang::bg::red);
            NEW_COLOUR(green,   rang::bg::green);
            NEW_COLOUR(yellow,  rang::bg::yellow);
            NEW_COLOUR(blue,    rang::bg::blue);
            NEW_COLOUR(magenta, rang::bg::magenta);
            NEW_COLOUR(cyan,    rang::bg::cyan);
            NEW_COLOUR(grey,    rang::bg::gray);

            namespace bright {
                NEW_COLOUR(black,   rang::bgB::black);
                NEW_COLOUR(red,     rang::bgB::red);
                NEW_COLOUR(green,   rang::bgB::green);
                NEW_COLOUR(yellow,  rang::bgB::yellow);
                NEW_COLOUR(blue,    rang::bgB::blue);
                NEW_COLOUR(magenta, rang::bgB::magenta);
                NEW_COLOUR(cyan,    rang::bgB::cyan);
                NEW_COLOUR(grey,    rang::bgB::gray);
            }
        }


        NEW_COLOUR(notice, fg::bright::blue);
        NEW_COLOUR(warn,   fg::bright::yellow);
        NEW_COLOUR(error,  fg::bright::red);
    }


    #undef NEW_COLOUR






    // Styles.
    namespace style {
        // These are the most universally supported styles.
        constexpr auto bold = rang::style::bold;
        constexpr auto reverse = rang::style::reversed;
    }



    namespace {
        // Reset style, BG and FG all at once.
        struct Reset {};

        std::ostream& operator<<(std::ostream& os, const Reset& obj) {
            return (os << rang::style::reset << rang::fg::reset << rang::bg::reset);
        }


        // Notice style.
        struct Notice {};

        std::ostream& operator<<(std::ostream& os, const Notice& obj) {
            return (os << style::bold << colour::notice);
        }


        // Warn style.
        struct Warn {};

        std::ostream& operator<<(std::ostream& os, const Warn& obj) {
            return (os << style::bold << colour::warn);
        }


        // Error style.
        struct Error {};

        std::ostream& operator<<(std::ostream& os, const Error& obj) {
            return (os << style::bold << colour::error);
        }
    }




    // Custom styles
    namespace style {
        inline Reset reset;
        inline Notice notice;
        inline Warn warn;
        inline Error error;
    }



    // Output symbols
    namespace {
        constexpr auto SYMBOL_NOTICE = "[-] ";
        constexpr auto SYMBOL_WARN   = "[*] ";
        constexpr auto SYMBOL_ERROR  = "[!] ";
    }



    // Streams.
    inline std::ostream& piper = std::cout;
    inline std::ostream& printer = std::cerr;









    // Logging and IO.
    template<typename... Ts>
    inline std::ostream& print(Ts&&... args) {
        return (
            ws::module::printer <<
                style::reset <<
                ... <<
                std::forward<Ts&&>(args)
        ) << style::reset;
    }



    template<typename... Ts>
    inline std::ostream& pipe(Ts&&... args) {
        return (ws::module::piper << ... << std::forward<Ts&&>(args));
    }






    // Logging and IO...
    template<typename... Ts>
    inline std::ostream& println(Ts&&... args) {
        return ws::module::print(std::forward<Ts&&>(args)...) << '\n';
    }



    template<typename... Ts>
    inline std::ostream& pipeln(Ts&&... args) {
        return ws::module::pipe(std::forward<Ts&&>(args)...) << '\n';
    }






    // Logging with colour.
    template<typename... Ts>
    inline std::ostream& notice(Ts&&... args) {
        ws::module::print(style::notice, SYMBOL_NOTICE);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& warn(Ts&&... args) {
        ws::module::print(style::warn, SYMBOL_WARN);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& error(Ts&&... args) {
        ws::module::print(style::error, SYMBOL_ERROR);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    // Print lines too.
    template<typename... Ts>
    inline std::ostream& noticeln(Ts&&... args) {
        return ws::module::notice(std::forward<Ts&&>(args)..., '\n');
    }



    template<typename... Ts>
    inline std::ostream& warnln(Ts&&... args) {
        return ws::module::warn(std::forward<Ts&&>(args)..., '\n');
    }



    template<typename... Ts>
    inline std::ostream& errorln(Ts&&... args) {
        return ws::module::error(std::forward<Ts&&>(args)..., '\n');
    }









    // Easter eggs
    namespace {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0, 5);


        using colour_t = decltype(colour::fg::bright::black);


        colour_t random_colour() {
            return std::array<colour_t, 6>{
                colour::fg::bright::red,
                colour::fg::bright::green,
                colour::fg::bright::yellow,
                colour::fg::bright::blue,
                colour::fg::bright::magenta,
                colour::fg::bright::cyan
            }.at(uni(rng));
        }
    }


    template <typename T>
    inline std::ostream& rainbow(T&& arg) {
        return ws::module::print(random_colour(), arg);
    }


    template <typename T1, typename T2, typename... Ts>
    inline std::ostream& rainbow(T1&& arg1, T2&& arg2, Ts&&... args) {
        ws::module::rainbow(arg1);
        return ws::module::rainbow(arg2, std::forward<Ts&&>(args)...);
    }


    template <typename... Ts>
    inline std::ostream& rainbowln(Ts&&... args) {
        return ws::module::rainbow(std::forward<Ts&&>(args)..., '\n');
    }

}


#endif
