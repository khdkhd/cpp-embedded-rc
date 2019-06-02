#include "rc.hpp"

#include <boost/format.hpp>
#include <map>

namespace khdkhd::rc {
namespace {

struct RCEntry {
    const std::size_t size;
    char *data;
};

std::map<std::string, RCEntry> resourcesMap;

} // namespace

Error::Error(const std::string &rc, const std::string &cause)
    : rc{rc}
    , cause{cause} {
}

const char *Error::what() const noexcept {
    return str(boost::format("Error: '%s' Cause: %s") % rc % cause).c_str();
}


rcstreambuf::rcstreambuf() {
}

rcstreambuf *
rcstreambuf::open(const std::string &path) {
    const auto entry = resourcesMap.find(path);
    open_ = false;
    if (entry != resourcesMap.end()) {
        const auto rc = entry->second;
        setg(rc.data, rc.data, rc.data + rc.size);
        open_ = true;
        return this;
    }
    return nullptr;
}

bool
rcstreambuf::is_open() const {
    return open_;
}

rcstreambuf::int_type
rcstreambuf::underflow() {
    return traits_type::eof();
}

void
registerResource(
    const std::string &path,
    char *data,
    std::size_t size) {
    if (resourcesMap.count(path) > 0) {
        throw new Error(path, "Resource has already been registered!");
    }
    resourcesMap.insert({path, RCEntry{size, data}});
}

std::vector<std::string>
resources() {
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
