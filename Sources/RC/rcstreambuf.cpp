#include "rc.hpp"

#include <map>
#include <iostream>

namespace khdkhd::rc {

extern std::map<std::string, RCEntry> resourcesMap;

streambuf &
streambuf::open(const std::string &path) {
    const auto entry = resourcesMap.find(path);
    open_ = false;
    if (entry != resourcesMap.end()) {
        const auto rc = entry->second;
        setg(rc.data, rc.data, rc.data + rc.size);
        open_ = true;
    }
    return *this;
}

bool
streambuf::is_open() const {
    return open_;
}

streambuf::pos_type
streambuf::seekoff(
    off_type off,
    std::ios_base::seekdir dir,
    std::ios_base::openmode which) {
    switch (dir) {
    case std::ios_base::beg:
        return seekpos(pos_type(off), which);

    case std::ios_base::cur:
        return seekpos(pos_type(gptr() - eback()) + off, which);

    case std::ios_base::end:
        return seekpos(pos_type(egptr() - eback()) + off, which);

    default:
        break;
    }
    return pos_type(off_type(-1));
}

streambuf::pos_type
streambuf::seekpos(
    pos_type pos,
    std::ios_base::openmode which) {
    if (is_open() && ((which | std::ios_base::in) != 0)) {
        const auto ptr = eback() + pos;
        if (ptr >= eback() && ptr <= egptr()) {
            setg(eback(), std::min(eback() + pos, egptr()), egptr());
            return pos_type(gptr() - eback());
        }
    }
    return pos_type(off_type(-1));
}
}
