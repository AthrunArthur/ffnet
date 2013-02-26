#ifndef _NETWORK_PACKAGE_PACKAGE_H_
#define _NETWORK_PACKAGE_PACKAGE_H_
#include "common.h"
#include "common/archive.h"


namespace ffnet
{
namespace details
{
class TypedNetNervure;
}
class BonderSplitter;
class LengthBonderSplitter;

/*******************
 * TODO(A.A), we should describe the useage of Package here!
 */
class Package
{
protected:

    virtual void		archive(Archive & ar) = 0;



public:
    Package(uint32_t typeID)
        : m_iTypeID(typeID) {}

    virtual ~Package() {};

    uint32_t		getTypeID() const {
        return m_iTypeID;
    }
    uint32_t	 &	getTypeID() {return m_iTypeID;}
    
    void		arch(Archive &ar) {
        ar.archive(m_iTypeID);
        archive(ar);
    }

protected:
    uint32_t							m_iTypeID;
};//end class Package

typedef boost::shared_ptr<Package>	PackagePtr_t;

}//end namespace ffnet

#endif