#pragma once

#include <algorithm>
#include <cstdint>

namespace khdkhd::algo {

template<typename InputIt, typename CallbackFunction>
void for_each_indexed(InputIt first, InputIt last, CallbackFunction f) {
    auto index = 0;
    std::for_each(first, last, [&](const auto &elt) {
        f(elt, index++, last - first);
    });
}

} // namespace khdkhd::algo
