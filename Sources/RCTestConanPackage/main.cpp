#include <rc.hpp>

#include <fstream>
#include <iostream>

RC_DECLARE(Assets);

int
main(int argc, char **argv) {
    RC_REGISTER(Assets);
    for (const auto rc: khdkhd::rc::resources()) {
        std::cout << rc << std::endl;
    }
    return 0;
}
