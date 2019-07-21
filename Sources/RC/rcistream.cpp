#include "rc.hpp"

namespace khdkhd::rc {

struct istream::impl {
    streambuf rdbuf;
};

istream::istream()
    : std::istream(nullptr)
    , pimpl_{std::make_unique<impl>()} {
}

istream::istream(const std::string &path)
    : std::istream(nullptr)
    , pimpl_{std::make_unique<impl>()} {
    open(path);
}

istream::~istream() {
}

istream &
istream::open(const std::string &path) {
    if (pimpl_->rdbuf.open(path).is_open()) {
        clear();
        rdbuf(&pimpl_->rdbuf);
    } else {
        setstate(std::ios_base::failbit);
    }
    return *this;
}

bool
istream::is_open() const {
    return pimpl_->rdbuf.is_open();
}

}
