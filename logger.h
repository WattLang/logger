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
#include <cstdint>


#include "rang/include/rang.hpp"



/*
    Macros and constants:

    WS_MODULE_LOG_OFF: disables all logging.
    WS_MODULE_PIPE_OFF: disables all piping.
*/



namespace ws::module {
    // Macros
    #define NEW_COLOUR(name, val) constexpr auto name = val;



    // Utility functions
    namespace details {
        // unsafe, can be called like: repeat(5, 'n') or repeat('n', 5)
        inline std::string repeat(char c, unsigned n) {
            return std::string(n, c);
        }
    }



    // std::string constructor does not allow repeating a string so
    // this function implements it.
    inline std::string repeat(const std::string& c, unsigned n) {
        if (!n) return ""; // Check for 0.

        std::string out = c;

        for (n--; n > 0; n--) out += c;

        return out;
    }


    inline std::string spaces(unsigned n) {
        return ws::module::details::repeat(' ', n);
    }


    inline std::string tabs(unsigned n) {
        return ws::module::details::repeat('\t', n);
    }


    inline std::string lines(unsigned n) {
        return ws::module::details::repeat('\n', n);
    }



    namespace details {
        constexpr auto DEFAULT_BUFFER_SIZE = 32;

        using bool_str = std::tuple<std::string, bool>;
    }



    inline details::bool_str receive_one(
        std::streamsize buffer_size = details::DEFAULT_BUFFER_SIZE
    ) {
        std::string buffer(static_cast<uintmax_t>(buffer_size), '\0');


        if (std::cin.read(&buffer[0], buffer_size))
            return std::make_tuple(buffer, false); // EOF = false;


        // Cleanup trailing garbage.
        buffer.erase(buffer.begin() + std::cin.gcount(), buffer.end());

        // EOF = true;
        return std::make_tuple(buffer, true);
    }



