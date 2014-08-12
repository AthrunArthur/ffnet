#include "common/log/logger.h"
#include <boost/static_assert.hpp>
namespace ffnet
{
    namespace details{
    
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

    }//end namespace details
}//end namespace ffnet