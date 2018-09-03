#ifndef WS_MODULE_H
#define WS_MODULE_H


#include <cstdint>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <utility>


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
        return (ws::printer << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    inline std::ostream& pipe(Ts&&... args) {
        return (ws::piper << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    inline std::ostream& notice(Ts&&... args) {
        return ws::print("[-] ", std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& warn(Ts&&... args) {
        return ws::print("[*] ", std::forward<Ts&&>(args)...);
    }



    template<typename... Ts>
    inline std::ostream& error(Ts&&... args) {
        return ws::print("[!] ", std::forward<Ts&&>(args)...);
    }






    // Logging and IO with line endings...
    template<typename... Ts>
    inline std::ostream& println(Ts&&... args) {
        return ws::print(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& pipeln(Ts&&... args) {
        return ws::pipe(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& noticeln(Ts&&... args) {
        return ws::notice(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& warnln(Ts&&... args) {
        return ws::warn(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    inline std::ostream& errorln(Ts&&... args) {
        return ws::error(std::forward<Ts&&>(args)...) << "\n";
    }
}


// Colour support
#ifndef WS_MODULE_NOCOLOUR

    namespace ws {

    }

#endif


#endif
