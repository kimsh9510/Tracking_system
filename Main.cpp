// db����Ǹ鼭, ��ȣ���ν� ���Ѱ�. ����MiR Ž�����
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <mysql.h>
#include "Main.h"
#include "db.h"
#include "dbinsert.h"
#include "GetDistance.h"
#include "GetTimeT.h"
#include "select.h"
#include <time.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"libmySQL.lib")
//db.h�� ����Ǿ�����.
#define SQL_INSERT_RECORD "INSERT INTO cam_info VALUES \ (%d, '%s' , %s, %s,%s, '%s', %s, %s );" 
#define SQL_SELECT_RECORD "SELECT cam_id,time FROM cam_info WHERE cam_dis <= %d AND pic_link = 'Z:/HDD1/Data/img_00000%d.jpg';"
#define SQL_SELECT_ID "SELECT cam_id FROM cam_info WHERE car_num = '%s';"
#define SQL_SELECT_LINK "SELECT pic_link FROM cam_info WHERE cam_id = %d AND time >= '%s';"
#define STACK_SIZE 3
#define SQL_UPDATE_RECORD "UPDATE cam_info SET car_num = '%s' WHERE pic_link = '%s' ;"
#define SQL_SELECT_TIME2 "SELECT time FROM cam_info WHERE car_num = '%s' AND cam_id = %d;"
#define MAX_STACK_SIZE 10 //path��� � ������� �Ⱦ�
//#define SQL_UPDATE_DIS "UPDATE cam_info SET dis = '%d' WHERE cam_dis <= %d ;"
int Platenum(string a);
int GetTimeT(int year, int month, int day, int hour, int minute, int second);
double Max(double *n, int size);
int selectTIME2(char *a, int b);
int selectTIMEE2(char *a, int b);
void Push(int nData);
int Pop();
int *pStack = NULL;

