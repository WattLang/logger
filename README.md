# module
A simple header for making the task of piping data between processes easier.

### Examples

##### 1. Processing streamed data

```cpp
#include <iostream>
#include "module.h"


int BUFFER_SIZE = 32;


int main(int argc, char const *argv[]) {
    // Stream input data and process it in chunks.
    ws::module::receive(
        BUFFER_SIZE,

        [&] (const std::string& buffer, int id, bool is_end) {
            // Pass the data along through the pipe.
            ws::module::piper << buffer;
        }
    );


    return 0;
}
```

##### 2. Processing accumulated data

```cpp
#include <iostream>
#include "module.h"


int BUFFER_SIZE = 32;


int main(int argc, char const *argv[]) {
    // Accumulate all input data.
    auto contents = ws::module::receive_all(BUFFER_SIZE);


    // Pipe the contents.
    ws::module::piper << contents;

    return 0;
}
```

##### 3. Logging & IO
```cpp
#include <iostream>
#include "module.h"


int main(int argc, char const *argv[]) {

    ws::module::print("Hello", " ", "there!", "\n");
    ws::module::println("Hello", " ", "there!");

    ws::module::pipe("Pipe ", 1, "\n");
    ws::module::pipeln("Pipe ", 2);

    ws::module::notice("Notice ", 1, "\n");
    ws::module::noticeln("Notice ", 2);

    ws::module::warn("Warn ", 1, "\n");
    ws::module::warnln("Warn ", 2);

    ws::module::error("Error ", 1, "\n");
    ws::module::errorln("Error ", 2);

    ws::module::print("Yes!") << " " << "No!" << std::endl;

    ws::module::printer << "Printer\n";
    ws::module::piper << "Piper\n";

    return 0;
}
```

### Run

> Note: This works with `bash` but should also work with other shells that support the same functionality.

`echo "Hello World!" | ./reciever.out`

`cat <filename> | ./reciever.out`

`./reciever.out < <filename>`

`cat <filename> | ./reciever.out | ./reciever.out`
