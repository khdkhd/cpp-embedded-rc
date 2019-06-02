#include "utils.hpp"

std::ostream &
operator<<(std::ostream &os, const std::function<std::ostream &(std::ostream &)> &s) {
    return s(os);
}
