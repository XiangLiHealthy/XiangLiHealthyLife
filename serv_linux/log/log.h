#ifndef LOG
#define LOG

#include"../config/config_data.h"
#include <string>
#include <fstream>

using std::fstream;
using std::string;
using XiangLi::LOG_LEVEL;
using XiangLi::config_t;
using XiangLi::LOG_MODE;

class Log
{
public:
	Log &		GetInstance()
	{
		static Log singleton;
		return singleton;
	}

	void		SetConfig(const config_t* ptr_onfig_t );
	void		WriteLog(LOG_LEVEL level, const char *File, 
        			int Line, const char *Function, 
        			const char *format, ...);
private:
	LOG_LEVEL	 m_eLevel;
	string					 m_strDir;
	LOG_MODE	m_eMode;
	int 						m_log_day;
	fstream               m_log_file;
private:
	void GetLogHeader( string& strLog, LOG_LEVEL level, const char *File, 
        			int Line, const char *Function );
	int WriteLogFile(const string& content);
	Log( );
	~Log( );
};

#define LOG_DEBUG(format, ...) Log::GetInstance().WriteLog(LOG_LEVEL::DEBUG, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Log::GetInstance().WriteLog(LOG_LEVEL::INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Log::GetInstance().WriteLog(LOG_LEVEL::WARNING, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log::GetInstance().WriteLog(LOG_LEVEL::ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) Log::GetInstance().WriteLog(LOG_LEVEL::FATAL, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#endif
