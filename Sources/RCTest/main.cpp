#include <rc.hpp>

#include <fstream>
#include <iostream>

RC_DECLARE(Assets1);
RC_DECLARE(Assets2);

int
main(int argc, char **argv) {
    RC_REGISTER(Assets1);
    RC_REGISTER(Assets2);

    for (const auto rc: khdkhd::rc::resources()) {
        std::cout << rc << std::endl;
    }

    khdkhd::rc::rcstreambuf rcbuf;

    rcbuf.open("/assets/txt/bar");
    if(rcbuf.is_open()) {
        std::istream input(&rcbuf);
        while (!input.eof()) {
            const auto c = input.get();
            if (c != khdkhd::rc::rcstreambuf::traits_type::eof()) {
                std::cout << static_cast<char>(c);
            }
        }
        std::cout << std::endl;
    }

    rcbuf.open("/assets/png/picture.png");
    if(rcbuf.is_open()) {
        std::istream input(&rcbuf);
        std::fstream output("picture.png", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);
        while (!input.eof()) {
            const auto c = input.get();
            if (c != khdkhd::rc::rcstreambuf::traits_type::eof()) {
                output.put(static_cast<char>(c));
            }
        }
    }

    return 0;
}
