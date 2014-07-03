#ifndef FFNET_COMMON_LOG_LOG_WRITER_H_
#define FFNET_COMMON_LOG_LOG_WRITER_H_
#include "common/singleton.h"
#include "common/blocking_queue.h"
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <boost/atomic.hpp>

namespace ffnet
{
namespace details
{
class LogWriter : boost::noncopyable
{
public:
    LogWriter();

    virtual ~LogWriter();
    BlockingQueue<String> &	queue() {
        return m_oQueue;
    }
    void			run(const char * filePath);
protected:
    void			actualRun();
protected:
    BlockingQueue<String>		m_oQueue;
    boost::shared_ptr<boost::thread>		m_oIOThread;
    String					m_strFilePath;
    std::ofstream				m_oFile;
    boost::atomic<bool>                         m_bRunning;
};//end class LogWriter

}//end namespace details;
}//end namespace ffnet
#endif