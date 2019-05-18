#include"DataBase.h"
#include<string.h>
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

bool DataBase::Connect(const char* host, const char* db, const char* user, const char* password) {
	if(NULL == host || NULL == db || NULL == user || NULL == password) {
		m_lastError = "空指针!";
		return false;
	}

	if(0 == host[0] || 0 == db[0] || 0 == user[0]) {
		m_lastError = "host,DBName,user不许与为空!";
		return false;
	}

	connection = mysql_init(NULL);
	if(NULL == connection) {
		m_lastError = "初始化数据库失败!";
		m_lastError += mysql_error(NULL);
		return false;
	}

	
	if(NULL == mysql_real_connect(connection, host, user, password, db, 0, NULL, 0)) {
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

LONG DataBase::Exec(const char* sql) {
	if(NULL == sql || 0 == sql[0]) {
		m_lastError = "无效的sql,NULL或为空!";
		return -1;
	}

	//执行查询
	if(!mysql_query(connection, sql)) {
		m_lastError = mysql_error(connection);
		return -1;
	}


	//获取记录数,执行写操作应该没有结果集,不知道那是什么情况
	LONG count = 0;
	result = mysql_use_result(connection);
	if(NULL != result) {
		count = result->row_count;
	}

	return count;
}

const char* DataBase::GetLastError() {
	return m_lastError.c_str();
}

bool DataBase::HasNext() {
	//下一行 char** row
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




bool DataBase::GetFieldValue(const char* name, int& value){
	if(NULL == name || 0 == name[0]) {
		m_lastError = "字段名为NULL或为空!";
		return false;
	}

	
	//获取对应的列值 
	char* field = GetField(name, sizeof(value));
	if(NULL == field) {
		return false;
	}

	value = atoi(field);

	return true;
}

bool DataBase::GetFieldValue(const char* name, bool& value) {
	char* field = GetField(name, sizeof(value));
	if(NULL == field) {
		return false;
	}


	value = atoi(field);

	return true;
}

bool DataBase::GetFieldValue(const char* name, short& value) {
	char* field = GetField(name, sizeof(value));
	if(NULL == field) {
		return false;
	}
	
	value = atoi(field);
	return true;
}

bool DataBase::GetFieldValue(const char* name, char* value) {

	if(NULL == value) {
		m_lastError = "非法参数NULL";
		return false;
	}

	char* field = GetField(name, sizeof(value));
	if(NULL == field) {
		return false;
	}

	strcpy(value, field);
	return true;
}

bool DataBase::GetFieldValue(const char* name, LONG& value) {
	char* field = GetField(name, sizeof(value));
	if(NULL == field) {
		return false;
	}

	value = atol(field);

	return true;
}

int DataBase::GetNameIndex(const char* name) {
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	if(NULL == fields) {
		return -1;
	}

	int nCount = mysql_num_fields(result);
	for(int nIndex = 0; nIndex < nCount; nIndex ++) {
		if(0 == strcmp(name, fields[nIndex].name)) {
			return nIndex;
		}
	}

	return -1;
}

char* DataBase::GetField(const char* name, int len) {
	if(NULL == name || 0 == name[0]) {
		m_lastError = "字段名为NULL或为空!";
		return NULL;
	}

	//获取到字段名的索引
	int nIndex = GetNameIndex(name);
	if(nIndex < 0) {
		m_lastError = "字段名不存在:";
		m_lastError += name;
		return NULL;
	}

	//获取对应的列值 
	char* field = row[nIndex];
	if(NULL == field) {
		m_lastError = "无效值";
		return NULL;
	}

	if(len > strlen(field)) {
		m_lastError = """字段类型不匹配";
		return NULL;
	}

	return field;
}

