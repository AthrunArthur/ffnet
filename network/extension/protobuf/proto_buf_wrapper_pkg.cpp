#include "extension/protobuf/proto_buf_wrapper_pkg.h"
#include "common.h"
#include "extension/package_types.h"

namespace ffnet
{
protobuf_wrapper_pkg::protobuf_wrapper_pkg()
    : package(protobuf_wrapper_pkg_type)
{

}
protobuf_wrapper_pkg::protobuf_wrapper_pkg(String strPBMessageName)
    : package(protobuf_wrapper_pkg_type)
    , m_strProtoBufMsgName(strPBMessageName)
{
}

protobuf_wrapper_pkg::protobuf_wrapper_pkg(boost::shared_ptr< Message > pMsg)
    : package(protobuf_wrapper_pkg_type)
    , m_strProtoBufMsgName(pMsg->GetDescriptor()->full_name())
    , m_pPBMsg(pMsg)
{

}

boost::shared_ptr< Message > protobuf_wrapper_pkg::protobuf_message() const
{
    return m_pPBMsg;
}

void            protobuf_wrapper_pkg::archive(marshaler &ar)
{
    //optimizing for each archiver
    ar.archive(m_strProtoBufMsgName);
    
    switch(ar.get_marshaler_type())
    {
        case marshaler::deseralizer:
        {
            String buf;
            ar.archive(buf);
            m_pPBMsg = create_message(m_strProtoBufMsgName);
            assert(m_pPBMsg != NULL && "Can't find message in protobuf");
            m_pPBMsg->ParseFromString(buf);
            break;
        }
        case marshaler::seralizer:
        {
            String buf(m_pPBMsg->ByteSize(), 0);
            m_pPBMsg->SerializeToString(&buf);
            ar.archive(buf);
        }
        case marshaler::length_retriver:
        {
            assert(m_pPBMsg != NULL && "Didn't set m_pPBMsg yet!");
            String buf(m_pPBMsg->ByteSize(), 0);
            ar.archive(buf);
        }
    }
}

    protobuf_wrapper_pkg::message_ptr protobuf_wrapper_pkg::create_message(const std::string& typeName)
{
    message_ptr message;
    const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (descriptor)
    {
        const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = message_ptr(prototype->New());
        }
    }
    return message;
}



}//end namespace ffnet
