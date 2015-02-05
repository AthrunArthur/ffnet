 
#define BOOST_TEST_MODULE TEST_ARCHIVE
#include <boost/test/unit_test.hpp>
#include <network.h>



#define BUF_LEN 1024

#define ARRAY_LEN 16

BOOST_AUTO_TEST_SUITE(minimal_test)

BOOST_AUTO_TEST_CASE(simple_test)
{
    char buf[BUF_LEN];
    std::memset(buf, 0, BUF_LEN);
    ffnet::Archive s(buf, BUF_LEN, ffnet::Archive::seralizer);
    ffnet::Archive d(buf, BUF_LEN, ffnet::Archive::deseralizer);
    ffnet::Archive l(ffnet::Archive::length_retriver);

    int int_data = 100;
    double double_data = 11.2;
    std::string str_data = "hello world";
    s.archive(int_data);
    s.archive(double_data);
    s.archive(str_data);

    int int_res;
    double double_res;
    std::string str_res;
    d.archive(int_res);
    d.archive(double_res);
    d.archive(str_res);

    std::cout<<"input:"<<str_data<<", output:"<<str_res<<std::endl;
    BOOST_CHECK(int_data == int_res);
    BOOST_CHECK(double_data == double_res);
    BOOST_CHECK(str_data == str_res);
}

BOOST_AUTO_TEST_CASE(array_test)
{
    char buf[BUF_LEN];
    std::memset(buf, 0, BUF_LEN);
    ffnet::Archive s(buf, BUF_LEN, ffnet::Archive::seralizer);
    ffnet::Archive d(buf, BUF_LEN, ffnet::Archive::deseralizer);

    int int_data[ARRAY_LEN];
    double double_data[ARRAY_LEN];
    std::string str_data[ARRAY_LEN];
    for(int i = 0; i < ARRAY_LEN; i++)
    {
        int_data[i] = i;
        double_data[i] = i * 3.14;
        std::stringstream ss;
        ss<<"hello : "<< i;
        str_data[i] =  ss.str();
    }
    s.archive(int_data);
    s.archive(double_data);
    s.archive(str_data);

    int int_res[ARRAY_LEN];
    double double_res[ARRAY_LEN];
    std::string str_res[ARRAY_LEN];

    d.archive(int_res);
    d.archive(double_res);
    d.archive(str_res);

    for(int i = 0; i < ARRAY_LEN; i++)
    {
        BOOST_CHECK(int_res[i] == i);
        BOOST_CHECK(double_res[i] == i*3.14);
        BOOST_CHECK(str_res[i] == str_data[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()
