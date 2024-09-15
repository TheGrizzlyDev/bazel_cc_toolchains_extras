#include <iostream>
#include "lib.hh"

int main() {
    if (! write_hello_world("hello_world.txt")) {
        std::cerr << "Couldn't write to hello_world.txt" << std::endl;
        return 1;
    }

    std::cout << "Hello, World! written to hello_world.txt" << std::endl;
    return 0;
}
