#define BOOST_TEST_MODULE TEST_ARCHIVE
#include <boost/test/unit_test.hpp>
#include <network.h>

struct Event1
{
    const static size_t identifier = 1;
    typedef boost::function<void (int, int)> Handler_t;
};

struct Event2
{
    const static size_t identifier = 2;
    typedef boost::function<void (int, std::string) > Handler_t;
};

void check_event1(int * counter, int v1, int v2)
{
    (*counter) ++;
    BOOST_CHECK(v1 + v2 == 10);
}

void check_event2(int & counter, int v, std::string s)
{
    counter ++;
    BOOST_CHECK(v == s.size());
}

BOOST_AUTO_TEST_SUITE(minimal_test)

BOOST_AUTO_TEST_CASE(simple_test)
{
    ffnet::EventHandler eh;
    int c1 = 0, c2 = 0;
    eh.listen<Event1>(boost::bind(check_event1,&c1, _1, _2));
    eh.listen<Event2>(boost::bind(check_event2, boost::ref(c2), _1, _2));

    for(int i=0;i < 10; i++)
    {
        eh.triger<Event1>(i, 10 -i);
        std::string s = "Hello world";
        eh.triger<Event2>(s.size(), s);
    }
    BOOST_CHECK_MESSAGE(c1 == 10, "c1 is "<<c1);
    BOOST_CHECK(c2 == 10);
}
BOOST_AUTO_TEST_SUITE_END()
