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

void			ProtoBufWrapperPkg::archive(Archive &ar)
{
    //optimizing for each archiver
    Seralizer s(NULL, 0);
    Deseralizer d(NULL, 0);
    LengthRetriver l;
    if(typeid(ar) == typeid(s))
    {
        Seralizer & ts = static_cast<Seralizer &>(ar);
        archiveS(ts);
    }
    else if(typeid(ar) == typeid(d))
    {
        Deseralizer &td = static_cast<Deseralizer &>(ar);
        archiveD(td);
    }
    else
    {
        LengthRetriver &lr = static_cast<LengthRetriver &>(ar);
        archiveL(lr);
    }
}

void			ProtoBufWrapperPkg::archiveS(Seralizer & ar)
{
    assert(m_pPBMsg != NULL && "Didn't set m_pPBMsg yet!");
    ar.archive(m_strProtoBufMsgName);
    String buf(m_pPBMsg->ByteSize(), 0);
    m_pPBMsg->SerializeToString(&buf);
    ar.archive(buf);
}

void			ProtoBufWrapperPkg::archiveD(Deseralizer & ar)
{
    ar.archive(m_strProtoBufMsgName);
    m_strProtoBufMsgName.append('\0');
    String buf;
    ar.archive(buf);
    m_pPBMsg = createMessage(m_strProtoBufMsgName);
    assert(m_pPBMsg != NULL && "Can't find message in protobuf");
    m_pPBMsg->ParseFromString(buf);
}

void			ProtoBufWrapperPkg::archiveL(LengthRetriver & ar)
{
    ar.archive(m_strProtoBufMsgName);
    assert(m_pPBMsg != NULL && "Didn't set m_pPBMsg yet!");
    String buf(m_pPBMsg->ByteSize(), 0);
    ar.archive(buf);
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