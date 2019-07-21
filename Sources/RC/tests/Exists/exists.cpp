#include <rc.hpp>
#include <gtest/gtest.h>

namespace khdkhd::rc {
extern std::map<std::string, RCEntry> resourcesMap;
}

namespace {

char data[] = "foo";

class exists : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Register resources
        khdkhd::rc::resourcesMap.clear();
        khdkhd::rc::register_resource("/foo", data, sizeof(data));
    }
};

TEST_F(exists, returnTrueIfResourceExists) {
    ASSERT_TRUE(khdkhd::rc::exists("/foo"));
}
//
}
