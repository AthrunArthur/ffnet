#ifndef _NETWORK_PACKAGE_PACKAGE_H_
#define _NETWORK_PACKAGE_PACKAGE_H_
#include "common.h"
#include "common/archive.h"


namespace ffnet
{
class BonderSplitter;
class LengthBonderSplitter;

/*******************
 * You should declare your own package by deriving from this class.
 * Basically, you need to do two things,
 * 1. to specify the typeID, which should be unique;
 * 2. to implement the virtual function, archive.
 *    archive is used to serialize and deserialize the Package, and
 * a typical implementation is like this,
 *    virtual void archive(Archive & ar)
 *    {
 *          ar.archive(m_strName);
 *    }
 *  You can find me examples about Archive.
 */
class Package
{
protected:
    virtual void        archive(Archive & ar) = 0;
    
public:
    Package(uint32_t typeID)
        : m_iTypeID(typeID) {}

    virtual ~Package() {};

    uint32_t        getTypeID() const {
        return m_iTypeID;
    }
    uint32_t     &    getTypeID() {return m_iTypeID;}
    
    void        arch(Archive &ar) {
        ar.archive(m_iTypeID);
        archive(ar);
    }

protected:
    uint32_t                            m_iTypeID;
};//end class Package

typedef boost::shared_ptr<Package>    PackagePtr_t;

}//end namespace ffnet

#endif