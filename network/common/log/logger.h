#ifndef FFNET_COMMON_LOG_LOGGER_H_
#define FFNET_COMMON_LOG_LOGGER_H_
#include <boost/noncopyable.hpp>
#include <string.h>
#include "common/log/logwriter.h"
#include "common/singleton.h"
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

  const char* data() const { return data_; }
  int length() const { return static_cast<int>(cur_ - data_); }

  // write to data_ directly
  char* current() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }
  void bzero() { ::bzero(data_, sizeof data_); }

  // for used by GDB
  const char* debugString();
  void setCookie(void (*cookie)()) { cookie_ = cookie; }
  // for used by unit test
  String asString() const { return String(data_, length()); }

 private:
  const char* end() const { return data_ + sizeof data_; }
  // Must be outline function for cookies.
  static void cookieStart();
  static void cookieEnd();

  void (*cookie_)();
  char data_[SIZE];
  char* cur_;
};

class LoggerDoLog{
protected:
  void staticCheck();

  template<typename T>
  void formatInteger(T);

  FixedBuffer<kSmallBuffer> buffer_;

  static const int kMaxNumericSize = 32;
public:
	virtual ~LoggerDoLog()
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		String s = boost::posix_time::to_simple_string(now) + buffer_.asString();
		
		Singleton<LogWriter>::instance_ref().queue().push_back(s);
	}
protected:
	virtual const char * getModuleName() = 0;
public:
	typedef LoggerDoLog self;
  self& operator<<(bool v)
  {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  self& operator<<(short);
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);

  self& operator<<(const void*);

  self& operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }
  self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
    buffer_.append(&v, 1);
    return *this;
  }

  // self& operator<<(signed char);
  // self& operator<<(unsigned char);

  self& operator<<(const char* v)
  {
    buffer_.append(v, strlen(v));
    return *this;
  }

  self& operator<<(const String& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  
  
private:
    void to_simple_string(boost::posix_time::ptime now);
};

class LoggerDoNothing{
public:
	typedef LoggerDoNothing self;
  inline self& operator<<(bool v){return *this;}  

  inline self& operator<<(short){return *this;}
  inline self& operator<<(unsigned short){return *this;}
  inline self& operator<<(int){return *this;}
  inline self& operator<<(unsigned int){return *this;}
  inline self& operator<<(long){return *this;}
  inline self& operator<<(unsigned long){return *this;}
  inline self& operator<<(long long){return *this;}
  inline self& operator<<(unsigned long long){return *this;}

  inline self& operator<<(const void*){return *this;}

  inline self& operator<<(float v){return *this;}
  inline self& operator<<(double){return *this;}
  // self& operator<<(long double);

  inline self& operator<<(char v){return *this;}

  inline self& operator<<(const char* v){return *this;}

  inline self& operator<<(const String& v){return *this;}

  
};

	}//end namespace details
}//end namespace ffnet

#endif