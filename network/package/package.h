#ifndef _NETWORK_PACKAGE_PACKAGE_H_
#define _NETWORK_PACKAGE_PACKAGE_H_
#include "common.h"
#include "common/archive.h"

#define ARCHIVE_PACKAGE(pkg_type) 	\
virtual	void		archiveLength(ffnet::LengthRetriver &lr) \
	{	\
		Package::archiveLength(lr); \
		archive(lr);	\
	}	\
	virtual void		archiveSerialize(ffnet::Seralizer &s)	\
	{	\
		Package::archiveSerialize(s); \
		archive(s);	\
	}	\
	virtual void		archiveDeserialize(ffnet::Deseralizer & d)	\
	{	\
		Package::archiveDeserialize(d); \
		archive(d);	\
	}	\
	virtual	boost::shared_ptr<Package >		createInstance() \
	{	\
		return boost::shared_ptr<Package>(new pkg_type()); \
	}
	

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
	/******************************************
	The following methods should not be modified!
	*************************/
    friend class BonderSplitter;
	friend class LengthBonderSplitter;
	friend class ffnet::details::TypedNetNervure;
	
    virtual	void		archiveLength(ffnet::LengthRetriver &lr) {
        archive(lr);
    }
    virtual void		archiveSerialize(ffnet::Seralizer &s) {
        archive(s);
    }
    virtual void		archiveDeserialize(ffnet::Deseralizer &d) {
        archive(d);
    }
    virtual boost::shared_ptr<Package> 	createInstance()
	{
		return boost::shared_ptr<Package>();
	}

    template <class Ar_t>
    void			archive(Ar_t &ar) {
        ar.archive(m_iTypeID);
    }
public:
    Package(uint32_t typeID);

    virtual ~Package();

    virtual uint32_t		getTypeID() const {
        return m_iTypeID;
    }

protected:
    boost::shared_array<char>		m_oBuffer;
    uint32_t							m_iTypeID;
};//end class Package
typedef boost::shared_ptr<Package>	PackagePtr_t;
}//end namespace ffnet

#endif