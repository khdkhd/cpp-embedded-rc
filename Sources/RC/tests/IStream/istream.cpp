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

class istream : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Register resources
        khdkhd::rc::resourcesMap.clear();
        khdkhd::rc::register_resource("/data", data, sizeof(data));
    }
};

// istream::istream
TEST_F(istream, istream) {
    // default
    khdkhd::rc::istream stream1;
    ASSERT_FALSE(stream1.is_open());

    // open constructor with existing file
    khdkhd::rc::istream stream2("/data");
    ASSERT_TRUE(stream2.is_open());

    // open constructor with not existing file
    khdkhd::rc::istream stream3("/atad");
    ASSERT_FALSE(stream3.is_open());
}

// istream::open
TEST_F(istream, open) {
    khdkhd::rc::istream stream;

    ASSERT_TRUE(stream.open("/data").is_open());
    ASSERT_TRUE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.bad());
    ASSERT_FALSE(stream.fail());

    ASSERT_FALSE(stream.open("/atad").is_open());
    ASSERT_FALSE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.bad());
    ASSERT_TRUE(stream.fail());
}

// istream::get
TEST_F(istream, get) {
    khdkhd::rc::istream stream;
    khdkhd::rc::istream::char_type c;
    khdkhd::rc::istream::char_type buf[5] = {0};

    stream.open("/data");

    ASSERT_EQ(stream.get(), 'a');

    ASSERT_TRUE(stream.get(c).good());
    ASSERT_EQ(c, 'b');

    ASSERT_TRUE(stream.get(buf, 5).good());
    ASSERT_EQ(stream.gcount(), 4);
    ASSERT_STREQ(buf, "cdef");
}

// istream::read
TEST_F(istream, read) {
    khdkhd::rc::istream stream;
    khdkhd::rc::istream::char_type buf[sizeof(data)+1] = {0};

    stream.open("/data");

    stream.read(buf, sizeof(data));
    ASSERT_TRUE(stream.good());
    ASSERT_EQ(stream.gcount(), sizeof(data));
    ASSERT_STREQ(buf, data);

    stream.read(buf, 1);
    ASSERT_FALSE(stream.good());
    ASSERT_TRUE(stream.fail());
    ASSERT_TRUE(stream.eof());
}

// istream::tellg
TEST_F(istream, tellg) {
    khdkhd::rc::istream stream;
    khdkhd::rc::istream::char_type buf[4];

    stream.open("/data");
    ASSERT_EQ(stream.read(buf, sizeof(buf)).tellg(), sizeof(buf));
    ASSERT_EQ(stream.read(buf, sizeof(buf)).tellg(), 2*sizeof(buf));
}

// istream::tellg
TEST_F(istream, seekg) {
    using khdkhd::rc::istream;

    istream stream;



    ///////////////////////////////////////////////////////////////////////////
    // istream::tellg(pos)
    stream.open("/data");

    ASSERT_TRUE(stream.seekg(istream::pos_type(4)).good());
    ASSERT_EQ(stream.tellg(), 4);

    ASSERT_TRUE(stream.seekg(istream::pos_type(sizeof(data))).good());
    ASSERT_EQ(stream.tellg(), sizeof(data));

    ASSERT_TRUE(stream.seekg(istream::pos_type(sizeof(data) + 1)).fail());
    ASSERT_EQ(stream.tellg(), Failure);

    ///////////////////////////////////////////////////////////////////////////
    // istream::tellg(off, dir)

    stream.open("/data");
    stream.seekg(4, istream::beg);
    ASSERT_TRUE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.fail());
    ASSERT_EQ(stream.tellg(), 4);

    stream.clear();
    stream.seekg(-4, istream::beg);
    ASSERT_FALSE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_TRUE(stream.fail());
    ASSERT_EQ(stream.tellg(), Failure);

    stream.clear();
    stream.seekg(sizeof(data), istream::beg);
    ASSERT_TRUE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.fail());
    ASSERT_EQ(stream.tellg(), sizeof(data));

    stream.clear();
    stream.seekg(sizeof(data) + 1, istream::beg);
    ASSERT_FALSE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_TRUE(stream.fail());
    ASSERT_EQ(stream.tellg(), Failure);

    stream.open("/data");
    stream.seekg(4, istream::cur);
    ASSERT_TRUE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.fail());
    ASSERT_EQ(stream.tellg(), 4);

    stream.clear();
    stream.seekg(-4, istream::cur);
    ASSERT_TRUE(stream.good());
    ASSERT_FALSE(stream.eof());
    ASSERT_FALSE(stream.fail());
    ASSERT_EQ(stream.tellg(), 0);
}

}
