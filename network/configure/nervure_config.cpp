#include "configure/nervure_config.h"
#include "log.h"

namespace ffnet
{
NervureConfigure::NervureConfigure(String conf)
{
    try {
        boost::property_tree::ini_parser::read_ini(conf, m_oPTree);
    } catch(boost::property_tree::ini_parser_error ipe) {
		FFNET_DEBUG(log_conf("NervureConfigure", "can't read conf file: %s", conf.c_str());)
    }
}
}