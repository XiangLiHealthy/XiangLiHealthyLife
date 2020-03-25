#include"DataBase.h"
#include<string.h>
#include <iostream>

using namespace std;

DataBase::DataBase() {
		connection 	= NULL;
		result 		= NULL;
		row			= NULL;
		fields		= NULL;
}

DataBase::~DataBase() {
	Disconnect();
}

bool DataBase::Connect(const string& host, const string& db, const string& user, const string& password) {

	connection = mysql_init(NULL);
	if(NULL == connection) 
	{
		m_lastError = "初始化数据库失败!";
		m_lastError += mysql_error(NULL);
		return false;
	}

	
	if(NULL == mysql_real_connect(connection, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, NULL, 0)) 
	{
		m_lastError = "数据库连接失败:";
		m_lastError	+= mysql_error(connection);
		Disconnect();
		return false;
	}

	return true;
}

void DataBase::Disconnect() {
	if(connection) {
		mysql_close(connection);
		connection = NULL;
	}
}

LONG DataBase::Exec(const string& sql) 
{
	//执行查询
	if(!mysql_query(connection, sql.c_str())) 
	{
		m_lastError = mysql_error(connection);
		return -1;
	}


	//获取记录数,执行写操作应该没有结果集,不知道那是什么情况
	LONG count = 0;
	result = mysql_use_result(connection);
	if(NULL != result) 
	{
		count = result->row_count;
	}

	return count;
}

const string& DataBase::GetLastError() 
{
	return m_lastError;
}

bool DataBase::HasNext() {
	//下一行 string&* row
	row =  mysql_fetch_row(result);
	return (NULL == row) ? false : true;
}

void DataBase::ReleaseResult() {
	//释放数据库读锁,并清除缓存;
	if(result) {
		mysql_free_result(result);
		result = NULL;
	}
}




bool DataBase::GetFieldValue(const string& name, int& value){

	//获取对应的列值 
	const string field = GetField(name );
	
	value = atoi(field.c_str());

	return true;
}

bool DataBase::GetFieldValue(const string& name, bool& value) {
	string field = GetField(name );
	value = atoi(field.c_str());

	return true;
}

bool DataBase::GetFieldValue(const string& name, short& value) 
{
	const string& field = GetField(name );
	if(field.empty()) {
		return false;
	}
	
	value = atoi(field.c_str());
	return true;
}

bool DataBase::GetFieldValue(const string& name, string& value) {

	if(value.empty()) {
		m_lastError = "非法参数NULL";
		return false;
	}

	value = GetField(name);
	if(value.empty()) {
		return false;
	}

	return true;
}

bool DataBase::GetFieldValue(const string& name, LONG& value) {
	const string& field = GetField(name );
	if(field.empty()) {
		return false;
	}

	value = atol(field.c_str());

	return true;
}

int DataBase::GetNameIndex(const string& name) {
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	if(NULL == fields) {
		return -1;
	}

	int nCount = mysql_num_fields(result);
	for(int nIndex = 0; nIndex < nCount; nIndex ++) 
	{
		if(0 == strcmp(name.c_str(), fields[nIndex].name)) 
		{
			return nIndex;
		}
	}

	return -1;
}

string DataBase::GetField(const string& name) 
{
	//获取到字段名的索引
	string tmp;

	int nIndex = GetNameIndex(name);
	if(nIndex < 0) {
		m_lastError = "字段名不存在:";
		m_lastError += name;
		return tmp;
	}

	//获取对应的列值 
	char* field = row[nIndex];
	if(NULL == field) {
		m_lastError = "无效值";
		return tmp;
	}

	tmp = field;
	return tmp;
}

