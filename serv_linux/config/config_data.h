#pragma once
#include <string>
using std::string;

namespace XiangLi
{
	enum  LOG_LEVEL
	{
		LOG_DEBUG_E 		= 1,
		LOG_INFO_E		   = 2,
		LOG_WARNING_E = 3,
		LOG_ERROR_E 			= 4,
		LOG_FATAL_E = 5,
		LOG_UNKOWN
	};

	enum LOG_MODE
	{
		LOG_PRINT = 0,
		LOG_TEXT,
		LOG_MODE_UNKNOWN
	};

	struct db_t 
	{
		string		strHost;
		string		strDBName;
		string		strUsr;
		string		strPassword;
		int            limit_count;
	};
	
	struct log_t 
	{
		LOG_LEVEL eLevel;
		LOG_MODE	eMode;
		string		strDir;
		int			nSaveDay;
	};
	
	struct config_t 
	{
		int			nPort;
		int			nThreadCount;
		log_t		tlog;
		db_t		tdb;
	};
	
};