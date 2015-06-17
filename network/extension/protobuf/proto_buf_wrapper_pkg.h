#pragma once

#include "common.h"
#include "middleware/package.h"
#include "common/archive.h"

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace ffnet {
    using google::protobuf::DescriptorPool;
    using google::protobuf::MessageFactory;
    using google::protobuf::Message;
    using google::protobuf::Descriptor;


    class protobuf_wrapper_pkg : public package {
    public:
        protobuf_wrapper_pkg();

        protobuf_wrapper_pkg(String strPBMessageName);

        protobuf_wrapper_pkg(boost::shared_ptr<google::protobuf::Message> pMsg);

        boost::shared_ptr<Message> protobuf_message() const;

        virtual void archive(marshaler &ar);

    protected:
        typedef boost::shared_ptr<Message> message_ptr;
        message_ptr create_message(const std::string &typeName);

    protected:
        String m_strProtoBufMsgName;
        message_ptr m_pPBMsg;
        //char *        m_pProtoBufMsg;
    };//end class ProtoBufWrapperPkg
}//end namespace ffnet
