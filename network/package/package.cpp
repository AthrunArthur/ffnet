#include "package/package.h"

#include "nervures/typed_net_nervure.h"

namespace ffnet
{
Package::Package(uint32_t typeID)
    : m_iTypeID(typeID){}

Package::~Package()
{

}

}//end namespace ffnet