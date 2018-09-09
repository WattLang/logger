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
#include <sstream>
#include <iterator>


#include "rang/include/rang.hpp"


namespace ws::module {
    // Macros
    #define NEW_COLOUR(name, val) constexpr auto name = val;



    // Utility functions
    inline std::string repeat(char c, unsigned n) {
        return std::string(n, c);
    }


    // std::string constructor does not allow repeating a string so
    // this function implements it.
    inline std::string repeat(const std::string& c, unsigned n) {
        std::string out = c;

        for (n--; n > 0; n--) out += c;

        return out;
    }


    inline std::string spaces(unsigned n) {
        return ws::module::repeat(' ', n);
    }


    inline std::string tabs(unsigned n) {
        return ws::module::repeat('\t', n);
    }


    inline std::string lines(unsigned n) {
        return ws::module::repeat('\n', n);
    }



    // Receive data in chunks.
    // Callback = (const std::string& buffer, int chunk_id, bool eof);
    inline void receive(
        std::streamsize buffer_size,
        std::function<void(const std::string&, int, bool)> callback
    ) {
        std::string buffer(static_cast<uintmax_t>(buffer_size), '\0');
        int packet_id = 0;


        while (std::cin.read(&buffer[0], buffer_size)) {
            callback(buffer, packet_id, false);
            ++packet_id;
        }


        buffer.erase(buffer.begin() + std::cin.gcount(), buffer.end());
        callback(buffer, packet_id, true);
    }



