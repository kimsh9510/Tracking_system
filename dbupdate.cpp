#define _CRT_SECURE_NO_WARNINGS
#include <mysql.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "tjsgud9248"
#define DB_NAME "devices"
#define SQL_INSERT_RECORD "INSERT INTO cam_info VALUES \ (%d, '%s' , %s, %s,%s, '%s', %s, %s );" 
#define SQL_SELECT_RECORD "SELECT cam_id,time FROM cam_info WHERE cam_dis <= %d AND pic_link = 'Z:/HDD1/Data/img_00000%d.jpg';"
#define SQL_UPDATE_RECORD "UPDATE cam_info SET car_num = '%s' WHERE pic_link = '%s' ;"
#include "PossiblePlate.h"
using namespace cv;
using namespace std;

int dbupdate(PossiblePlate &licPlate, string b)
{
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	int i;
	int m = 1;
	char query[255];

	//인자정의
	// DB 연결
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	//cout << "Printed LicPlate.strChars value ";
	//cout << licPlate.strChars << endl;
	sprintf(query, SQL_UPDATE_RECORD, licPlate.strChars.c_str(), b.c_str());
	cout << "\nQuery UPDATE"/* << query */<< endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "여기" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	
	// DB 연결닫기
	mysql_close(connection);
	return 0;
}

