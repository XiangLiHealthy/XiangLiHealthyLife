#pragma once

namespace XiangLi
{
	enum  LOG_LEVEL
	{
		LOG_DEBUG_E 		= 1,
		LOG_INFO_E		   = 2,
		LOG_WARNING_E = 3,
		LOG_ERROR_E 			= 4,
		LOG_FATAL_E = 5,
	};

	enum LOG_MODE
	{
		LOG_PRINT = 0,
		LOG_TEXT,
		LOG_MODE_UNKNOWN
	};

	struct db_t {
		char		szHost[ 32];
		char		szDBName[ 64];
		char		szUsr[ 64];
		char		szPassword[ 64];
	};
	
	struct log_t 
	{
		LOG_LEVEL eLevel;
		LOG_MODE	eMode;
		char		szDir[ 260];
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