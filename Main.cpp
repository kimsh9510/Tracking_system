// db실행되면서, 번호판인식 합한것. 최종MiR 탐색방법
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
//db.h에 선언되어있음.
#define SQL_INSERT_RECORD "INSERT INTO cam_info VALUES \ (%d, '%s' , %s, %s,%s, '%s', %s, %s );" 
#define SQL_SELECT_RECORD "SELECT cam_id,time FROM cam_info WHERE cam_dis <= %d AND pic_link = 'Z:/HDD1/Data/img_00000%d.jpg';"
#define SQL_SELECT_ID "SELECT cam_id FROM cam_info WHERE car_num = '%s';"
#define SQL_SELECT_LINK "SELECT pic_link FROM cam_info WHERE cam_id = %d AND time >= '%s';"
#define STACK_SIZE 3
#define SQL_UPDATE_RECORD "UPDATE cam_info SET car_num = '%s' WHERE pic_link = '%s' ;"
#define SQL_SELECT_TIME2 "SELECT time FROM cam_info WHERE car_num = '%s' AND cam_id = %d;"
#define MAX_STACK_SIZE 10 //path경로 몇개 출력할지 안씀
//#define SQL_UPDATE_DIS "UPDATE cam_info SET dis = '%d' WHERE cam_dis <= %d ;"
int Platenum(string a);
int GetTimeT(int year, int month, int day, int hour, int minute, int second);
double Max(double *n, int size);
int selectTIME2(char *a, int b);
int selectTIMEE2(char *a, int b);
void Push(int nData);
int Pop();
int *pStack = NULL;

