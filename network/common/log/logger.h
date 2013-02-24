#ifndef FFNET_COMMON_LOG_LOGGER_H_
#define FFNET_COMMON_LOG_LOGGER_H_
#include <boost/noncopyable.hpp>
#include <string.h>
#include "common/log/logwriter.h"
#include "common/singleton.h"
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace ffnet
{
namespace details
{

const int kSmallBuffer = 4*1024;
const int kLargeBuffer = 4000*1000;

template<int SIZE>
class FixedBuffer : boost::noncopyable
{
public:
    FixedBuffer()
        : cur_(data_)
    {
        setCookie(cookieStart);
    }

    ~FixedBuffer()
    {
        setCookie(cookieEnd);
    }

    void append(const char* /*restrict*/ buf, size_t len)
    {
        // FIXME: append partially
        if (static_cast<size_t>(avail()) > len)
        {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char* data() const {
        return data_;
    }
    int length() const {
        return static_cast<int>(cur_ - data_);
    }

    // write to data_ directly
    char* current() {
        return cur_;
    }
    int avail() const {
        return static_cast<int>(end() - cur_);
    }
    void add(size_t len) {
        cur_ += len;
    }

    void reset() {
        cur_ = data_;
    }
    void bzero() {
        ::bzero(data_, sizeof data_);
    }

    // for used by GDB
    const char* debugString();
    void setCookie(void (*cookie)()) {
        cookie_ = cookie;
    }
    // for used by unit test
    String asString() const {
        return String(data_, length());
    }

private:
    const char* end() const {
        return data_ + sizeof data_;
    }
    // Must be outline function for cookies.
    static void cookieStart();
    static void cookieEnd();

    void (*cookie_)();
    char data_[SIZE];
    char* cur_;
};

template<class T>
struct enable_traits{
	static const bool value = false;
};//end enable_traits
}//end namespace details

static const char digits[] = "9876543210123456789";
static const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);
static const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

using ::ffnet::details::LogWriter;
using namespace ::ffnet::details;
template<bool EnableLogFlag>
class Logger {
protected:
    void staticCheck()
    {
        BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10);
        BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
        BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
        BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10);
    }

    template<typename T>
    void formatInteger(T v)
    {
        if (buffer_.avail() >= kMaxNumericSize)
        {
            size_t len = convert(buffer_.current(), v);
            buffer_.add(len);
        }
    }

    FixedBuffer<kSmallBuffer> buffer_;

    static const int kMaxNumericSize = 32;
public:
    virtual ~Logger()
    {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        String s = boost::posix_time::to_simple_string(now) + buffer_.asString();
		//std::cout<<s<<std::endl;
        Singleton<LogWriter>::instance_ref().queue().push_back(s);
    }
public:
    typedef Logger<EnableLogFlag> self;
	
	template<typename T>
	self& operator<<(T v)
	{
		BOOST_STATIC_ASSERT(boost::is_integral<T>::value);
		BOOST_STATIC_ASSERT((boost::is_same<bool, T>::value) == false);
		BOOST_STATIC_ASSERT((boost::is_same<char, T>::value) == false);
		
		formatInteger(v);
		return * this;
	}
	template<typename T>
	self& operator<<(T * p)
	{
		uintptr_t v = reinterpret_cast<uintptr_t>(p);
        if (buffer_.avail() >= kMaxNumericSize)
        {
            char* buf = buffer_.current();
            buf[0] = '0';
            buf[1] = 'x';
            size_t len = convertHex(buf+2, v);
            buffer_.add(len+2);
        }
        return *this;
	}
	
    self & operator<<(bool v)
    {
        buffer_.append(v ? "1": "0", 1);
        return *this;
    }
    self& operator<<(char v)
    {
        buffer_.append(&v, 1);
        return *this;
    }


    self& operator<<(const char* p)
    {
        buffer_.append(p, strlen(p));
        return *this;
    }

    self& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    self& operator<<(double v)
    {
        if (buffer_.avail() >= kMaxNumericSize)
        {
            int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
            buffer_.add(len);
        }
        return *this;
    }
    // self& operator<<(long double);
    self& operator<<(const String& v)
    {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }


protected:
	
    template<typename T>
    static size_t convert(char buf[], T value)
    {
        T i = value;
        char* p = buf;

        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    static size_t convertHex(char buf[], uintptr_t value)
    {
        uintptr_t i = value;
        char* p = buf;

        do
        {
            int lsd = i % 16;
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }
};

template<>
class Logger<false>{
public:
    typedef Logger<false> self;
	template<typename T>
    self& operator<<(T v) {
        return *this;
    }
    template<typename T>
    self& operator<< (T *v){
		return *this;
	}
};

}//end namespace ffnet

#endif