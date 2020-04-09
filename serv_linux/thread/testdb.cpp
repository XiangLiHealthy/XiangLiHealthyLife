#include "../include/mysql/mysql.h"
#include<stdio.h>
#include<stdlib.h>
#include "../log/log.h"
/*
int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char server[] 	= "localhost";
	char user[]		= "xiangbaosong";
	char password[]	= "Xl2016xl";
	char database[]	= "testdb";

	conn = mysql_init(NULL);
	
	if ( !mysql_real_connect(conn, server, user,password, database, 0,NULL, 0) ) {
		LOG_ERROR("stderr:%s \n", mysql_error(conn));
		exit(1);
	}

	if (mysql_query(conn, "show tables;")) {
		LOG_ERROR("%s \n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	LOG_ERROR("tavles in mysql database:\n");

	while( (row = mysql_fetch_row(res)) != NULL) {
		LOG_ERROR("%s \n", row[0]);
	}

	mysql_free_result(res);
	mysql_close(conn);

	LOG_ERROR("finish \n");
	return 0;
}
*/
