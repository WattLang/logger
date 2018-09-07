#ifndef WS_MODULE_H
#define WS_MODULE_H


#include <cstdint>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <utility>

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





    // Streams.
    inline std::ostream& piper = std::cout;
    inline std::ostream& printer = std::cerr;

    // Output symbols
    constexpr auto SYMBOL_NOTICE = "[-] ";
    constexpr auto SYMBOL_WARN   = "[*] ";
    constexpr auto SYMBOL_ERROR  = "[!] ";





    // Logging and IO.
    template<typename... Ts>
    inline std::ostream& print(Ts&&... args) {
        return (ws::module::printer << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    inline std::ostream& pipe(Ts&&... args) {
        return (ws::module::piper << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    inline std::ostream& notice(Ts&&... args) {
        return ws::module::print(SYMBOL_NOTICE, std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& warn(Ts&&... args) {
        return ws::module::print(SYMBOL_WARN, std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& error(Ts&&... args) {
        return ws::module::print(SYMBOL_ERROR, std::forward<Ts&&>(args)...);
    }






    // Logging and IO with line endings...
    template<typename... Ts>
    inline std::ostream& println(Ts&&... args) {
        return ws::module::print(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& pipeln(Ts&&... args) {
        return ws::module::pipe(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& noticeln(Ts&&... args) {
        return ws::module::notice(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& warnln(Ts&&... args) {
        return ws::module::warn(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& errorln(Ts&&... args) {
        return ws::module::error(std::forward<Ts&&>(args)...) << "\n";
    }


    //Coloured print functions 

    using fg = rang::fgB;

    constexpr auto NOTICE_COLOR = fg::blue;
    constexpr auto WARN_COLOR   = fg::yellow;
    constexpr auto ERROR_COLOR  = fg::red;

    template<typename... Ts>
    inline std::ostream& printc(fg colour, Ts&&... args) {
        return ws::module::print(colour, std::forward<Ts&&>(args)..., rang::fg::reset);
    }


    template<typename... Ts>
    inline std::ostream& noticec(Ts&&... args) {
        ws::module::printc(NOTICE_COLOR, SYMBOL_NOTICE);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }


    template<typename... Ts>
    inline std::ostream& warnc(Ts&&... args) {
        ws::module::printc(WARN_COLOR, SYMBOL_WARN);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }


    template<typename... Ts>
    inline std::ostream& errorc(Ts&&... args) {
        ws::module::printc(ERROR_COLOR, SYMBOL_ERROR);
        return ws::module::print(std::forward<Ts&&>(args)...);
    }

    template<typename... Ts>
    inline std::ostream& printlnc(fg colour, Ts&&... args) {
        return ws::module::printc(colour, std::forward<Ts&&>(args)..., '\n');
    }


    template<typename... Ts>
    inline std::ostream& noticelnc(Ts&&... args) {
        return ws::module::noticec(std::forward<Ts&&>(args)..., '\n');
    }



    template<typename... Ts>
    inline std::ostream& warnlnc(Ts&&... args) {
        return ws::module::warnc(std::forward<Ts&&>(args)..., '\n');
    }



    template<typename... Ts>
    inline std::ostream& errorlnc(Ts&&... args) {
        return ws::module::errorc(std::forward<Ts&&>(args)..., '\n');
    }
    
}


#endif
