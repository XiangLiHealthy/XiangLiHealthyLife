#include "log.h"
#include<pthread.h> 
#include<time.h>
#include<sys/time.h>
#include<string>
#include<error.h>

Log g_logger;

pthread_mutex_t lock;

Log::Log( ){
	pthread_mutex_init( &lock,NULL);
}

Log::~Log( ){
	pthread_mutex_destroy( &lock);
}

void Log::SetConfig( const config_t* ptConfig) {
	if( NULL == ptConfig) {
		Add( LOG_ERR, " 非法参数NULL");
		return ;
	}

	pthread_mutex_lock( &lock);
	m_eMode		= ptConfig->tlog.eMode;
	m_eLevel	= ptConfig->tlog.eLevel;
	m_pstrDir	= ptConfig->tlog.szDir;

	pthread_mutex_unlock( &lock);
}

void Log::Add(LOG_LEVEL_E level, const char* content) {
	pthread_mutex_lock( &lock);
	
	std::string strLog;

	//取得线程id
	pthread_t pid = pthread_self( );
	
	//获取当前时间
	char szDateTime[ 128] =  {0};
	struct timeval	tv;
	struct timezone	tz;
	struct tm		*p;

	gettimeofday( &tv, &tz);

	p = localtime( &tv.tv_sec);
	if( NULL == p) {
		strLog = " 无法获取时间：";
		strLog += strerror( errno);

	}else{
		sprintf( szDateTime, " %4d-%2d-%2d %2d:%2d:%2d.%3d",
				1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
				p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
		strLog += szDateTime;
	}

	char szThreadID[ 64] = {0};
	sprintf( szThreadID, " id:%h", pid);
	strLog += szThreadID;

	strLog += content;

	//根据日志等级绝对是否输出
	
	//根据模式输出到终端或文件
	
	pthread_mutex_unlock( &lock);
}
