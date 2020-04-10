#include "log.h"
#include<pthread.h> 
#include<time.h>
#include<sys/time.h>
#include<string.h>
#include<error.h>
#include <syslog.h>
#include <iostream>
#include <map>
#include <stdarg.h>

#undef max
#undef min
#include <sstream>
#include<unistd.h>
#include <stdio.h>

using namespace std;
static string LOG_DIR = "/etc/xiangli/log";

pthread_mutex_t lock;

map<LOG_LEVEL, string> g_log_level_desc =
{
	{XiangLi::LOG_DEBUG_E, "[D]"},
	{XiangLi::LOG_INFO_E, "[I]"},
	{XiangLi::LOG_WARNING_E, "[W]"},
	{XiangLi::LOG_ERROR_E, "[E]"},
	{XiangLi::LOG_FATAL_E, "[F]"},
};

Log::Log( )
{
	pthread_mutex_init( &lock,NULL);

	char szDir[256];
	m_strDir = LOG_DIR;
	m_eLevel = XiangLi::LOG_DEBUG_E;
}

Log::~Log( )
{
	pthread_mutex_destroy( &lock);
}

void Log::SetConfig( const config_t* ptrConfig) {
	if( NULL == ptrConfig) 
	{
		LOG_ERROR( " 非法参数NULL");
		return ;
	}

	pthread_mutex_lock( &lock);
	m_eMode	  = ptrConfig->tlog.eMode;
	m_eLevel	= ptrConfig->tlog.eLevel;
	m_strDir	  = ptrConfig->tlog.strDir;

	pthread_mutex_unlock( &lock);
}

void Log::WriteLog(LOG_LEVEL eLevel, const char *szFile, 
        			int nLine, const char *szFunction, 
        			const char *format, ...)
 {
	//根据日志等级绝对是否输出
	if (eLevel <  m_eLevel )
	{
		return;
	}

	//get log header
	string strLogHeader;
	GetLogHeader(strLogHeader, eLevel, szFile, nLine, szFunction);

	//merge log content
	va_list valst;
	va_start(valst, format);
	char szBody[2048] = {0};
	int nCount = vsnprintf(szBody, sizeof(szBody) -1,  format, valst);
	va_end(valst);
	if (nCount < 0)
	{
		openlog("XiangLiHealthyService", LOG_NOWAIT, LOG_SYSLOG);
		syslog(LOG_NOWAIT, "create log body failed :%s", strerror(errno));
		return ;
	}
	
	//merger header and body into log
	string strLog = strLogHeader +  ":" + szBody;
	
	//根据模式输出到终端或文件
	int ret = WriteLogFile(strLog);
	if (  ret != 0)
	{
		strLog += ret;
		openlog("XiangLiHealthyService", LOG_NOWAIT, LOG_SYSLOG);
		syslog(LOG_NOWAIT,"%s",  strLog.c_str());
	}
}

int Log::WriteLogFile(const string& content)
{
	//create file name by date
	struct timeval	tv;
	struct timezone	tz;
	struct tm		*p;
	int nRet = 0;

	try
	{
		gettimeofday( &tv, &tz);
		p = localtime( &tv.tv_sec);
		
		pthread_mutex_lock( &lock);
		do
		{
			//if out of date ,reopen log file
			if (p->tm_mday != m_log_day)
			{
				m_log_file.close();

				p->tm_year += 1900;
				p->tm_mon += 1;
				
				ostringstream ostr;
				ostr << p->tm_year <<"-"
						<< p->tm_mon  << "-"
						<< p->tm_mday;

				string file_name = m_strDir + "/" + ostr.str() + ".txt";

				m_log_file.open(file_name, ios_base::app | ios_base::out);
				if (!m_log_file)
				{
					nRet = -1;
					break;
				}
			}	

			m_log_file << content << endl;
		} while (0);
		pthread_mutex_unlock( &lock);

	}
	catch(...)
	{
		nRet = -2;
	}
	
	return nRet;
}

void Log::GetLogHeader( string& strLog, LOG_LEVEL eLevel, const char *szFile, 
        			int nLine, const char *szFunction )
{
	//取得线程id
	pthread_t pid = pthread_self( );
	
	//获取当前时间
	char szDateTime[ 128] =  {0};
	struct timeval	tv;
	struct timezone	tz;
	struct tm		*p;

	gettimeofday( &tv, &tz);
	p = localtime( &tv.tv_sec);
	if( NULL == p)
	 {
		strLog = " 无法获取时间：";
		strLog += strerror( errno);

	}else
	{
	sprintf( szDateTime, " %04d-%02d-%02d %02d:%02d:%02d.%03ld:",
				1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
				p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
		strLog += szDateTime;
	}

	//create log header
	//etc:/etc/log/log.cpp:42:WriteLog:[tid=12]:[E]:connect database error
	ostringstream ostr;
	ostr << szFile << ":" << nLine << ":" << szFunction << ":";

	ostr << "[tid=" << pid << "]";

	strLog += ostr.str();

	strLog += g_log_level_desc[eLevel];
}
