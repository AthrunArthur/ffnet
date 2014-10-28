//#define PROTO_BUF_SUPPORT
#ifdef PROTO_BUF_SUPPORT
#include "package/proto_buf_wrapper_pkg.h"
#include "common.h"
namespace ffnet
{
ProtoBufWrapperPkg::ProtoBufWrapperPkg()
    : Package(0)
{

}
ProtoBufWrapperPkg::ProtoBufWrapperPkg(String strPBMessageName)
    : Package(0)
    , m_strProtoBufMsgName(strPBMessageName)
{
}

ProtoBufWrapperPkg::ProtoBufWrapperPkg(boost::shared_ptr< Message > pMsg)
    : Package(0)
    , m_strProtoBufMsgName(pMsg->GetDescriptor()->full_name())
    , m_pPBMsg(pMsg)
{

}

boost::shared_ptr< Message > ProtoBufWrapperPkg::PBMessage() const
{
    return m_pPBMsg;
}

void            ProtoBufWrapperPkg::archive(Archive &ar)
{
    //optimizing for each archiver
    ar.archive(m_strProtoBufMsgName);
    
    switch(ar.getArTy())
    {
        case Archive::deseralizer:
        {
            String buf;
            ar.archive(buf);
            m_pPBMsg = createMessage(m_strProtoBufMsgName);
            assert(m_pPBMsg != NULL && "Can't find message in protobuf");
            m_pPBMsg->ParseFromString(buf);
            break;
        }
        case Archive::seralizer:
        {
            String buf(m_pPBMsg->ByteSize(), 0);
            m_pPBMsg->SerializeToString(&buf);
            ar.archive(buf);
        }
        case Archive::length_retriver:
        {
            assert(m_pPBMsg != NULL && "Didn't set m_pPBMsg yet!");
            String buf(m_pPBMsg->ByteSize(), 0);
            ar.archive(buf);
        }
    }
}

ProtoBufWrapperPkg::MessagePtr_t ProtoBufWrapperPkg::createMessage(const std::string& typeName)
{
    MessagePtr_t message;
    const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (descriptor)
    {
        const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = MessagePtr_t(prototype->New());
        }
    }
    return message;
}



}//end namespace ffnet

#endif