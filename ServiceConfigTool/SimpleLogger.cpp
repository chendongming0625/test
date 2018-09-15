#include "SimpleLogger.h"
#include "MainUI.h"
#ifdef _MSC_VER
#include <windows.h>
mutex CLog::m_Mutex;
#endif


CLog *CLog::m_This = NULL;
CLog::CLog()
{
	m_This = this;
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	m_strFileName = szPath;
	size_t pos = m_strFileName.rfind('\\');
	m_strFileName.erase(pos);
	m_strFileName += "\\..\\data\\log\\Vastool.log";
}

CLog::~CLog()
{
	m_This = NULL;
}


CLog* CLog::GetInstance()
{
	static CLog m_Instance;
	return m_This;
}

void CLog::SetLogPath(string strPath)
{
#ifdef _MSC_VER
	std::lock_guard<mutex> lck(m_Mutex);	//互斥锁
#else
	pthread_mutex_lock(&m_Lock);
#endif // _MSC_VER
	
	m_strFileName = strPath + DEFAULT_LOG_FILE;

#ifndef _MSC_VER
	pthread_mutex_unlock(&m_Lock);
#endif
}

void CLog::WriteLog(string strFile, int nLine, const char* format, ...)
{
#ifdef _MSC_VER
	std::lock_guard<mutex> lck(m_Mutex);	//互斥锁
	size_t pos = strFile.rfind("\\");
#else
	pthread_mutex_lock(&m_Lock);
	size_t pos = strFile.rfind("/");
#endif
	if (pos != string::npos)
		strFile.assign(strFile, pos + 1, string::npos);	//全路径太长  只取文件名

	char szContent[1024 * 20] = { 0 };
	va_list arguments;
	va_start(arguments, format);
	vsnprintf(szContent, 1024 * 20, format, arguments);
	va_end(arguments);

	time_t ttime = time(0);
	struct tm *pTime = localtime(&ttime);
	char szHead[1024];
	strftime(szHead, 80, "[%Y-%m-%d %H:%M:%S]", pTime);
	sprintf(szHead, "%s[%s][%d]", szHead, strFile.c_str(), nLine);
	sprintf(szHead, "%-60s	", szHead);
	string strLog = szHead;
	strLog += szContent;




//////////////////////////////////////////////////////////////////PC
#ifndef _DEBUG
	fstream file(m_strFileName, ios::out | ios::in | ios::binary);
	file.seekp(0, ios::end);
	streampos size = file.tellp();
	if (!file || size > MAX_LOG_SIZE)
	{
		file.close();
		file.open(m_strFileName, ios::out);	//如果文件过大则清空重写
	}
	file << strLog;
	file.close();
#else
	OutputDebugStringA(strLog.c_str());
#endif
//////////////////////////////////////////////////////////////////

}
