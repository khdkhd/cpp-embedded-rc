#include "io.hpp"

std::ostream &
operator<<(std::ostream &os, const khdkhd::io::Writer &s) {
    return s(os);
}
