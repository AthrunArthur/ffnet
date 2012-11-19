#ifdef PROTO_BUF_SUPPORT
#ifndef FFNET_PACKAGE_PROTO_BUF_WRAPPER_PKG_H_
#define FFNET_PACKAGE_PROTO_BUF_WRAPPER_PKG_H_

#include "package/package.h"
#include "archive/deseralizer.h"
#include "archive/serializer.h"
#include "archive/length_retriver.h"
#include "common.h"

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace ffnet
{
using google::protobuf::DescriptorPool;
using google::protobuf::MessageFactory;
using google::protobuf::Message;
using google::protobuf::Descriptor;
class ProtoBufWrapperPkg : public Package
{
protected:
	typedef boost::shared_ptr<Message> MessagePtr_t;
public:
    ProtoBufWrapperPkg();
    ProtoBufWrapperPkg(String strPBMessageName);
    ProtoBufWrapperPkg(boost::shared_ptr<google::protobuf::Message>  pMsg);

	boost::shared_ptr<Message> PBMessage() const;
	
    virtual void			archive(Archive &ar);
protected:
	
    void			archiveS(Seralizer & ar);

    void			archiveD(Deseralizer & ar);
    
    void			archiveL(LengthRetriver & ar);
	
    MessagePtr_t 	createMessage(const std::string& typeName);
	
protected:
    String 		m_strProtoBufMsgName;
    MessagePtr_t  m_pPBMsg;
    //char *		m_pProtoBufMsg;
};//end class ProtoBufWrapperPkg
}//end namespace ffnet
#endif//end FFNET_PACKAGE_PROTO_BUF_WRAPPER_PKG_H_

#endif //end PROTO_BUF_SUPPORT