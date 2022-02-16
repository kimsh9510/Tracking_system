#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <mysql.h>
#include "Main.h"
#include "db.h"
#include "dbinsert.h"
#include "GetDistance.h"
#include "GetTimeT.h"
#include "select.h"
#include <time.h>
#include <string>
#pragma comment(lib,"libmySQL.lib")
#define SQL_INSERT_RECORD "INSERT INTO cam_info VALUES \ (%d, '%s' , %s, %s,%s, '%s', %s, %s );" 
//#define SQL_SELECT_RECORD "SELECT cam_id,time FROM cam_info WHERE cam_dis <= %d AND pic_link = 'Z:/HDD1/Data/img_00000%d.jpg';"


//platenum 함수 가져오고싶다


void drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
	cv::Point2f p2fRectPoints[4];

	licPlate.rrLocationOfPlateInScene.points(p2fRectPoints);            // get 4 vertices of rotated rect

	for (int i = 0; i < 4; i++) {                                       // draw 4 red lines
		cv::line(imgOriginalScene, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_RED, 2);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
	cv::Point ptCenterOfTextArea;                   // this will be the center of the area the text will be written to
	cv::Point ptLowerLeftTextOrigin;                // this will be the bottom left of the area that the text will be written to

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;                              // choose a plain jane font
	double dblFontScale = (double)licPlate.imgPlate.rows / 30.0;            // base font scale on height of plate area
	int intFontThickness = (int)std::round(dblFontScale * 1.5);             // base font thickness on font scale
	int intBaseline = 0;

	cv::Size textSize = cv::getTextSize(licPlate.strChars, intFontFace, dblFontScale, intFontThickness, &intBaseline);      // call getTextSize

	ptCenterOfTextArea.x = (int)licPlate.rrLocationOfPlateInScene.center.x;         // the horizontal location of the text area is the same as the plate

	if (licPlate.rrLocationOfPlateInScene.center.y < (imgOriginalScene.rows * 0.75)) {      // if the license plate is in the upper 3/4 of the image
																							// write the chars in below the plate
		ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) + (int)std::round((double)licPlate.imgPlate.rows * 1.6);
	}
	else {                                                                                // else if the license plate is in the lower 1/4 of the image
																						  // write the chars in above the plate
		ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) - (int)std::round((double)licPlate.imgPlate.rows * 1.6);
	}

	ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2));           // calculate the lower left origin of the text area
	ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2));          // based on the text area center, width, and height

																							// write the text on the image
	cv::putText(imgOriginalScene, licPlate.strChars, ptLowerLeftTextOrigin, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);

}