//��������
double mov_dis;
char *num;
char linkselect(int a,char *b);
char number[256];
int m = 0;
//int k, k2,k3;
char *taketime;
int k[5];
int stack[MAX_STACK_SIZE];
int path[5];
MYSQL_ROW row1;
double in[5];
double max2[5];
double Max2(double *n, int size);
int c = 0;
////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
	system("Title Tracking System");
	system("COLOR F0");
	pStack = (int*)malloc((sizeof(int)) * STACK_SIZE);
	// �ʱ�ȭ
	for (int i = 0; i < STACK_SIZE; i++) {
		pStack[i] = NULL;
	}
	//printf("%s\n", mysql_get_client_info());// ������� ������ �������
	//char a[256];
	int b[256];
	tagPT loc[6];//20
	
	//ī�޶�1�� ��ġ it4
	loc[1].y = 35.889147;
	loc[1].x = 128.610314;
	//ī�޶�2�� ��ġ �����
	loc[2].y = 35.894074;
	loc[2].x = 128.606904;
	//ī�޶�3�� ��ġ ��ȭ����
	loc[3].y = 35.902965;
	loc[3].x = 128.618665;
	//ī�޶�4�� ��ġ �����б�
	loc[4].y = 35.897390;
	loc[4].x = 128.620502;
	//ī�޶�5�� ��ġ ����,, ���þƾ��
	loc[5].y = 58.311724;
	loc[5].x = 161.803357;
	
	/*
	loc[1].y = 35.898553;
	loc[1].x = 128.604039;
	loc[2].y = 35.902585;
	loc[2].x = 128.614511;
	loc[3].y = 35.896467;
	loc[3].x = 128.620519;
	loc[4].y = 35.899804;
	loc[4].x = 128.627042;
	loc[5].y = 35.905714;
	loc[5].x = 128.639144;
	loc[6].y = 35.887219;
	loc[6].x = 128.603524;
	loc[7].y = 35.889861;
	loc[7].x = 128.611249;
	loc[8].y = 35.887567;
	loc[8].x = 128.615884;
	loc[9].y = 35.892295;
	loc[9].x = 128.638200;
	loc[10].y = 35.896258;
	loc[10].x = 128.640775;
	loc[11].y = 35.882143;
	loc[11].x = 128.613395;
	loc[12].y = 35.881030;
	loc[12].x = 128.622235;
	*/
	int i, j;
	int l=0;
	float time;
	clock_t start, end, end2;
	double car_in[5];
	double Dis[5][5];//////5.5
	

	for (i = 1; i <= 5; i++) {//12
		for (j = 1; j <= 5; j++) {
			Dis[i][j] = GetDistance(loc[i], loc[j]);
			 printf("Dis[%d][%d] = %f\n", i, j, Dis[i][j]);
		}
	}
	
	printf("ã���� �ϴ� ��ȣ���� ��ȣ�� �Է��ϼ��� : ");
	scanf("%s", number);
	printf("\n-------------------------------------------------------\n\n");

	//////
	start = clock();
	//////
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {

		fprintf(stderr, "mysql_init() failed \n");

		exit(1);

	}
	if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS,

		DB_NAME, 3306, NULL, 0) == NULL)
	{
		finish_with_error(con);
	}
	//�� �������� �κ�
	if (mysql_query(con, "SELECT pic_link FROM cam_info"))//////////////�̺κп��� WHERE �־ Ž�� ���ϴ� �ð��� �����غ���
	{
		finish_with_error(con);
	}
	// mysql_store_result�Լ��� result set�� ��������
	// result set�� MYSQL_RES ����ü�� �����Ѵ�

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL)
	{
		finish_with_error(con);
	}
	// mysql_num_fields �Լ��� ���̺��� Column ���� �˾Ƴ���
	int num_fields = mysql_num_fields(result);
	// row�� ���������� �ݺ��ؼ� row�� �������鼭
	// ������ ����ŭ row�� ������ �ֿܼ� ����Ѵ�
	MYSQL_ROW row;
	//string num[100];
	//string A = "LHF";
	while (row = mysql_fetch_row(result))
	{
		for (int i = 0; i < num_fields; i++)
		{
			printf("%s\n ", row[i] ? row[i] : "NULL");
			m = Platenum(row[i]); //������ row[i]=pic_link �� ���ؼ� plate number �м�
			if (m == 1) {
				goto EXIT;
			}
		}
		printf("\n");
	}
	if (m != 1) {
		printf("��ȣ�� Ž�� �Ұ�\n");//ó������ ������ �� Ž���ߴµ� ��ȣ���� ã������ ����
		goto EXIT2;//������
	}
	EXIT: //���ϴ� ��ȣ���� ã���� ��ȣ���ν� ����

	//dbselect("UAI");
	dbselect(number);
	//printf("num��� : %s\n", num);
	selectTIME(number); // �ð�, �̵��ݰ�
	selectTIMEE(number); // �ð�
	
	//num�̿��� �ݰ泻�� �ִ� ī�޶� id ���ϱ�
	int car_id = atoi(num); //��ο� �߰�
	k[5] = { 0, };
	//int in[5];//�ݰ� �� ī�޶� ������
	k[0] = car_id;//��� �߰��ؾߵ� 
	//�ݰ泻�� �ִ� ī�޶� �Ÿ��� �迭 car_in[]�� �ְ� ������ ���� 0���� ä���.
	for (int c = 1; c <= 5; c++) { //���� 3
	
			if (Dis[car_id][c] < mov_dis && car_id != c) {
				//printf("�ݰ泻�� �ִ� ī�޶��ȣ = %d �̰� Dis[%d][%d] = %f �̴�.\n", c, car_id, c, Dis[car_id][c]);
				printf("�ݰ泻�� �ִ� ī�޶��ȣ = %d �̰� ī�޶� �Ÿ� = %f.\n\n",c, Dis[car_id][c]);
			
				//in[l] = c;
				car_in[l] = Dis[car_id][c]; //�ݰ� �� �Ÿ��ֱ�
				l++;
			}

			for (i = l; i < 5; i++) {
				car_in[i] = NULL;
			}
		
	}
	//printf("l : %d\n", l);
	/*�Ÿ� �迭�� �Ÿ��� �ֱ�
	for (i = 0; i < sizeof(car_in)/sizeof(double); i++) {
		printf("size = %d car_in = %f\n",sizeof(car_in)/sizeof(double), car_in[i]);
	}
	*/

	//�̵��ݰ� �� ī�޶� �߿� �Ÿ��� �� �� ī�޶�ã��
	double max = Max(car_in, sizeof(car_in) / sizeof(double));
	//printf("max = %f\n", max);

	Max2(car_in, sizeof(car_in) / sizeof(double));
	for (int i = 0; i < 5; i++) {
		printf("[in : %f]\n", in[i]);
	}
	
	for (k[1] = 1; k[1] <= 5; k[1]++) { //���� 3
		if (max == Dis[car_id][k[1]])
			break;
	}
	printf("�ݰ� �� �ִ� Ce ���� ������ ī�޶� ��ȣ = %d\n\n", k[1]);  //k�� ���� Ž���� �����ؾ��� ī�޶�
	printf("-----------------------------------------\n\n");
	///////////////k�� ���� �ٽ�Ž��!! 
	
	for (int d = 1; d < 3; d++) {//d�� ����� ��� ã�� ������ �Է��ϸ�ȴ�.
		//#define SQL_SELECT_LINK "SELECT pic_link FROM cam_info WHERE cam_id = %d AND time >= TO_DATE('2020-01-31 10:23:01') ;"
		printf("d=%d", d);
		linkselect(k[d],row1[0]); //k�� ī�޶� ���� �̹��� ��ȣ�� �ν�=>db�ֱ�
		if (m != 1) {
			//�̱��� ���Ծ� car_id?////////////////
			for (k[1] = 1; k[1] <= 5; k[1]++) { //���� 3
				if (in[1] == Dis[car_id][k[1]])//in[1] �� �ι�° �Ÿ��� �� ī�޶�
				{
					printf("����k[1]=%d", k[1]);
					break;
				}
				else if (in[1] == 0)//in[1] �� �ι�° ī�޶� ������ Ž���� �� ���ص� �ȴ�..
					goto EXIT3;
			}
			linkselect(k[d], row1[0]);
			if (m != 1) {
				goto EXIT3;
			}
		}
		selectTIME2(number, k[d]); //db�� number �νĵǰ�, ī�޶� ��ȣk �̸� ����ð�ã��
		selectTIMEE2(number,k[d]);
		l = 0;

		for (int c = 1; c <= 5; c++) { //���� 3

			if (Dis[k[1]][c] < mov_dis && k[d] != c) {
				printf("�ݰ泻�� �ִ� ī�޶� ��ȣ = %d �̰� ī�޶� �Ÿ� = %f\n\n", c, Dis[k[d]][c]);
			
				car_in[l] = Dis[k[d]][c]; //�ݰ� �� �Ÿ��ֱ�
				l++;
			}
			for (i = l; i < 5; i++) {
				car_in[i] = NULL;
			}
		}
		for (i = 0; i < sizeof(car_in) / sizeof(double); i++) {
			//printf("size = %d car_in = %f\n", sizeof(car_in) / sizeof(double), car_in[i]);
		}
		//�̵��ݰ� �� ī�޶� �߿� �Ÿ��� �� �� ī�޶�ã��
		max = Max(car_in, sizeof(car_in) / sizeof(double));
		//printf("max = %f\n", max);

	    Max2(car_in, sizeof(car_in) / sizeof(double));
		for (int i = 0; i < 5; i++) {
			printf("����[in : %f]\n", in[i]);
		}
		for (k[d+1] = 1; k[d+1] <= 5; k[d+1]++) {  //���� 3
			if (max == Dis[k[d]][k[d+1]])
				break;
		}
		printf("�ݰ� �� �ִ� Ce ���� ������ ī�޶� ��ȣ2 = %d\n\n", k[d+1]);  //k�� ���� Ž���� �����ؾ��� ī�޶�
		printf("-----------------------------------------\n\n");

		//printf("k[d+1]=%d\n", k[d + 1]);
	}
	EXIT3:
	printf("������ �̵���� : ");

	/*for (int path = 1; path < 4; path++) {
		printf("%d �� ī�޶�->", k[path]);
	}*/
	for (int i = 0; i < STACK_SIZE; i++) {
		printf("%d �� ī�޶�",pStack[i]);
		if (i >= STACK_SIZE-1) {
			break;
		}
		printf("->");
	}
	printf("\n\n");

	///

