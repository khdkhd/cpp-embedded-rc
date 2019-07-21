#include <rc.hpp>
#include <gtest/gtest.h>

namespace khdkhd::rc {
extern std::map<std::string, RCEntry> resourcesMap;
}

namespace {
const khdkhd::rc::streambuf::pos_type Failure = khdkhd::rc::streambuf::pos_type(khdkhd::rc::streambuf::off_type(-1));

char data[] = {
//   1    2    3    4    5    6    7    8
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
//   9    0   11   12   13   14   15   16
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
//  17   18   19   20   21   22   23   24
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
//  25   26
    'y', 'z'
};

class streambuf : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Register resources
        khdkhd::rc::resourcesMap.clear();
        khdkhd::rc::register_resource("/data", data, sizeof(data));
    }
};

// streambuf::open
TEST_F(streambuf, open) {
    khdkhd::rc::streambuf rcbuf;

    // open an existing file
    ASSERT_TRUE(rcbuf.open("/data").is_open());

    // open a non existing file
    ASSERT_FALSE(rcbuf.open("/atad").is_open());
}

// streambuf::sgetc
TEST_F(streambuf, sgetc) {
    khdkhd::rc::streambuf rcbuf;

    ASSERT_TRUE(rcbuf.open("/data").is_open());
    ASSERT_EQ(rcbuf.sgetc(), 'a');
}

// streambuf::sgetn
TEST_F(streambuf, sgetn) {
    khdkhd::rc::streambuf rcbuf;
    char buf[5] = {0};

    ASSERT_TRUE(rcbuf.open("/data").is_open());

    ASSERT_EQ(rcbuf.sgetn(buf, 4), 4);
    ASSERT_STREQ(buf, "abcd");

    ASSERT_EQ(rcbuf.sgetn(buf, 4), 4);
    ASSERT_STREQ(buf, "efgh");
}

// streambuf::seekoff
TEST_F(streambuf, seekoff) {
    using std::ios_base;

    khdkhd::rc::streambuf rcbuf;

    ///////////////////////////////////////////////////////////////////////////
    // seekoff from begining
    rcbuf.open("/data").is_open();

    // - should correctly set the position
    ASSERT_EQ(rcbuf.pubseekoff(4, ios_base::beg), 4);
    ASSERT_EQ(rcbuf.in_avail(), sizeof(data) - 4);

    ASSERT_EQ(rcbuf.pubseekoff(sizeof(data), ios_base::beg), sizeof(data));
    ASSERT_EQ(rcbuf.in_avail(), 0);

    // - should fail if the repositioning would move the pointer to before the
    // beginning of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(-10, ios_base::beg), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 0);

    // - should fail if the repositioning would move the pointer to after the
    // end of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(sizeof(data) + 1, ios_base::beg), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 0);

    ///////////////////////////////////////////////////////////////////////////
    // seekoff from end
    rcbuf.open("/data").is_open();

    // - should correctly set the position
    ASSERT_EQ(rcbuf.pubseekoff(-4, ios_base::end), sizeof(data) - 4);
    ASSERT_EQ(rcbuf.in_avail(), 4);

    ASSERT_EQ(rcbuf.pubseekoff(0, ios_base::end), sizeof(data));
    ASSERT_EQ(rcbuf.in_avail(), 0);

    // - should fail if the repositioning would move the pointer to before the
    // beginning of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(-(sizeof(data) + 1), ios_base::end), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 0);

    // - should fail if the repositioning would move the pointer to after the
    // end of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(1, ios_base::end), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 0);

    ///////////////////////////////////////////////////////////////////////////
    // seekoff from cur
    rcbuf.open("/data").is_open();

    // - should correctly set the position
    ASSERT_EQ(rcbuf.pubseekoff(4, ios_base::cur), 4);
    ASSERT_EQ(rcbuf.in_avail(), sizeof(data) - 4);

    ASSERT_EQ(rcbuf.pubseekoff(sizeof(data) - 4, ios_base::cur), sizeof(data));
    ASSERT_EQ(rcbuf.in_avail(), 0);

    ASSERT_EQ(rcbuf.pubseekoff(-4, ios_base::cur), sizeof(data) - 4);
    ASSERT_EQ(rcbuf.in_avail(), 4);

    // - should fail if the repositioning would move the pointer to before the
    // beginning of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(-sizeof(data), ios_base::cur), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 4);

    // - should fail if the repositioning would move the pointer to after the
    // end of the buffer
    ASSERT_EQ(rcbuf.pubseekoff(5, ios_base::cur), Failure);
    ASSERT_EQ(rcbuf.in_avail(), 4);
}
}
