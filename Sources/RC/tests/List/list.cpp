#include <rc.hpp>
#include <gtest/gtest.h>

namespace khdkhd::rc {
extern std::map<std::string, RCEntry> resourcesMap;
}

namespace {

char data1[] = "foo";
char data2[] = "bar";
char data3[] = "gee";

class list : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Register resources
        khdkhd::rc::resourcesMap.clear();
        khdkhd::rc::register_resource("/foo", data1, sizeof(data1));
        khdkhd::rc::register_resource("/bar", data2, sizeof(data2));
        khdkhd::rc::register_resource("/gee", data3, sizeof(data3));
    }
};

TEST_F(list, returnTrueIfResourceExists) {
    std::vector<std::string> expected{"/foo", "/bar", "/gee"};
    const auto list = khdkhd::rc::list();
    ASSERT_TRUE(std::is_permutation(expected.begin(), expected.end(), list.begin()));
}
}
