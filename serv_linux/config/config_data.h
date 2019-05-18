#pragma once

enum LOG_LEVEL_E {
	LOG_ERR = 0,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG,
	LOG_LEVEL_UNKNOWN
};

enum LOG_MODE_E{
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

struct log_t {
	LOG_LEVEL_E eLevel;
	LOG_MODE_E	eMode;
	char		szDir[ 260];
	int			nSaveDay;
};

struct config_t {
	int			nPort;
	int			nThreadCount;
	log_t		tlog;
	db_t		tdb;
};
