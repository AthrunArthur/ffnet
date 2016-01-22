#pragma once
#include "common.h"
#include "common/archive.h"


namespace ffnet
{
/*******************
 * You should declare your own package by deriving from this class.
 * Basically, you need to do two things,
 * 1. to specify the typeID, which should be unique;
 * 2. to implement the virtual function, archive.
 *    archive is used to serialize and deserialize the Package, and
 * a typical implementation is like this,
 *    virtual void archive(archive & ar)
 *    {
 *          ar.archive(m_strName);
 *    }
 *  You can find me examples about archive.
 */
class package
{
protected:
    virtual void        archive(marshaler & ar) = 0;

public:
    package(uint32_t typeID)
        : m_iTypeID(typeID) {}

    virtual ~package() {};

    uint32_t        type_id() const {
        return m_iTypeID;
    }
    uint32_t     &    type_id() {return m_iTypeID;}

    void        arch(marshaler &ar) {
        ar.archive(m_iTypeID);
        archive(ar);
    }

protected:
    uint32_t                            m_iTypeID;
};//end class Package

typedef std::shared_ptr<package>    package_ptr;

}//end namespace ffnet

