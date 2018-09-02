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
    ws::receive(
        BUFFER_SIZE,

        [&] (const std::string& buffer, int id, bool is_end) {
            // Pass the data along through the pipe.
            ws::pipe(buffer);
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
    auto contents = ws::receive_all(BUFFER_SIZE);


    // Pipe the contents.
    ws::pipe(contents);

    return 0;
}
```

### Run

> Note: This works with `bash` but should also work with other shells that support the same functionality.

`echo "Hello World!" | ./reciever.out`

`cat <filename> | ./reciever.out`

`./reciever.out < <filename>`

`cat <filename> | ./reciever.out | ./reciever.out`
