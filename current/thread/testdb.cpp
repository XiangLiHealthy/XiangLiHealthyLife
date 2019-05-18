#include<mysql/mysql.h>
#include<stdio.h>
#include<stdlib.h>

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
		printf("stderr:%s \n", mysql_error(conn));
		exit(1);
	}

	if (mysql_query(conn, "show tables;")) {
		printf("%s \n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	printf("tavles in mysql database:\n");

	while( (row = mysql_fetch_row(res)) != NULL) {
		printf("%s \n", row[0]);
	}

	mysql_free_result(res);
	mysql_close(conn);

	printf("finish \n");
	return 0;
}
*/
