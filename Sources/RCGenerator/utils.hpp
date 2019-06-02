#pragma once

#include <algorithm>
#include <cstdint>

namespace khdkhd::algo {

template<typename InputIt, typename OutputIt, typename CallbackFunction>
OutputIt
indexed_transform(
    InputIt first,
    InputIt last,
    OutputIt output,
    CallbackFunction cb) {
    auto index = 0;
    return std::transform(first, last, output, [&](const auto &elt) {
        return cb(elt, index++, last - first);
    });
}

} // namespace khdkhd::algo
