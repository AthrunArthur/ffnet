#include "common/log/logwriter.h"

namespace ffnet
{
namespace details {
LogWriter::LogWriter()
    : m_strFilePath()
    , m_bRunning(true)
{

}
void LogWriter::run(const char * filePath)
{
    m_strFilePath = String(filePath);
    if(m_oIOThread)
        return;
    m_oIOThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&LogWriter::actualRun, this)));
}

void LogWriter::actualRun()
{
    String str;
    m_oFile.open(m_strFilePath.c_str() );

    while(m_bRunning || !m_oQueue.empty())
    {
        while(!m_oQueue.empty())
        {
            m_oQueue.pop(str);
            m_oFile<<str<<std::endl;
        }
        m_oFile.flush();
        m_oQueue.pop(str);
        m_oFile<<str<<std::endl;
    }
    m_oFile.close();
}

LogWriter::~LogWriter()
{
    m_bRunning = false;
    m_oQueue.push_back("End and quit log!");

    m_oIOThread->join();
}

}//end namespace details;
}//end namespace ffnet