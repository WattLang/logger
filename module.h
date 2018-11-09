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






    // Wrappers to help implement compile time checks for compiling code.
    template <bool B, typename F>
    constexpr void run_if(F&& func) {
        if constexpr(B) std::forward<F>(func)();
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






    // Compile time variants.
    template <bool B, typename... Ts>
    constexpr std::ostream& print_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::print(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& pipe_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::pipe(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& notice_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::notice(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warn_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::warn(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& error_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::error(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& success_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::success(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }






    // Compile time variants with newlines.
    template <bool B, typename... Ts>
    constexpr std::ostream& println_if(Ts&&... args) {
        return ws::module::print_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& pipeln_if(Ts&&... args) {
        return ws::module::pipe_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& noticeln_if(Ts&&... args) {
        return ws::module::notice_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warnln_if(Ts&&... args) {
        return ws::module::warn_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& errorln_if(Ts&&... args) {
        return ws::module::error_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& successln_if(Ts&&... args) {
        return ws::module::success_if<B>(std::forward<Ts&&>(args)..., '\n');
    }






    // Compile time print repeated characters.
    namespace details {
        // unsafe, can be called like: print_repeat(5, 'n') or print_repeat('n', 5)
        template <bool B>
        constexpr std::ostream& print_repeat_if(char c, unsigned n) {
            return ws::module::print_if<B>(ws::module::details::repeat(c, n));
        }
    }


    template <bool B>
    constexpr std::ostream& print_repeat_if(const std::string& c, unsigned n) {
        return ws::module::print_if<B>(ws::module::repeat(c, n));
    }


    template <bool B>
    constexpr std::ostream& print_tabs_if(unsigned n) {
        return ws::module::details::print_repeat_if<B>('\t', n);
    }


    template <bool B>
    constexpr std::ostream& print_spaces_if(unsigned n) {
        return ws::module::details::print_repeat_if<B>(' ', n);
    }


    template <bool B>
    constexpr std::ostream& print_lines_if(unsigned n) {
        return ws::module::details::print_repeat_if<B>('\n', n);
    }







    // Compile time emphasised variants.
    template <bool B, typename... Ts>
    constexpr std::ostream& print_em_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::print_em(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& notice_em_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::notice_em(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warn_em_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::warn_em(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& error_em_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::error_em(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& success_em_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::success_em(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }






    // Compile time emphasised variants with newlines.
    template <bool B, typename... Ts>
    constexpr std::ostream& println_em_if(Ts&&... args) {
        return ws::module::print_em_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& noticeln_em_if(Ts&&... args) {
        return ws::module::notice_em_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warnln_em_if(Ts&&... args) {
        return ws::module::warn_em_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& errorln_em_if(Ts&&... args) {
        return ws::module::error_em_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& successln_em_if(Ts&&... args) {
        return ws::module::success_em_if<B>(std::forward<Ts&&>(args)..., '\n');
    }







    // Compile time heading variants.
    template <bool B, typename... Ts>
    constexpr std::ostream& print_h_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::print_h(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& notice_h_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::notice_h(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warn_h_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::warn_h(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& error_h_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::error_h(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& success_h_if(Ts&&... args) {
        if constexpr(B)
            return ws::module::success_h(std::forward<Ts&&>(args)...);

        return ws::module::printer;
    }






    // Compile time heading variants with newlines.
    template <bool B, typename... Ts>
    constexpr std::ostream& println_h_if(Ts&&... args) {
        return ws::module::print_h_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& noticeln_h_if(Ts&&... args) {
        return ws::module::notice_h_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& warnln_h_if(Ts&&... args) {
        return ws::module::warn_h_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& errorln_h_if(Ts&&... args) {
        return ws::module::error_h_if<B>(std::forward<Ts&&>(args)..., '\n');
    }


    template <bool B, typename... Ts>
    constexpr std::ostream& successln_h_if(Ts&&... args) {
        return ws::module::success_h_if<B>(std::forward<Ts&&>(args)..., '\n');
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
