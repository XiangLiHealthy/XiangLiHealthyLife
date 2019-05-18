#pragma once
#include<mysql/mysql.h>
#include<string>

typedef long long LONG;

class DataBase {
	public:
		DataBase();
		~DataBase();

		/*****************************************************************
		 *const char* host:		数据库的ip地址或者主机名
		 *const char* DBName:	数据库的名称;
		 *const char* user:		数据库用户名;
		 *const char* password:	用户密码;
		 *返回值:				true 连接成功,false 连接失败;
		 *
		 * ****************************************************************/ 
		bool 		Connect(const char* host, const char* DBName, const char* user, const char* password);

		/*****************************************************************
		 *释放连接及所有资源
		 * *************************************************************/ 
		void 		Disconnect();

		/*******************************************************************
		 *const char* sql	: 数据库查询语句
		 *返回值			: 成功就返回数据的结果条数,失败就返回-1;
		 *
		 * *****************************************************************/ 
		LONG 		Exec(const char* query);

		const char* GetLastError();

		/*********************************************************************
		 *功能:		指向下一行的结果集
		 *参数:		无
		 *返回值:	true 成功, false 失败 
		 *说明:		依次访问每一行数据的效率比行索引号访问的效率高;用完记得调用
		 *			ReleaseResult()释放内存;
		 * *******************************************************************/ 
		bool		HasNext();
		void		ReleaseResult();

		int			GetNameIndex(const char* name);
		bool 		GetFieldValue(const char* name, int& value);
		bool		GetFieldValue(const char* name, LONG& value);
		bool		GetFieldValue(const char* name, char* value);
		bool		GetFieldValue(const char* name, bool& value);
		bool		GetFieldValue(const char* name, short& value);

	private:
		MYSQL*			connection;
		MYSQL_RES*		result;
		MYSQL_ROW		row;
		MYSQL_FIELD*	fields;	
		std::string 	m_lastError;

		char*			GetField(const char* name, int len);
};
