#pragma once

#include"config_manager.h"
#include<memory.h>
#include"../log/log.h"
#include"../../include/json/json.h"
#include<unistd.h>
#include<map>

using namespace XiangLi;

std::map<LOG_LEVEL, const char*>	mapLevel;
std::map<LOG_MODE, const char*>	mapMode;

ConfigManager::ConfigManager( )
{
	mapLevel[ LOG_ERROR_E]		= "LOG_ERR";
	mapLevel[ LOG_WARNING_E]	= "LOG_WARNING";
	mapLevel[ LOG_INFO_E]		= "LOG_INFO";
	mapLevel[ LOG_DEBUG_E]	= "LOG_DEBUG";

	mapMode[ LOG_PRINT]		= "LOG_PRINT";
	mapMode[ LOG_TEXT]		= "LOG_TEXT";


	m_config.tdb.strHost = "127.0.0.1";
	m_config.tdb.strDBName = "HealthyLife";
	m_config.tdb.strUsr = "xiangbaosong";
	m_config.tdb.strPassword = "Xl2016xl";
	m_config.tdb.limit_count = 10;
}

ConfigManager::~ConfigManager( ) {

}

int ConfigManager::LoadConfig( ){
	//1.获取程序全路劲，然后解析出文件目录
	GetConfigPath( m_szPath, sizeof( m_szPath));
	if( 0 == m_szPath[ 0]) {
		LOG_ERROR(" 配置文件路径加载失败！" );
		return -1;
	}

	//2.读取本目录下的config.ini文件
	std::string strConfig;
	if( GetConfigString( m_szPath, strConfig) < 0) {
		LOG_ERROR(" 读取配置文件流失败！");
		return -1;
	}

	Json::Value jConfig;
	Json::Reader reader;

	LOG_DEBUG( strConfig.c_str( ));

	if( !reader.parse(strConfig, jConfig )) {
		LOG_ERROR(" 配置文件解析成json对象失败！");
		return -1;
	}

	//3.解析对应的参数
	if( jConfig[" port"].isNull( ))	{
		LOG_ERROR(" 配置文件没有port");
		return -1;
	}
	m_config.nPort = jConfig[ " port"].asInt( );

	if( jConfig[ " threadCount"].isNull( )) {
		LOG_ERROR("配置文件没有threadCount");
		return -1;
	}
	m_config.nThreadCount = jConfig[ " threadCount"].asInt( );

	if( jConfig[  "log"].isNull( )){
		LOG_ERROR(" 配置文件中缺少log");
		return -1;
	}
	Json::Value jChild = jConfig[ "log"];

	log_t tLog;
	memset( &tLog, 0, sizeof( tLog));
	if( jChild[ "level"].isNull( )) {
		LOG_ERROR(" 配置文件里没有level");
		return -1;
	}
	tLog.eLevel = GetLogLevel( jChild[ "level"].asString( ).c_str( ) );

	if( jChild[ "mode"].isNull( )) {
		LOG_ERROR("配置文件里没有mode");
		return -1;
	}
	tLog.eMode = GetLogMode( jChild[ "mode"].asString( ).c_str( ));

	if( jChild[ "dir"].isNull( )){
		LOG_ERROR(" 配置文件log节点缺少dir");
		return -1;
	}
	tLog.strDir = jChild[ "dir"].asString( );
	
	if( jChild[ " saveDay"].isNull( )){
		LOG_ERROR(" 配置文件log节点却好啊savaDay");
		return -1;
	}
	tLog.nSaveDay = jChild[ "saveDay"].asInt( );

	m_config.tlog = tLog;

	//数据库节点
	if( jConfig[ "db"].isNull( )){
		LOG_ERROR(" 配置文件缺少db");
		return -1;
	}
	jChild = jConfig[ "db"];

	db_t tDB;
	memset( &tDB, 0, sizeof( tDB));

	if( jChild[ "host"].isNull( )){
		LOG_ERROR(" 配置文件db节点缺少host");
		return -1;
	}
	tDB.strHost = jChild[ "host"].asString( );
	
	if( jChild[ "db_name"].isNull( )) {
		LOG_ERROR(" 配置节点缺少db_name");
		return -1;
	}
	tDB.strDBName = jChild[ "db_name"].asString( );

	if( jChild[ "usr"].isNull( )){
		LOG_ERROR(" 配置文件缺少usr");
		return -1;
	}
	tDB.strUsr = jChild[ "usr"].asString( );

	if( jChild[ "password"].isNull( )){
		LOG_ERROR(" 配置文件缺少password");
		return -1;
	}
	tDB.strPassword = jChild[ " password"].asString( );

	m_config.tdb = tDB;

	return 0;
}

