#include "framework/nervure_config.h"
#include "common/defines.h"

namespace ffnet
{
NervureConfigure::NervureConfigure(String conf)
{
    try {
        boost::property_tree::ini_parser::read_ini(conf, m_oPTree);
    } catch(boost::property_tree::ini_parser_error ipe) {
			LOG_ERROR(conf)<<"NervureConfigure "<< "can't read conf file: " << conf;
    }
}
}