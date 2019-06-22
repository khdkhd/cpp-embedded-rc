#include <functional>
#include <ostream>

namespace khdkhd::io {
using Writer = std::function<std::ostream &(std::ostream &)>;
}

std::ostream &operator<<(std::ostream &os, const khdkhd::io::Writer &);