    // Accumulate all data and return a single string.
    inline std::string receive_all(std::streamsize buffer_size) {
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
    namespace colour {
        namespace fg {
            NEW_COLOUR(black,   rang::fg::black)
            NEW_COLOUR(red,     rang::fg::red)
            NEW_COLOUR(green,   rang::fg::green)
            NEW_COLOUR(yellow,  rang::fg::yellow)
            NEW_COLOUR(blue,    rang::fg::blue)
            NEW_COLOUR(magenta, rang::fg::magenta)
            NEW_COLOUR(cyan,    rang::fg::cyan)
            NEW_COLOUR(grey,    rang::fg::gray)
            NEW_COLOUR(normal,  rang::fg::reset)

            namespace bright {
                NEW_COLOUR(black,   rang::fgB::black)
                NEW_COLOUR(red,     rang::fgB::red)
                NEW_COLOUR(green,   rang::fgB::green)
                NEW_COLOUR(yellow,  rang::fgB::yellow)
                NEW_COLOUR(blue,    rang::fgB::blue)
                NEW_COLOUR(magenta, rang::fgB::magenta)
                NEW_COLOUR(cyan,    rang::fgB::cyan)
                NEW_COLOUR(grey,    rang::fgB::gray)
                NEW_COLOUR(normal,  rang::fg::reset)
            }
        }


        namespace bg {
            NEW_COLOUR(black,   rang::bg::black)
            NEW_COLOUR(red,     rang::bg::red)
            NEW_COLOUR(green,   rang::bg::green)
            NEW_COLOUR(yellow,  rang::bg::yellow)
            NEW_COLOUR(blue,    rang::bg::blue)
            NEW_COLOUR(magenta, rang::bg::magenta)
            NEW_COLOUR(cyan,    rang::bg::cyan)
            NEW_COLOUR(grey,    rang::bg::gray)
            NEW_COLOUR(normal,  rang::bg::reset)

            namespace bright {
                NEW_COLOUR(black,   rang::bgB::black)
                NEW_COLOUR(red,     rang::bgB::red)
                NEW_COLOUR(green,   rang::bgB::green)
                NEW_COLOUR(yellow,  rang::bgB::yellow)
                NEW_COLOUR(blue,    rang::bgB::blue)
                NEW_COLOUR(magenta, rang::bgB::magenta)
                NEW_COLOUR(cyan,    rang::bgB::cyan)
                NEW_COLOUR(grey,    rang::bgB::gray)
                NEW_COLOUR(normal,  rang::bg::reset)
            }
        }


        NEW_COLOUR(notice, fg::bright::blue)
        NEW_COLOUR(warn,   fg::bright::yellow)
        NEW_COLOUR(error,  fg::bright::red)
    }






    // Styles.
    namespace style {
        // These are the most universally supported styles.
        constexpr auto bold = rang::style::bold;
        constexpr auto reverse = rang::style::reversed;
    }



    namespace details {
        // Reset style, BG and FG all at once.
        struct Reset {};

        // Dim colour style.
        struct DimFG {
            decltype(colour::fg::black) colour;
        };

        struct DimBG {
            decltype(colour::bg::black) colour;
        };

        // Notice style, set bold and notice colour.
        struct Notice {};

        // Warn style, set bold and warn colour.
        struct Warn {};

        // Error style, set bold and error colour.
        struct Error {};
    }



    namespace colour {
        namespace fg::dim {
            NEW_COLOUR(black,   details::DimFG{colour::fg::black})
            NEW_COLOUR(red,     details::DimFG{colour::fg::red})
            NEW_COLOUR(green,   details::DimFG{colour::fg::green})
            NEW_COLOUR(yellow,  details::DimFG{colour::fg::yellow})
            NEW_COLOUR(blue,    details::DimFG{colour::fg::blue})
            NEW_COLOUR(magenta, details::DimFG{colour::fg::magenta})
            NEW_COLOUR(cyan,    details::DimFG{colour::fg::cyan})
            NEW_COLOUR(grey,    details::DimFG{colour::fg::grey})
            NEW_COLOUR(normal,  rang::style::reset)
        }

        namespace bg::dim {
            NEW_COLOUR(black,   details::DimBG{colour::bg::black})
            NEW_COLOUR(red,     details::DimBG{colour::bg::red})
            NEW_COLOUR(green,   details::DimBG{colour::bg::green})
            NEW_COLOUR(yellow,  details::DimBG{colour::bg::yellow})
            NEW_COLOUR(blue,    details::DimBG{colour::bg::blue})
            NEW_COLOUR(magenta, details::DimBG{colour::bg::magenta})
            NEW_COLOUR(cyan,    details::DimBG{colour::bg::cyan})
            NEW_COLOUR(grey,    details::DimBG{colour::bg::grey})
            NEW_COLOUR(normal,  rang::style::reset)
        }
    }



    // Overloads.
    inline std::ostream& operator<<(
        std::ostream& os, const details::Reset&
    ) {
        return (os << rang::style::reset << rang::fg::reset << rang::bg::reset);
    }



    inline std::ostream& operator<<(
        std::ostream& os, const details::DimFG& obj
    ) {
        return (os << rang::style::dim << obj.colour);
    }



    inline std::ostream& operator<<(
        std::ostream& os, const details::DimBG& obj
    ) {
        return (os << rang::style::dim << obj.colour);
    }



    inline std::ostream& operator<<(
        std::ostream& os, const details::Notice&
    ) {
        return (os << style::bold << colour::notice);
    }



    inline std::ostream& operator<<(
        std::ostream& os, const details::Warn&
    ) {
        return (os << style::bold << colour::warn);
    }



    inline std::ostream& operator<<(
        std::ostream& os, const details::Error&
    ) {
        return (os << style::bold << colour::error);
    }



    // Custom styles
    namespace style {
        inline details::Reset reset;
        inline details::Notice notice;
        inline details::Warn warn;
        inline details::Error error;
    }



    // Output symbols
    namespace details {
        namespace symbol {
            constexpr auto notice = "[-] ";
            constexpr auto warn   = "[*] ";
            constexpr auto error  = "[!] ";
        }
    }



    // Streams.
    inline std::ostream& piper = std::cout;
    inline std::ostream& printer = std::cerr;









    // Logging and IO.
    template <typename... Ts>
    inline std::ostream& print(Ts&&... args) {
        return (
            (ws::module::printer <<
                style::reset) <<
                ... <<
                std::forward<Ts&&>(args)
        ) << style::reset;
    }



    template <typename... Ts>
    inline std::ostream& pipe(Ts&&... args) {
        return (ws::module::piper << ... << std::forward<Ts&&>(args));
    }






    // Logging and IO...
    template <typename... Ts>
    inline std::ostream& println(Ts&&... args) {
        return ws::module::print(std::forward<Ts&&>(args)...) << '\n';
    }



    template <typename... Ts>
    inline std::ostream& pipeln(Ts&&... args) {
        return ws::module::pipe(std::forward<Ts&&>(args)...) << '\n';
    }






    // Logging with colour.
    template <typename... Ts>
    inline std::ostream& notice(Ts&&... args) {
        ws::module::print(style::notice, details::symbol::notice);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    template <typename... Ts>
    inline std::ostream& warn(Ts&&... args) {
        ws::module::print(style::warn, details::symbol::warn);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    template <typename... Ts>
    inline std::ostream& error(Ts&&... args) {
        ws::module::print(style::error, details::symbol::error);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }



    // Print lines too.
    template <typename... Ts>
    inline std::ostream& noticeln(Ts&&... args) {
        return ws::module::notice(std::forward<Ts&&>(args)..., '\n');
    }



    template <typename... Ts>
    inline std::ostream& warnln(Ts&&... args) {
        return ws::module::warn(std::forward<Ts&&>(args)..., '\n');
    }



    template <typename... Ts>
    inline std::ostream& errorln(Ts&&... args) {
        return ws::module::error(std::forward<Ts&&>(args)..., '\n');
    }









    // Easter eggs
    namespace details {
        using colour_t = std::remove_cv_t<decltype(colour::fg::bright::black)>;


        constexpr colour_t palette[] = {
            colour::fg::bright::red,
            colour::fg::bright::green,
            colour::fg::bright::yellow,
            colour::fg::bright::blue,
            colour::fg::bright::magenta,
            colour::fg::bright::cyan
        };


        inline std::random_device rd;
        inline std::mt19937 rng(rd());
        inline std::uniform_int_distribution<int> uni(0, std::size(palette) - 1);


        inline colour_t random_colour() {
            return details::palette[uni(rng)];
        }
    }



    template <typename T>
    inline std::ostream& rainbow(T&& arg) {
        std::stringstream ss;
        ss << arg;

        for (const auto& chr: ss.str())
            ws::module::print(details::random_colour(), chr);

        return ws::module::printer;
    }



    template <typename T1, typename T2, typename... Ts>
    inline std::ostream& rainbow(T1 arg1, T2 arg2, Ts&&... args) {
        ws::module::rainbow(arg1);
        return ws::module::rainbow(arg2, std::forward<Ts&&>(args)...);
    }



    template <typename... Ts>
    inline std::ostream& rainbowln(Ts&&... args) {
        return ws::module::rainbow(std::forward<Ts&&>(args)..., '\n');
    }



    // Remove all macros.
    #undef NEW_COLOUR
}


#endif
