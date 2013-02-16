#include "common/log/logger.h"
#include <boost/static_assert.hpp>
namespace ffnet
{
	namespace details{
		
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

// Efficient Integer to String Conversions, by Matthew Wilson.
template<typename T>
size_t convert(char buf[], T value)
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

size_t convertHex(char buf[], uintptr_t value)
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


template<int SIZE>
const char* FixedBuffer<SIZE>::debugString()
{
  *cur_ = '\0';
  return data_;
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieStart()
{
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd()
{
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

void LoggerDoLog::staticCheck()
{
  BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10);
  BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
  BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
  BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10);
}

template<typename T>
void LoggerDoLog::formatInteger(T v)
{
  if (buffer_.avail() >= kMaxNumericSize)
  {
    size_t len = convert(buffer_.current(), v);
    buffer_.add(len);
  }
}

LoggerDoLog& LoggerDoLog::operator<<(short v)
{
  *this << static_cast<int>(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(unsigned short v)
{
  *this << static_cast<unsigned int>(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(int v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(unsigned int v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(long v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(unsigned long v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(long long v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(unsigned long long v)
{
  formatInteger(v);
  return *this;
}

LoggerDoLog& LoggerDoLog::operator<<(const void* p)
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

// FIXME: replace this with Grisu3 by Florian Loitsch.
LoggerDoLog& LoggerDoLog::operator<<(double v)
{
  if (buffer_.avail() >= kMaxNumericSize)
  {
    int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
    buffer_.add(len);
  }
  return *this;
}

	}//end namespace details
}//end namespace ffnet