#include "rc.hpp"

#include <boost/format.hpp>
#include <map>

namespace khdkhd::rc {

std::map<std::string, RCEntry> resourcesMap;

Error::Error(const std::string &rc, const std::string &cause)
    : rc{rc}
    , cause{cause} {
}

const char *Error::what() const noexcept {
    return str(boost::format("Error: '%s' Cause: %s") % rc % cause).c_str();
}

void
register_resource(
    const std::string &path,
    char *data,
    std::size_t size) {
    if (resourcesMap.count(path) > 0) {
        throw Error(path, "Resource has already been registered!");
    }
    resourcesMap.insert({path, RCEntry{size, data}});
}

bool
exists(const std::string &path) {
    return resourcesMap.find(path) != resourcesMap.end();
}

streambuf
open(const std::string &path) {
    streambuf rcbuf;
    rcbuf.open(path);
    return rcbuf;
}

std::vector<std::string>
list() {
    std::vector<std::string> res;
    std::transform(
        std::begin(resourcesMap),
        std::end(resourcesMap),
        std::back_inserter(res),
        [](const auto pair) {
            return pair.first;
        }
    );
    return res;
}

} // namespace khdkhd::rc