EXIT2:
	end = clock();
	time = (float)(end - start) / CLOCKS_PER_SEC;
	printf("�ɸ� �ð�: %f sec\n\n", time);
	printf("%d", c);
	mysql_free_result(result);
	mysql_close(con);
	exit(0);

	}

int Platenum(string a) {

	bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN();           // attempt KNN training

	if (blnKNNTrainingSuccessful == false) {                            // if KNN training was not successful
																		// show error message
		std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
		return(0);                                                      // and exit program
	}

	cv::Mat imgOriginalScene;           // input image

	imgOriginalScene = cv::imread(a);         // open image
											  //imgOriginalScene = cv::imread("Z:\\HDD1\\Data\\image1.png");
											  //imgOriginalScene = cv::imread();   //�� buf=�̹��� ��θ� �ٷ� �о���� �� �ְ� 
	if (imgOriginalScene.empty()) {                             // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);                                              // and exit program
	}

	std::vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene);          // detect plates

	vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates);                               // detect chars in plates

																										//cv::imshow("imgOriginalScene", imgOriginalScene);           // show scene image

	if (vectorOfPossiblePlates.empty()) {                                               // if no plates were found
		std::cout << std::endl << "no license plates" << std::endl; c++;      // inform user no plates were found
		std::cout << std::endl << "-----------------------------------------" << std::endl;
	//	std::cout << std::endl << c << std::endl;
	}
	else {                                                                            // else
																					  // if we get in here vector of possible plates has at leat one plate

																					  // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
		std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);

		// suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
		PossiblePlate licPlate = vectorOfPossiblePlates.front();

		//cv::imshow("imgPlate", licPlate.imgPlate);            // show crop of plate and threshold of plate
		//cv::imshow("imgThresh", licPlate.imgThresh);

		if (licPlate.strChars.length() == 0) {                                                      // if no chars were found in the plate
			std::cout << std::endl << "no characters" << std::endl << std::endl; c++;      // show message
			std::cout << std::endl << "-----------------------------------------" << std::endl;
//			std::cout << std::endl << c << std::endl;
			return(0);                                                                              // and exit program
		}

		drawRedRectangleAroundPlate(imgOriginalScene, licPlate);                // draw red rectangle around plate

		std::cout << std::endl << "Analyzed License Plate Number = " << licPlate.strChars << std::endl;   c++;  // write license plate text to std out
	//	std::cout << std::endl << c << std::endl;
		std::string c_str();
		///////////////////////////////////
		dbupdate(licPlate, a); //db���� car_num�� �м��� ��ȣ�� ��ȣ �־��ֱ�
		std::cout << std::endl << "-----------------------------------------" << std::endl;
		if (licPlate.strChars == number) {
			return (1);
		}
		//writeLicensePlateCharsOnImage(imgOriginalScene, licPlate);              // write license plate text on the image
		return(0);
																				//cv::imshow("imgOriginalScene", imgOriginalScene);                       // re-show scene image

																				//cv::imwrite("imgOriginalScene.png", imgOriginalScene);                  // write image out to file
																				//return licPlate.strChars;
	}

	cv::waitKey(0);                 // hold windows open until user presses a key


	return(0);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
