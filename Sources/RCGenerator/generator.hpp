#pragma once

#include "io.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace khdkhd::rc {

class Generator {
    struct impl;
    std::unique_ptr<impl> pimpl_;

public:
    using value_type = boost::filesystem::path;
    struct Config {
        const std::string name;
        const boost::filesystem::path base;
        const boost::filesystem::path prefix;
        bool verbose{false};
    };

public:
    Generator(std::ostream &, Config);
    virtual ~Generator();

public:
    void push_back(const boost::filesystem::path &resource);
};

} // namespace khdkhd::rc
