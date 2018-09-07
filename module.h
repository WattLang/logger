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

            [&] (const std::string& buffer, int chunk_id, bool eof) {
                accumulator += buffer;
            }
        );


        return accumulator;
    }





    // Streams.
    inline std::ostream& piper = std::cout;
    inline std::ostream& printer = std::cerr;





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
        return ws::module::print("[-] ", std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& warn(Ts&&... args) {
        return ws::module::print("[*] ", std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& error(Ts&&... args) {
        return ws::module::print("[!] ", std::forward<Ts&&>(args)...);
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

    constexpr auto FG_RED    = rang::fgB::red;
    constexpr auto FG_BLUE   = rang::fgB::blue;
    constexpr auto FG_GREEN  = rang::fgB::green;
    constexpr auto FG_YELLOW = rang::fgB::yellow;


    template<typename... Ts>
    inline std::ostream& printc(rang::fgB colour, Ts&&... args) {
        return ((ws::module::printer << colour << ">>> " << rang::fg::reset) << ... << std::forward<Ts&&>(args));
    }


    template<typename... Ts>
    inline std::ostream& noticec(Ts&&... args) {
        return ((ws::module::printer << FG_BLUE << "[-] " << rang::fg::reset ) << ... << std::forward<Ts&&>(args));
    }


    template<typename... Ts>
    inline std::ostream& warnc(Ts&&... args) {
        return ((ws::module::printer << FG_YELLOW << "[*] " << rang::fg::reset ) << ... << std::forward<Ts&&>(args));
    }


    template<typename... Ts>
    inline std::ostream& errorc(Ts&&... args) {
        return ((ws::module::printer << FG_RED << "[!] " << rang::fg::reset ) << ... << std::forward<Ts&&>(args));
    }

    template<typename... Ts>
    inline std::ostream& printlnc(rang::fgB colour, Ts&&... args) {
        return ws::module::printc(colour, std::forward<Ts&&>(args)...) << "\n";
    }


    template<typename... Ts>
    inline std::ostream& noticelnc(Ts&&... args) {
        return ws::module::noticec(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& warnlnc(Ts&&... args) {
        return ws::module::warnc(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& errorlnc(Ts&&... args) {
        return ws::module::errorc(std::forward<Ts&&>(args)...) << "\n";
    }
    
}


#endif
