#pragma once

#include <cstdint>
#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define RC_DECLARE(RC) extern void rc_initiliaze_##RC();
#define RC_REGISTER(RC) do { \
    rc_initiliaze_##RC();    \
} while (0)

namespace khdkhd::rc {

struct Error : public std::exception {
    const std::string rc;
    const std::string cause;
    Error(const std::string &rc, const std::string &cause);
    const char *what() const noexcept override;
};

class rcstreambuf : public std::streambuf {
    bool open_{false};

public:
    rcstreambuf();

public:
    rcstreambuf *open(const std::string &path);
    bool is_open() const;

protected:
    int_type underflow() override;
};

void registerResource(const std::string &path, char *data, std::size_t size);
bool exists(const std::string &path);
std::vector<std::string> resources();

} // namespace khdkhd::rc
