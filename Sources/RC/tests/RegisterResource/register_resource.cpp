#include <rc.hpp>
#include <gtest/gtest.h>

namespace khdkhd::rc {
extern std::map<std::string, RCEntry> resourcesMap;
}

namespace {

class register_resource : public ::testing::Test {
protected:
    virtual void SetUp() {
        khdkhd::rc::resourcesMap.clear();
    }
};

// Register a resource
TEST_F(register_resource, addResourceToRegistry) {
    const auto rc = "/foo";
    char data[] = "foo";
    ASSERT_TRUE(khdkhd::rc::resourcesMap.count(rc) == 0);
    khdkhd::rc::register_resource(rc, data, sizeof(data));
    ASSERT_TRUE(khdkhd::rc::resourcesMap.count(rc) == 1);
}
TEST_F(register_resource, doesNotAlterDataSize) {
    const auto rc = "/foo";
    char data[] = "foo";
    khdkhd::rc::register_resource(rc, data, sizeof(data));
    auto rc_entry = khdkhd::rc::resourcesMap.at(rc);
    ASSERT_EQ(rc_entry.size, sizeof(data));
}
TEST_F(register_resource, doesNotAlterDataContent) {
    const auto rc = "/foo";
    char data[] = "foo";
    khdkhd::rc::register_resource(rc, data, sizeof(data));
    auto rc_entry = khdkhd::rc::resourcesMap.at(rc);
    ASSERT_STREQ(rc_entry.data, data);
}
TEST_F(register_resource, throwsWhenTwoEntriesWithSamePathAreRegistered) {
    const auto rc = "/foo";
    char data[] = "foo";
    khdkhd::rc::register_resource(rc, data, sizeof(data));
    ASSERT_THROW(
        khdkhd::rc::register_resource(rc, data, sizeof(data)),
        khdkhd::rc::Error
    );
}
//
}
