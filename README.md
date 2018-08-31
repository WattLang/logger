# module
A simple header for making the task of piping data between processes easier.


Example usage:
```cpp
#include <iostream>
#include <string>

#include "module.h"


int BUFFER_SIZE = 32;


int main(int argc, char const *argv[]) {
    ws::Receiver recv(BUFFER_SIZE);  // Receives data from pipe.


    // Callback that is run every time a new piece of data is received.
    auto callback = [&] (const std::string& buffer, int id, bool is_end) {
        // Write the data out to the pipe.
        ws::pipe(buffer);
    };


    // Run the loop.
    recv.read(callback);

    return 0;
}
```

Run:

`echo "Hello World!" | ./reciever.out`

`cat <filename> | ./reciever.out`

`./reciever.out < <filename>`

`cat <filename> | ./reciever.out | ./reciever.out`
