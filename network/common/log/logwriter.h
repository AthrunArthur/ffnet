#ifndef FFNET_COMMON_LOG_LOG_WRITER_H_
#define FFNET_COMMON_LOG_LOG_WRITER_H_
#include "utils/singleton.h"
#include "common/cond_pop_queue.h"
#include <boost/shared_ptr.hpp>
#include <fstream>

namespace ffnet
{
	namespace details
	{
	class LogWriter : boost::noncopyable
	{
	public:
		LogWriter();
		
		virtual ~LogWriter();
		CondPopQueue<String> &	queue(){return m_oQueue;}
		void			run(const char * filePath);
	protected:
		void			actualRun();
	protected:
		CondPopQueue<String>		m_oQueue;
		boost::shared_ptr<boost::thread>		m_oIOThread;
		String					m_strFilePath;
		boost::mutex				m_oMutex;
		std::ofstream				m_oFile;
		bool						m_bRunning;
	};//end class LogWriter
		
	}//end namespace details;
}//end namespace ffnet
#endif