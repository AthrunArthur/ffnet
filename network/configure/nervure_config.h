#ifndef _NETWORK_CONFIGURE_NERVURE_CONFIG_H_
#define _NETWORK_CONFIGURE_NERVURE_CONFIG_H_
#include "common.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace ffnet
{
class NervureConfigure
{
public:
    NervureConfigure(String conf);

    template< class Ty_>
    Ty_		get(String str) const {
        return m_oPTree.get<Ty_>(str);
    }
protected:
    boost::property_tree::ptree		m_oPTree;

};//end class NervureConfigure
typedef boost::shared_ptr<NervureConfigure> NervureConfigurePtr_t;
}//end namespace ffnet
#endif