//전역변수
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
	// 초기화
	for (int i = 0; i < STACK_SIZE; i++) {
		pStack[i] = NULL;
	}
	//printf("%s\n", mysql_get_client_info());// 연결된지 보려고 버전출력
	//char a[256];
	int b[256];
	tagPT loc[6];//20
	
	//카메라1의 위치 it4
	loc[1].y = 35.889147;
	loc[1].x = 128.610314;
	//카메라2의 위치 산격초
	loc[2].y = 35.894074;
	loc[2].x = 128.606904;
	//카메라3의 위치 성화여고
	loc[3].y = 35.902965;
	loc[3].x = 128.618665;
	//카메라4의 위치 북중학교
	loc[4].y = 35.897390;
	loc[4].x = 128.620502;
	//카메라5의 위치 경상고,, 러시아어디
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
	
	printf("찾고자 하는 번호판의 번호를 입력하세요 : ");
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
	//값 가져오는 부분
	if (mysql_query(con, "SELECT pic_link FROM cam_info"))//////////////이부분에서 WHERE 넣어서 탐색 원하는 시간대 조정해보기
	{
		finish_with_error(con);
	}
	// mysql_store_result함수로 result set을 가져오고
	// result set을 MYSQL_RES 구조체에 저장한다

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL)
	{
		finish_with_error(con);
	}
	// mysql_num_fields 함수로 테이블의 Column 수를 알아낸다
	int num_fields = mysql_num_fields(result);
	// row가 없을때까지 반복해서 row를 가져오면서
	// 영역의 수만큼 row의 내용을 콘솔에 출력한다
	MYSQL_ROW row;
	//string num[100];
	//string A = "LHF";
	while (row = mysql_fetch_row(result))
	{
		for (int i = 0; i < num_fields; i++)
		{
			printf("%s\n ", row[i] ? row[i] : "NULL");
			m = Platenum(row[i]); //가져온 row[i]=pic_link 에 대해서 plate number 분석
			if (m == 1) {
				goto EXIT;
			}
		}
		printf("\n");
	}
	if (m != 1) {
		printf("번호판 탐색 불가\n");//처음부터 끝까지 다 탐색했는데 번호판을 찾을수가 없어
		goto EXIT2;//끝내기
	}
	EXIT: //원하는 번호판을 찾으면 번호판인식 멈춤

	//dbselect("UAI");
	dbselect(number);
	//printf("num출력 : %s\n", num);
	selectTIME(number); // 시간, 이동반경
	selectTIMEE(number); // 시간
	
	//num이용계산 반경내에 있는 카메라 id 구하기
	int car_id = atoi(num); //경로에 추가
	k[5] = { 0, };
	//int in[5];//반경 내 카메라 모으기
	k[0] = car_id;//계속 추가해야돼 
	//반경내에 있는 카메라 거리를 배열 car_in[]에 넣고 나머지 값은 0으로 채운다.
	for (int c = 1; c <= 5; c++) { //원래 3
	
			if (Dis[car_id][c] < mov_dis && car_id != c) {
				//printf("반경내에 있는 카메라번호 = %d 이고 Dis[%d][%d] = %f 이다.\n", c, car_id, c, Dis[car_id][c]);
				printf("반경내에 있는 카메라번호 = %d 이고 카메라간 거리 = %f.\n\n",c, Dis[car_id][c]);
			
				//in[l] = c;
				car_in[l] = Dis[car_id][c]; //반경 내 거리넣기
				l++;
			}

			for (i = l; i < 5; i++) {
				car_in[i] = NULL;
			}
		
	}
	//printf("l : %d\n", l);
	/*거리 배열에 거리값 넣기
	for (i = 0; i < sizeof(car_in)/sizeof(double); i++) {
		printf("size = %d car_in = %f\n",sizeof(car_in)/sizeof(double), car_in[i]);
	}
	*/

	//이동반경 내 카메라 중에 거리가 더 긴 카메라찾기
	double max = Max(car_in, sizeof(car_in) / sizeof(double));
	//printf("max = %f\n", max);

	Max2(car_in, sizeof(car_in) / sizeof(double));
	for (int i = 0; i < 5; i++) {
		printf("[in : %f]\n", in[i]);
	}
	
	for (k[1] = 1; k[1] <= 5; k[1]++) { //원래 3
		if (max == Dis[car_id][k[1]])
			break;
	}
	printf("반경 내 최대 Ce 값을 가지는 카메라 번호 = %d\n\n", k[1]);  //k는 다음 탐색을 시작해야할 카메라
	printf("-----------------------------------------\n\n");
	///////////////k에 대해 다시탐색!! 
	
	for (int d = 1; d < 3; d++) {//d에 몇개까지 경로 찾고 싶은지 입력하면된다.
		//#define SQL_SELECT_LINK "SELECT pic_link FROM cam_info WHERE cam_id = %d AND time >= TO_DATE('2020-01-31 10:23:01') ;"
		printf("d=%d", d);
		linkselect(k[d],row1[0]); //k번 카메라에 대해 이미지 번호판 인식=>db넣기
		if (m != 1) {
			//이까지 들어왔어 car_id?////////////////
			for (k[1] = 1; k[1] <= 5; k[1]++) { //원래 3
				if (in[1] == Dis[car_id][k[1]])//in[1] 이 두번째 거리가 먼 카메라
				{
					printf("여기k[1]=%d", k[1]);
					break;
				}
				else if (in[1] == 0)//in[1] 즉 두번째 카메라가 없으면 탐색을 더 안해도 된다..
					goto EXIT3;
			}
			linkselect(k[d], row1[0]);
			if (m != 1) {
				goto EXIT3;
			}
		}
		selectTIME2(number, k[d]); //db에 number 인식되고, 카메라 번호k 이면 경과시간찾기
		selectTIMEE2(number,k[d]);
		l = 0;

		for (int c = 1; c <= 5; c++) { //원래 3

			if (Dis[k[1]][c] < mov_dis && k[d] != c) {
				printf("반경내에 있는 카메라 번호 = %d 이고 카메라간 거리 = %f\n\n", c, Dis[k[d]][c]);
			
				car_in[l] = Dis[k[d]][c]; //반경 내 거리넣기
				l++;
			}
			for (i = l; i < 5; i++) {
				car_in[i] = NULL;
			}
		}
		for (i = 0; i < sizeof(car_in) / sizeof(double); i++) {
			//printf("size = %d car_in = %f\n", sizeof(car_in) / sizeof(double), car_in[i]);
		}
		//이동반경 내 카메라 중에 거리가 더 긴 카메라찾기
		max = Max(car_in, sizeof(car_in) / sizeof(double));
		//printf("max = %f\n", max);

	    Max2(car_in, sizeof(car_in) / sizeof(double));
		for (int i = 0; i < 5; i++) {
			printf("다음[in : %f]\n", in[i]);
		}
		for (k[d+1] = 1; k[d+1] <= 5; k[d+1]++) {  //원래 3
			if (max == Dis[k[d]][k[d+1]])
				break;
		}
		printf("반경 내 최대 Ce 값을 가지는 카메라 번호2 = %d\n\n", k[d+1]);  //k는 다음 탐색을 시작해야할 카메라
		printf("-----------------------------------------\n\n");

		//printf("k[d+1]=%d\n", k[d + 1]);
	}
	EXIT3:
	printf("차량의 이동경로 : ");

	/*for (int path = 1; path < 4; path++) {
		printf("%d 번 카메라->", k[path]);
	}*/
	for (int i = 0; i < STACK_SIZE; i++) {
		printf("%d 번 카메라",pStack[i]);
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
	printf("걸린 시간: %f sec\n\n", time);
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
											  //imgOriginalScene = cv::imread();   //음 buf=이미지 경로를 바로 읽어들일 수 있게 
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
		dbupdate(licPlate, a); //db에서 car_num에 분석한 번호판 번호 넣어주기
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
	mov_dis = ((tm_day * 24) + tm_hour + ((double)tm_min * 1 / 60) + ((double)tm_sec * 1 / 3600)) * 60;//평균속도 60일 때,
	//printf("\n");
	printf("%d년 %d월 %d일 %d시 %d분 %d초 부터 지금까지는 경과시간 : %d일 %d시 %d분 %d초\n\n", year, month, day, hour, minute, second, tm_day, tm_hour, tm_min, tm_sec);
	printf("차량의 이동반경 : %f km \n", mov_dis);

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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{
			//printf("%s 번 카메라에서 탐색되었습니다.\n ", row[i] ? row[i] : "NULL");
			printf("탐색한 카메라 : %s번 카메라.\n ", row[i]);
			num = row[i];
			Push(atoi(num));
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while문 한바퀴돌고 하나만 출력하려고 break함
		return (int)row[0];
	}

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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{

			printf("%s\n ", row[i] ? row[i] : "NULL");
			m = Platenum(row[i]); //가져온 row[i]=pic_link 에 대해서 plate number 분석
			if (m == 1) {
				printf("탐색한 카메라 : %d번 카메라.\n\n", a);
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
		printf("번호판 탐색 불가2\n");//처음부터 끝까지 다 탐색했는데 번호판을 찾을수가 없어

	}
	EXIT:
	
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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{
			//printf("시간 : %s .\n ", row1[i] ? row1[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while문 한바퀴돌고 하나만 출력하려고 break함
		//return (int)row[0];
	}

	if (query_stat != 0)
	{
		cout << "여기" << endl;
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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{
			printf("시간 : %s .\n ", row[i] ? row[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while문 한바퀴돌고 하나만 출력하려고 break함
		return (int)row[0];
	}

	if (query_stat != 0)
	{
		cout << "여기" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	char *sArr[10] = { NULL, };
	char *date[10] = { NULL, };
	char *time[10] = { NULL, };
	int s = 0, d = 0, f = 0;
	//" "공백기준 나누기
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
	//"-" 기준 나누기
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
	//":" 기준 나누기
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
	// DB 연결닫기
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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{
			printf("시간 : %s .\n ", row[i] ? row[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while문 한바퀴돌고 하나만 출력하려고 break함
		//return (int)row1[0];
	}
	//printf("row[0] = %s", row[0]);
	//taketime = row[0];

	//std::cout << taketime;
	if (query_stat != 0)
	{
		cout << "여기" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	char *sArr[10] = { NULL, };
	char *date[10] = { NULL, };
	char *time[10] = { NULL, };
	int s = 0, d = 0, f = 0;
	//" "공백기준 나누기
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
	// DB 연결닫기
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
		cout << "여기" << endl;
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
		for (int i = 0; i < num_fields; i++) //num_fields 는 출력하고자 하는 필드개수
		{
			//printf("시간 : %s .\n ", row1[i] ? row1[i] : "NULL");
			//printf("%s\n ", row[i] ? row[i] : "NULL");
		}
		printf("\n");


		break;  //while문 한바퀴돌고 하나만 출력하려고 break함
				//return (int)row1[0];
	}
	//printf("row1[0] = %s", row1[0]);
	//taketime = row1[0];

	//std::cout << taketime;
	if (query_stat != 0)
	{
		cout << "여기" << endl;
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


