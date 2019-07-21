#pragma once

#include <cstdint>
#include <exception>
#include <ios>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define RC_DECLARE(RC) extern void rc_initiliaze_##RC();
#define RC_REGISTER(RC) do { \
    rc_initiliaze_##RC();    \
} while (0)

namespace khdkhd::rc {

struct RCEntry {
    const std::size_t size;
    char *data;
};

struct Error : public std::exception {
    const std::string rc;
    const std::string cause;
    Error(const std::string &rc, const std::string &cause);
    const char *what() const noexcept override;
};

class streambuf : public std::streambuf {
public:
    streambuf &open(const std::string &path);
    bool is_open() const;

protected:
    virtual pos_type seekoff(
        off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which = std::ios_base::in | std::ios_base::out
    ) override;

    virtual pos_type seekpos(
        pos_type pos,
        std::ios_base::openmode which = std::ios_base::in | std::ios_base::out
    ) override;

private:
    bool open_{false};
};

class istream : public std::istream {
public:
    istream();
    istream(const std::string &path);

    virtual ~istream();

public:
    istream &open(const std::string &path);
    bool is_open() const;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

bool exists(const std::string &path);
std::vector<std::string> list();

streambuf open(const std::string &path);

void register_resource(const std::string &path, char *data, std::size_t size);
} // namespace khdkhd::rc
