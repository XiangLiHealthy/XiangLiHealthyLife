#pragma once
#include"config_data.h"
#include<string>

class ConfigManager {

public:
	ConfigManager( );
	~ConfigManager( );

	int					LoadConfig( );
	int					SaveConfig( );
	const	config_t*	GetConfig( );
private:
	char				m_szPath[260 ];
	config_t			m_config;
	int					GetConfigPath(char* szPath, int nLen );
	int					GetConfigString( const char* pstrPath, std::string& strConfigDst);

	const char*			GetLogLevelDesc( LOG_LEVEL_E eLevel);
	LOG_LEVEL_E			GetLogLevel( const char* pstrLevel);

	const char*			GetLogModeDesc( LOG_MODE_E eMode);
	LOG_MODE_E			GetLogMode( const char* pstrMode);
	std::string			GetJsonString( );
};

extern ConfigManager g_config;
