//경위도 coordinates 값으로 거리 구하기

#define rad(x) x*3.14159/180.0
#include <math.h>
#include "GetDistance.h"

double GetDistance(tagPT cd1, tagPT cd2)
{
	int radius = 6371; //지구평균 반지름 km

	double dLat = rad((cd2.y - cd1.y));
	double dLon = rad((cd2.x - cd1.x));

	cd1.y = rad(cd1.y);
	cd2.y = rad(cd2.y);

	double a = sin(dLat / 2) * sin(dLat / 2) + sin(dLon / 2) * sin(dLon / 2) * cos(cd1.y) * cos(cd2.y);
	double c = 2 * atan2f(sqrtf(a), sqrtf(1 - a));
	double dDistance = radius * c;

	//dDistance *= 1000;  //주석처리하면 km값, 아니면 m값

	return dDistance;
}
