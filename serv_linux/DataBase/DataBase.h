#ifndef DATABASE_
#define DATABASE_

#include<mysql/mysql.h>
#include<string>
using std::string;

typedef long long LONG;

class DataBase {
	public:
		DataBase();
		~DataBase();

		/*****************************************************************
		 *const string& host:		数据库的ip地址或者主机名
		 *const string& DBName:	数据库的名称;
		 *const string& user:		数据库用户名;
		 *const string& password:	用户密码;
		 *返回值:				true 连接成功,false 连接失败;
		 *
		 * ****************************************************************/ 
		bool 		Connect(const string& host, const string& DBName, const string& user, const string& password);

		/*****************************************************************
		 *释放连接及所有资源
		 * *************************************************************/ 
		void 		Disconnect();

		/*******************************************************************
		 *const string& sql	: 数据库查询语句
		 *返回值			: 成功就返回数据的结果条数,失败就返回-1;
		 *
		 * *****************************************************************/ 
		LONG 		Exec(const string& query);

		const string& GetLastError();

		/*********************************************************************
		 *功能:		指向下一行的结果集
		 *参数:		无
		 *返回值:	true 成功, false 失败 
		 *说明:		依次访问每一行数据的效率比行索引号访问的效率高;用完记得调用
		 *			ReleaseResult()释放内存;
		 * *******************************************************************/ 
		bool		HasNext();
		void		ReleaseResult();

		int			GetNameIndex(const string& name);
		bool 		GetFieldValue(const string& name, int& value);
		bool		GetFieldValue(const string& name, LONG& value);
		bool		GetFieldValue(const string& name, string& value);
		bool		GetFieldValue(const string& name, bool& value);
		bool		GetFieldValue(const string& name, short& value);

	private:
		MYSQL*			connection;
		MYSQL_RES*		result;
		MYSQL_ROW		row;
		MYSQL_FIELD*	fields;	
		std::string 	m_lastError;

		string			GetField(const string& name);
};

#endif
