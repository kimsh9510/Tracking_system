// 시간을 받아오는 구조체
// 시간 가져와서 현재시간과 비교하고 시간이 얼마나 지났는지 출력, 차량의 이동거리 출력
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdio.h>


char* timeToString(struct tm *t) {
	static char s[20];

	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec
	);

	return s;
}
/*mov_dis 쓰려고 전역변수로 main에 가져다둠
double mov_dis;
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
	printf("\n");
	printf("%d년 %d월 %d일 %d시 %d분 %d초 부터 지금까지는 %d일 %d시 %d분 %d초 지났음\n", year, month, day, hour, minute, second, tm_day, tm_hour, tm_min, tm_sec);
	printf("차량의 이동반경은 %f km 입니다\n\n", mov_dis);

	return 0;
	//return mktime(&t);
}
*/