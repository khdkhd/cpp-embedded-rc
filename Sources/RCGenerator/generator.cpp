#include "generator.hpp"
#include "io.hpp"
#include "utils.hpp"

#include <cstdint>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace khdkhd::rc {
namespace {

const std::string CPPHeaderTemplate = R"__(// 🤖 -=> Generated Code <=- 🤖
#include <rc.hpp>

#include <mutex>
#include <string>

)__";

const std::string CPPInitializeTemplate = R"__(
void rc_initiliaze_%1%() {
    using khdkhd::rc::register_resource;
    static std::once_flag inialized;
    std::call_once(inialized, []{
%2%    });
}
)__";

std::string
rc_entry_name(unsigned int index) {
    using boost::format;
    return str(format("rc_data_%02u") % index);
}

std::string
rc_entry_path(
    const fs::path &base,
    const fs::path &prefix,
    const fs::path &rc
) {
    return (prefix/fs::relative(fs::canonical(rc), fs::canonical(base))).string();
}

char
convert(char c) {
    return std::isprint(c) ? c : '.';
}

io::Writer
comment(const std::string &message) {
    return [&message](std::ostream &output) -> std::ostream & {
        return output << "// " << message << "\n";
    };
}

struct Hex {
    std::string segment;
    const unsigned int segmentSize;

    Hex(unsigned int segmentSize)
        : segmentSize{segmentSize} {
    }

    std::string
    operator()(const char c) {
        using boost::format;
        std::string s;

        segment.push_back(convert(c));

        if (segment.size() == segmentSize) {
            s = str(format("// %s\n    ") % segment);
            segment.clear();
        }
        return str(format("0x%02x, %s") % static_cast<uint16_t>((uint8_t)c) % s);
    }

    std::string
    flush() const {
        using boost::format;
        if (!segment.empty()) {
            return str(format("%s// %s")
                % std::string((segmentSize - segment.size())*6, ' ')
                % segment
            );
        }
        return "";
    }
};

io::Writer
data(const fs::path &rc, unsigned int segment_size = 8) {
    using boost::format;
    return [&rc, segment_size](std::ostream &output) -> std::ostream & {
        auto input = std::ifstream(rc.string(), std::ios_base::in|std::ios_base::binary);
        auto hex = Hex(segment_size);
        std::transform(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(),
            std::ostream_iterator<std::string>(output),
            [&hex](const auto c) {
                return hex(c);
            }
        );
        return output << hex.flush() << '\n';
    };
}

io::Writer
generateRCData(
    const std::string &rc_name,
    const std::string &rc_path,
    const fs::path &rc
) {
    using boost::format;
    return [&rc_name, &rc_path, &rc](std::ostream &output) -> std::ostream & {
        return output
            << comment(str((format("begin %s") % rc_path)))
            << format("const unsigned char %1$s[] = {\n    ") % rc_name
            << data(rc)
            << "}; "
            << comment(str((format("end %s") % rc_path)))
            << std::endl;
    };
}

std::string
generateRCRegister(const std::string &rc_path, unsigned int index, unsigned int) {
    using boost::format;
    const auto rc_name = rc_entry_name(index);
    return str(format("        register_resource(%1%, (char *)(%2%), sizeof(%2%));\n")
        % std::quoted(rc_path)
        % rc_name
    );
}

io::Writer
generateRCInitialize(const std::string &name, const std::vector<std::string> &resources) {
    using boost::format;
    return [&name, &resources](std::ostream &output) -> std::ostream & {
        std::stringstream ss;
        algo::indexed_transform(
            std::begin(resources),
            std::end(resources),
            std::ostream_iterator<std::string>(ss),
            &generateRCRegister
        );

        return output << boost::format(CPPInitializeTemplate) % name % ss.str();
    };
}
} // namespace

struct Generator::impl {
    std::vector<std::string> resources;
    std::ostream &output;
    Config config;

    impl(std::ostream &output, Config config)
        : output{output}
        , config{config} {
    }
};

Generator::Generator(std::ostream &output, Config config)
    : pimpl_{std::make_unique<impl>(output, config)} {
    pimpl_->output
        << CPPHeaderTemplate
        << "namespace {\n";
}

Generator::~Generator() {
    pimpl_->output
        << "} " << comment("namespace")
        << generateRCInitialize(pimpl_->config.name, pimpl_->resources)
        << std::endl;
}

void
Generator::push_back(const boost::filesystem::path &resource) {
    using boost::format;

    if (pimpl_->config.verbose) {
        std::cerr << "Push " << resource << std::endl;
    }

    const auto rc_name = rc_entry_name(pimpl_->resources.size());
    const auto rc_path = rc_entry_path(pimpl_->config.base, pimpl_->config.prefix, resource);

    pimpl_->resources.push_back(rc_path);
    pimpl_->output << generateRCData(rc_name, rc_path, resource);
}

} // namespace khdkhd::rc