int GetTimeT(int year, int month, int day, int hour, int minute, int second) {
	time_t tm_st;
	time_t tm_nd;
	int tm_day, tm_hour, tm_min, tm_sec;
	//double mov_dis;
	double d_diff;

	struct tm t = { 0 };
	t.tm_year = year - 1900;
	t.tm_mon = month - 1;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = second;

	tm_st = mktime(&t);
	time(&tm_nd);

	d_diff = difftime(tm_nd, tm_st);
	tm_day = d_diff / (60 * 60 * 24);
	d_diff = d_diff - (tm_day * 60 * 60 * 24);

	tm_hour = d_diff / (60 * 60);
	d_diff = d_diff - (tm_hour * 60 * 60);

	tm_min = d_diff / 60;
	d_diff = d_diff - (tm_min * 60);
	tm_sec = d_diff;
	mov_dis = ((tm_day * 24) + tm_hour + ((double)tm_min * 1 / 60) + ((double)tm_sec * 1 / 3600)) * 60;//��ռӵ� 60�� ��,
	//printf("\n");
	printf("%d�� %d�� %d�� %d�� %d�� %d�� ���� ���ݱ����� ����ð� : %d�� %d�� %d�� %d��\n\n", year, month, day, hour, minute, second, tm_day, tm_hour, tm_min, tm_sec);
	printf("������ �̵��ݰ� : %f km \n", mov_dis);

	return 0;
	//return mktime(&t);
}
//////
int dbselect(char *a)
{
	int query_stat;
	char query[255];

	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	sprintf(query, SQL_SELECT_ID, a);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_ID))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);

	MYSQL_ROW row;
	//char *num;
	while ((row = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{
			//printf("%s �� ī�޶󿡼� Ž���Ǿ����ϴ�.\n ", row[i] ? row[i] : "NULL");
			printf("Ž���� ī�޶� : %s�� ī�޶�.\n ", row[i]);
			num = row[i];
			Push(atoi(num));
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while�� �ѹ������� �ϳ��� ����Ϸ��� break��
		return (int)row[0];
	}

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	// DB ����ݱ�
	mysql_close(connection);
	return 0;

}
double Max(double *n, int size) {
	double max = n[0];
	for (int i = 1; i < size; i++) {
		if (n[i] > max && n[i]>0)
			max = n[i];
	}
	return max;
}
double Max2(double *n, int size) {
	double temp;
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (n[i] < n[j]) {
				temp = n[j];
				n[j] = n[i];
				n[i] = temp;
			}
			
		}
	}
	for (int i = 0; i < size - 1; i++) {
		in[i] = n[i];
	}

	return 0;
}
char linkselect(int a,char *b)
{
	int query_stat;
	char query[255];
	m = 0;
	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	
	sprintf(query, SQL_SELECT_LINK, a,b);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_LINK))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);

	MYSQL_ROW row;
	//char *num;
	while ((row = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{

			printf("%s\n ", row[i] ? row[i] : "NULL");
			m = Platenum(row[i]); //������ row[i]=pic_link �� ���ؼ� plate number �м�
			if (m == 1) {
				printf("Ž���� ī�޶� : %d�� ī�޶�.\n\n", a);
				//path[1] = a;
				k[i] = a;
				num = row[i];
				Push(a);
				goto EXIT;
			}

		}
		printf("\n");

	}
	if (m != 1) {
		printf("��ȣ�� Ž�� �Ұ�2\n");//ó������ ������ �� Ž���ߴµ� ��ȣ���� ã������ ����

	}
	EXIT:
	
	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	// DB ����ݱ�
	mysql_close(connection);
	return 0;

}
int selectTIMEE2(char *a,int b)
{
	int query_stat;
	char query[255];

	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	sprintf(query, SQL_SELECT_TIME2, a, b);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_TIME2))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);

	//MYSQL_ROW row;

	while ((row1 = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{
			//printf("�ð� : %s .\n ", row1[i] ? row1[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while�� �ѹ������� �ϳ��� ����Ϸ��� break��
		//return (int)row[0];
	}

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	
	mysql_close(connection);
	return 0;

}
int selectTIME2(char *a, int b)
{
	int query_stat;
	char query[255];

	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	sprintf(query, SQL_SELECT_TIME2, a, b);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_TIME2))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);

	MYSQL_ROW row;

	while ((row = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{
			printf("�ð� : %s .\n ", row[i] ? row[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while�� �ѹ������� �ϳ��� ����Ϸ��� break��
		return (int)row[0];
	}

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	char *sArr[10] = { NULL, };
	char *date[10] = { NULL, };
	char *time[10] = { NULL, };
	int s = 0, d = 0, f = 0;
	//" "������� ������
	char *ptr = strtok(row[0], " ");

	while (ptr != NULL) {
		sArr[s] = ptr;
		s++;
		ptr = strtok(NULL, " ");
	}

	/*
	for (int i = 0; i < 10; i++) {
	if (sArr[i] != NULL)
	printf("%s\n", sArr[i]);
	}
	*/
	//"-" ���� ������
	char *ptr1 = strtok(sArr[0], "-");
	while (ptr1 != NULL)
	{
		date[d] = ptr1;
		d++;
		ptr1 = strtok(NULL, "-");
	}
	/*
	for (int i = 0; i < 10; i++) {
	if (date[i] != NULL)
	printf("%s\n", date[i]);
	}
	*/
	int year = atoi(date[0]);
	int month = atoi(date[1]);
	int day = atoi(date[2]);
	//printf("%s hellllllo \n", date[0]);
	//":" ���� ������
	char *ptr2 = strtok(sArr[1], ":");
	while (ptr2 != NULL) {
		time[d] = ptr2;
		d++;
		ptr2 = strtok(NULL, ":");
	}
	/*
	for (int i = 0; i < 10; i++) {
	if (time[i] != NULL)
	printf("%s\n", time[i]);
	}
	*/
	int hour = atoi(time[3]);
	int minute = atoi(time[4]);
	int second = atoi(time[5]);
	//

	GetTimeT(year, month, day, hour, minute, second);
	// DB ����ݱ�
	mysql_close(connection);
	return 0;

}
int selectTIME(char *a)
{
	int query_stat;
	char query[255];
	MYSQL_ROW row;
	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	sprintf(query, SQL_SELECT_TIME, a);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_TIME))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);

	

	while ((row = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{
			printf("�ð� : %s .\n ", row[i] ? row[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while�� �ѹ������� �ϳ��� ����Ϸ��� break��
		//return (int)row1[0];
	}
	//printf("row[0] = %s", row[0]);
	//taketime = row[0];

	//std::cout << taketime;
	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	char *sArr[10] = { NULL, };
	char *date[10] = { NULL, };
	char *time[10] = { NULL, };
	int s = 0, d = 0, f = 0;
	//" "������� ������
	char *ptr = strtok(row[0], " ");

	while (ptr != NULL) {
		sArr[s] = ptr;
		s++;
		ptr = strtok(NULL, " ");
	}

	char *ptr1 = strtok(sArr[0], "-");
	while (ptr1 != NULL)
	{
		date[d] = ptr1;
		d++;
		ptr1 = strtok(NULL, "-");
	}

	int year = atoi(date[0]);
	int month = atoi(date[1]);
	int day = atoi(date[2]);

	char *ptr2 = strtok(sArr[1], ":");
	while (ptr2 != NULL) {
		time[d] = ptr2;
		d++;
		ptr2 = strtok(NULL, ":");
	}

	int hour = atoi(time[3]);
	int minute = atoi(time[4]);
	int second = atoi(time[5]);


	GetTimeT(year, month, day, hour, minute, second);
	// DB ����ݱ�
	mysql_close(connection);
	return 0;

}

int selectTIMEE(char *a)
{
	int query_stat;
	char query[255];
	
	MYSQL *connection = NULL, conn;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	sprintf(query, SQL_SELECT_TIME, a);
	//cout << query << endl;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	if (mysql_query(&conn, SQL_SELECT_TIME))
	{
		mysql_error(&conn);
	}
	MYSQL_RES *sql_result = mysql_store_result(&conn);
	int num_fields = mysql_num_fields(sql_result);



	while ((row1 = mysql_fetch_row(sql_result)))
	{
		for (int i = 0; i < num_fields; i++) //num_fields �� ����ϰ��� �ϴ� �ʵ尳��
		{
			//printf("�ð� : %s .\n ", row1[i] ? row1[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while�� �ѹ������� �ϳ��� ����Ϸ��� break��
				//return (int)row1[0];
	}
	//printf("row1[0] = %s", row1[0]);
	//taketime = row1[0];

	//std::cout << taketime;
	if (query_stat != 0)
	{
		cout << "����" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	
	mysql_close(connection);
	return 0;

}

void Push(int nData) {

	int nCount = 0;

	for (int i = 0; i < STACK_SIZE; i++) {

		if (pStack[i] == NULL)

		{

			break;

		}

		nCount++;

	}



	if (nCount != STACK_SIZE) {

		pStack[nCount] = nData;

	}



}



int Pop() {

	int nCount = STACK_SIZE - 1;

	int nResult = 0;

	for (int i = STACK_SIZE - 1; i >0; i--) {

		if (pStack[i] != NULL)

		{

			break;

		}

		nCount--;

	}

	if (nCount != -1) {

		nResult = pStack[nCount];

		pStack[nCount] = NULL;

	}

	return nResult;

}


