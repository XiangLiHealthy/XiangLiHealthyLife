#pragma once
#include"../config/config_data.h"

class Log{
public:
	Log( );
	~Log( );

	void		SetConfig(const config_t* ptr_onfig_t );
	void		Add(LOG_LEVEL_E level, const char* content);
private:
	LOG_LEVEL_E	m_eLevel;
	char*		m_pstrDir;
	LOG_MODE_E	m_eMode;
};

extern Log g_logger;