    // Receive data in chunks.
    // Callback = (const std::string& buffer, int chunk_id, bool eof);
    inline void receive(
        std::function<void(const std::string&, int, bool)> callback,
        std::streamsize buffer_size = details::DEFAULT_BUFFER_SIZE
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
    inline std::string receive_all() {
        // Find the size of the input stream.
        std::cin.seekg(0, std::cin.end);
        auto size = std::cin.tellg();
        std::cin.seekg(0);


        // Check stream actually has content.
        if (size <= 0) return std::string{};


        // Reserve a buffer.
        std::string str(static_cast<std::string::size_type>(size), '\0');


        // Read the stream into the buffer.
        std::cin.read(str.data(), static_cast<std::streamsize>(size));

        return str;
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


        NEW_COLOUR(notice,  fg::bright::blue)
        NEW_COLOUR(warn,    fg::bright::yellow)
        NEW_COLOUR(error,   fg::bright::red)
        NEW_COLOUR(success, fg::bright::green)
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

        // Success style, set bold and success colour.
        struct Success {};



        // Overloads.
        inline std::ostream& operator<<(
            std::ostream& os, const Reset&
        ) {
            return (os << rang::style::reset << rang::fg::reset << rang::bg::reset);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const DimFG& obj
        ) {
            return (os << rang::style::dim << obj.colour);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const DimBG& obj
        ) {
            return (os << rang::style::dim << obj.colour);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const Notice&
        ) {
            return (os << style::bold << colour::notice);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const Warn&
        ) {
            return (os << style::bold << colour::warn);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const Error&
        ) {
            return (os << style::bold << colour::error);
        }



        inline std::ostream& operator<<(
            std::ostream& os, const Success&
        ) {
            return (os << style::bold << colour::success);
        }
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



    // Custom styles
    namespace style {
        inline details::Reset reset;
        inline details::Notice notice;
        inline details::Warn warn;
        inline details::Error error;
        inline details::Success success;
    }



    // Output symbols
    namespace details {
        namespace symbol {
            constexpr auto notice  = "[-] ";
            constexpr auto warn    = "[*] ";
            constexpr auto error   = "[!] ";
            constexpr auto success = "[^] ";
        }
    }



    // Streams.
    inline std::ostream& piper = std::cout;
    inline std::ostream& printer = std::cerr;









    // Logging and IO.
    template <typename... Ts>
    inline std::ostream& print(Ts&&... args) {
        #ifndef WS_MODULE_LOG_OFF
            return (
                (ws::module::printer <<
                    style::reset) <<
                    ... <<
                    std::forward<Ts&&>(args)
            ) << style::reset;

        #else
            return ws::module::printer;

        #endif
    }



    template <typename... Ts>
    inline std::ostream& pipe(Ts&&... args) {
        #ifndef WS_MODULE_PIPE_OFF
            return (ws::module::piper << ... << std::forward<Ts&&>(args));

        #else
            return ws::module::piper;

        #endif
    }






    // Logging and IO...
    template <typename... Ts>
    inline std::ostream& println(Ts&&... args) {
        #ifndef WS_MODULE_LOG_OFF
            return ws::module::print(std::forward<Ts&&>(args)...) << '\n';

        #else
            return ws::module::printer;

        #endif
    }



    template <typename... Ts>
    inline std::ostream& pipeln(Ts&&... args) {
        #ifndef WS_MODULE_PIPE_OFF
            return ws::module::pipe(std::forward<Ts&&>(args)...) << '\n';

        #else
            return ws::module::piper;

        #endif
    }





    // Print repeated characters.
    namespace details {
        // unsafe, can be called like: print_repeat(5, 'n') or print_repeat('n', 5)
        inline std::ostream& print_repeat(char c, unsigned n) {
            return ws::module::print(ws::module::details::repeat(c, n));
        }
    }


    inline std::ostream& print_repeat(const std::string& c, unsigned n) {
        return ws::module::print(ws::module::repeat(c, n));
    }


    inline std::ostream& print_tabs(unsigned n) {
        return ws::module::details::print_repeat('\t', n);
    }


    inline std::ostream& print_spaces(unsigned n) {
        return ws::module::details::print_repeat(' ', n);
    }


    inline std::ostream& print_lines(unsigned n) {
        return ws::module::details::print_repeat('\n', n);
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



    template <typename... Ts>
    inline std::ostream& success(Ts&&... args) {
        ws::module::print(style::success, details::symbol::success);
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



    template <typename... Ts>
    inline std::ostream& successln(Ts&&... args) {
        return ws::module::success(std::forward<Ts&&>(args)..., '\n');
    }






    // Emphasised prints.
    template <typename T, typename... Ts>
    inline std::ostream& print_em(T&& first, Ts&&... args) {
        return ws::module::print(
            style::bold, first, style::reset, std::forward<Ts&&>(args)...
        );
    }


    template <typename T, typename... Ts>
    inline std::ostream& notice_em(T&& first, Ts&&... args) {
        return ws::module::notice(
            style::bold, first, style::reset, std::forward<Ts&&>(args)...
        );
    }


    template <typename T, typename... Ts>
    inline std::ostream& warn_em(T&& first, Ts&&... args) {
        return ws::module::warn(
            style::bold, first, style::reset, std::forward<Ts&&>(args)...
        );
    }


    template <typename T, typename... Ts>
    inline std::ostream& error_em(T&& first, Ts&&... args) {
        return ws::module::error(
            style::bold, first, style::reset, std::forward<Ts&&>(args)...
        );
    }


    template <typename T, typename... Ts>
    inline std::ostream& success_em(T&& first, Ts&&... args) {
        return ws::module::success(
            style::bold, first, style::reset, std::forward<Ts&&>(args)...
        );
    }







    // Emphasis with newlines.
    template <typename... Ts>
    inline std::ostream& println_em(Ts&&... args) {
        return ws::module::print_em(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& noticeln_em(Ts&&... args) {
        return ws::module::notice_em(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& warnln_em(Ts&&... args) {
        return ws::module::warn_em(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& errorln_em(Ts&&... args) {
        return ws::module::error_em(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& successln_em(Ts&&... args) {
        return ws::module::success_em(std::forward<Ts&&>(args)..., '\n');
    }







    // Headings.
    template <typename... Ts>
    inline std::ostream& print_h(Ts&&... args) {
        return ws::module::print(style::bold, std::forward<Ts&&>(args)...);
    }


    template <typename... Ts>
    inline std::ostream& notice_h(Ts&&... args) {
        return ws::module::notice(style::bold, std::forward<Ts&&>(args)...);
    }


    template <typename... Ts>
    inline std::ostream& warn_h(Ts&&... args) {
        return ws::module::warn(style::bold, std::forward<Ts&&>(args)...);
    }


    template <typename... Ts>
    inline std::ostream& error_h(Ts&&... args) {
        return ws::module::error(style::bold, std::forward<Ts&&>(args)...);
    }


    template <typename... Ts>
    inline std::ostream& success_h(Ts&&... args) {
        return ws::module::success(style::bold, std::forward<Ts&&>(args)...);
    }




    // Headings with newlines.
    template <typename... Ts>
    inline std::ostream& println_h(Ts&&... args) {
        return ws::module::print_h(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& noticeln_h(Ts&&... args) {
        return ws::module::notice_h(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& warnln_h(Ts&&... args) {
        return ws::module::warn_h(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& errorln_h(Ts&&... args) {
        return ws::module::error_h(std::forward<Ts&&>(args)..., '\n');
    }


    template <typename... Ts>
    inline std::ostream& successln_h(Ts&&... args) {
        return ws::module::success_h(std::forward<Ts&&>(args)..., '\n');
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



    template <typename...Ts>
    inline std::ostream& rainbow(Ts&&... arg) {
        std::stringstream ss;
        (ss << ... << arg);

        for (const auto& chr: ss.str())
            ws::module::print(details::random_colour(), chr);

        return ws::module::printer;
    }


    template <typename... Ts>
    inline std::ostream& rainbowln(Ts&&... args) {
        return ws::module::rainbow(std::forward<Ts&&>(args)..., '\n');
    }



    // Remove all macros.
    #undef NEW_COLOUR
}


#endif
