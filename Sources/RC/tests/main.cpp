#include <rc.hpp>
#include <gtest/gtest.h>

namespace khdkhd::rc {
extern std::map<std::string, RCEntry> resourcesMap;
}

namespace {
// Register a resource
TEST(registerResource, addResourceToRegistry) {
    const auto rc = "/foo";
    char data[] = "foo";
    ASSERT_TRUE(khdkhd::rc::resourcesMap.count(rc) == 0);
    khdkhd::rc::registerResource(rc, data, sizeof(data));
    ASSERT_TRUE(khdkhd::rc::resourcesMap.count(rc) == 1);
}
TEST(registerResource, doesNotAlterDataSize) {
    const auto rc = "/bar";
    char data[] = "bar";
    khdkhd::rc::registerResource(rc, data, sizeof(data));
    auto rc_entry = khdkhd::rc::resourcesMap.at(rc);
    ASSERT_EQ(rc_entry.size, sizeof(data));
}
TEST(registerResource, doesNotAlterDataContent) {
    const auto rc = "/gee";
    char data[] = "gee";
    khdkhd::rc::registerResource(rc, data, sizeof(data));
    auto rc_entry = khdkhd::rc::resourcesMap.at(rc);
    ASSERT_STREQ(rc_entry.data, data);
}
TEST(registerResource, throwsWhenTwoEntriesWithSamePathAreRegistered) {
    const auto rc = "/buzz";
    char data[] = "buzz";
    khdkhd::rc::registerResource(rc, data, sizeof(data));
    ASSERT_THROW(
        khdkhd::rc::registerResource(rc, data, sizeof(data)),
        khdkhd::rc::Error
    );
}
//
}

int
main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
