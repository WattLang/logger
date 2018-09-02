#ifndef WS_MODULE_H
#define WS_MODULE_H


#include <cstdint>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <utility>


namespace ws {
    // Receive data in chunks.
    // Callback = (const std::string& buffer, int chunk_id, bool eof);
    void receive(
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
    std::string receive_all(uintmax_t buffer_size) {
        std::string accumulator;


        receive(
            buffer_size,

            [&] (const std::string& buffer, int chunk_id, bool eof) {
                accumulator += buffer;
            }
        );


        return accumulator;
    }






    // Logging and IO.
    template<typename... Ts>
    std::ostream& print(Ts&&... args) {
        return (std::cerr << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    std::ostream& pipe(Ts&&... args) {
        return (std::cout << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    std::ostream& notice(Ts&&... args) {
        return (std::cerr << "[-] " << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    std::ostream& warn(Ts&&... args) {
        return (std::cerr << "[*] " << ... << std::forward<Ts&&>(args));
    }



    template<typename... Ts>
    std::ostream& error(Ts&&... args) {
        return (std::cerr << "[!] " << ... << std::forward<Ts&&>(args));
    }






    // Logging and IO with line endings...
    template<typename... Ts>
    std::ostream& println(Ts&&... args) {
        return ws::print(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    std::ostream& pipeln(Ts&&... args) {
        return ws::pipe(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    std::ostream& noticeln(Ts&&... args) {
        return ws::notice(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    std::ostream& warnln(Ts&&... args) {
        return ws::warn(std::forward<Ts&&>(args)...) << "\n";
    }



    template<typename... Ts>
    std::ostream& errorln(Ts&&... args) {
        return ws::error(std::forward<Ts&&>(args)...) << "\n";
    }
}


#endif
