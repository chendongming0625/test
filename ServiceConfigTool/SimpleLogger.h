#ifndef __SIMPLE_LOGGER_H__
#define __SIMPLE_LOGGER_H__


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <mutex>
#include <fstream>


using namespace std;


#define MAX_LOG_SIZE	1024*1024*2
#define DEFAULT_LOG_FILE "VAS_Tool.log"


class CLog
{
public:
	~CLog();
	static CLog* GetInstance();
	void WriteLog(string strFile, int nLine, const char *format, ...);
	void SetLogPath(string strPath);

private:
	CLog();
	string m_strFileName;
	static CLog *m_This;
#if defined(_MSC_VER)
	static mutex m_Mutex;
#else
	pthread_mutex_t m_Lock;
#endif
};



#define LOGTAG 1

#if LOGTAG

#define LOG(FORMAT, ... ) if(CLog::GetInstance()) CLog::GetInstance()->WriteLog(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__);
#define GetLogInst	if(CLog::GetInstance()) CLog::GetInstance()

#define LOG1(FORMAT, ... ) __noop

#else

#define LOG(FORMAT, ... ) __noop
#define GetLogInst	if(CLog::GetInstance()) CLog::GetInstance()

#define LOG1(FORMAT, ... ) if(CLog::GetInstance()) CLog::GetInstance()->WriteLog(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__);

#endif






#endif