std::string ConfigManager::GetJsonString( ) {
	Json::Value jRoot;
	Json::Value jLog;
	Json::Value jDB;

	jRoot[ "port"]			= m_config.nPort;
	jRoot[ "threadCount"]	= m_config.nThreadCount;

	log_t tLog				= m_config.tlog;
	jLog[ "level"]			= GetLogLevelDesc( tLog.eLevel);
	jLog[ "mode"]			= GetLogModeDesc( tLog.eMode);
	jLog[ "dir"]			= tLog.strDir;
	jLog[ "saveDay"]		= tLog.nSaveDay;
	
	jRoot[ "log"]			= jLog;


	db_t tDB				= m_config.tdb;
	jDB[ "host"]			= tDB.strHost;
	jDB[ "db_name"]			= tDB.strDBName;
	jDB[ "usr"]				= tDB.strUsr;
	jDB[ "password"]		= tDB.strPassword;

	jRoot[ "db"]			= jDB;

	return jRoot.toStyledString( );
}


int ConfigManager::SaveConfig( ){
	//1.得到配置文件目录
	std::string strErr;
	FILE* pf = fopen( m_szPath,"rb+");
	if( NULL == pf) {
		strErr = " 配置文件打开失败：";
		strErr += strerror( errno);
		LOG_ERROR(strErr.c_str( ));
		return -1;
	}

	//得到json格式字符串string
	std::string strBuff = GetJsonString( );
	LOG_DEBUG( strBuff.c_str( ));

	//2.写入节点数据
	if( strBuff.size( ) != fwrite( strBuff.c_str( ), sizeof( char), strBuff.size( ), pf) ){
		strErr = " 配置文件写入失败";
		strErr += strerror( errno);	
		LOG_ERROR(strErr.c_str( ));
	}

	fclose( pf);

	return 0;
}

const config_t& ConfigManager::GetConfig( ){

	return m_config;
}


int ConfigManager::GetConfigPath( char* szPath, int nLen) {
	if( NULL == szPath ) {
		return -1;
	}

	//获取文件模块目录
	if( NULL == getcwd( szPath, nLen - 1)) {
		LOG_ERROR(" 获取当前工作目录失败！");
		return -1;
	}

	//寻找最后一个反斜线的位置
	//int nPos = strrch( )//这已经是目录
	//std::string strPath = szPath;
	//int nPos = strPath.rfind( "/");
	//if( nPos < 0) {
	//	std::string strErr = " 非法路径：" + strPath;
	//	LOG_ERROR(strErr );
	//	return -1;
	//}

	//将斜线后面的位置替换成config.json
	sprintf(szPath + strlen( szPath), "/config.json"  );

	LOG_DEBUG( szPath);

	return 0;
}

int ConfigManager::GetConfigString( const char* pstrPath, std::string& strConfig) {
	if( NULL == pstrPath || 0 == pstrPath[ 0]) {
		LOG_ERROR(" 非法参数NULL！");
		return -1;
	}

	std::string strErr;

	//打开配置文件
	FILE* pf = fopen( pstrPath, "rb+");
	if( NULL == pf) {
		strErr = " 文件打开失败：" ;
	    strErr += strerror(errno );
		LOG_ERROR(strErr.c_str( ));
		return -1;
	}

	size_t num;
	char szBuff[ 1024];
	memset( szBuff, 0, sizeof( szBuff));
	strConfig = "";

	//读取配置数据流
	while( fread( szBuff, sizeof( char), sizeof( szBuff), pf)){
		strConfig += szBuff;

		memset( szBuff, 0, sizeof( szBuff));
	}

	fclose( pf);

	return 0;
}

const char* ConfigManager::GetLogLevelDesc( LOG_LEVEL eLevel) {
	if( mapLevel.count( eLevel) < 1){
		return "UNKNOWN";
	}

	return mapLevel[ eLevel];
}

LOG_LEVEL ConfigManager::GetLogLevel( const char* pstrLevel) 
{
	if( NULL == pstrLevel || 0 == pstrLevel[ 0]) {
		return LOG_UNKOWN; 
	}

	std::map<LOG_LEVEL, const char*>::iterator itr = mapLevel.begin( );
	for( ; itr != mapLevel.end( ); itr++) {
		if( strcmp( pstrLevel, itr->second) == 0){
			return itr->first;
		}
	}

	return LOG_UNKOWN;
}

const char* ConfigManager::GetLogModeDesc( LOG_MODE eMode) {
	if( mapMode.count( eMode) < 1) {
		return "UNKNOWN";
	}

	return mapMode[ eMode];
}

LOG_MODE ConfigManager::GetLogMode( const char* pstrMode) {
	if( NULL == pstrMode || 0 == pstrMode[ 0]) {
		return LOG_MODE_UNKNOWN;
	}

	std::map<LOG_MODE, const char*> ::iterator itr = mapMode.begin( );
	for( ; itr != mapMode.end( ); itr++) {
		if( strcmp( itr->second , pstrMode) == 0) {
			return itr->first;
		}
	}

	return LOG_MODE_UNKNOWN;
}


ConfigManager& ConfigManager::GetInstance()
{
	static ConfigManager sinleton;
	return sinleton;
}
