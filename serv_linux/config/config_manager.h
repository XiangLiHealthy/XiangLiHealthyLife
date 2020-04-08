#ifndef CONFIG_MANAGER_
#define CONFIG_MANAGER_

#include"config_data.h"
#include<string>

using XiangLi::config_t;
using XiangLi::LOG_LEVEL;
using XiangLi::LOG_MODE;

class ConfigManager {

public:
	ConfigManager( );
	~ConfigManager( );

	static ConfigManager& GetInstance();
	int					LoadConfig( );
	int					SaveConfig( );
	const	config_t&	GetConfig( );
private:
	char				m_szPath[260 ];
	config_t			m_config;
	int					GetConfigPath(char* szPath, int nLen );
	int					GetConfigString( const char* pstrPath, std::string& strConfigDst);

	const char*			GetLogLevelDesc( LOG_LEVEL eLevel);
	LOG_LEVEL			GetLogLevel( const char* pstrLevel);

	const char*			GetLogModeDesc( LOG_MODE eMode);
	LOG_MODE			GetLogMode( const char* pstrMode);
	std::string			GetJsonString( );
};

#endif
