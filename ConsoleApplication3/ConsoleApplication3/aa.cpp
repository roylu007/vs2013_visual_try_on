#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
using namespace cv;
int threshval = 160;
int pointinterval = 200;
vector<Point>contoursPoint1;
vector<Point>contoursPoint2;
int M = 10;
void ProgramInitial(){
	contoursPoint1.clear();
	contoursPoint2.clear();
}
double getDistance(Point p1, Point p2){
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
double getCosAngle2(Point pt1, Point pt2, Point pt3, Point pt4){
	Point p1 = pt2 - pt1;
	Point p2 = pt4 - pt3;
	return (p1.x*p2.x + p1.y*p2.y) / sqrt((p1.x*p1.x + p1.y*p1.y)*(p2.x*p2.x + p2.y*p2.y) + 1e-10);
}
//double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )  from pt0->pt1 and from pt0->pt2
double getCosAngle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
Point getPointP0(vector<Point>contoursPoint, Point v0){//P0  --   neck of head
	int cnt = 0;
	int lens = contoursPoint.size();
	double maxCosAngle = 0;
	int i = find(contoursPoint.begin(), contoursPoint.end(), v0) - contoursPoint.begin();
	int j = 0;
	Point temp;
	Point pt(v0.x, v0.y + 1);
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 1.5* M; j++){
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		double tempCosAngle = getCosAngle(contoursPoint[j], pt, v0);
		//cout << "cosangle=" << tempCosAngle << endl;
		if (maxCosAngle < tempCosAngle && fabs(maxCosAngle - tempCosAngle)>1e-6){
			maxCosAngle = tempCosAngle;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP26(vector<Point>contoursPoint, Point v0){//P0  --   neck of head
	int cnt = 0;
	int lens = contoursPoint.size();
	double maxCosAngle = 0;
	int i = find(contoursPoint.begin(), contoursPoint.end(), v0) - contoursPoint.begin();
	int j = 0;
	Point temp;
	Point pt(v0.x, v0.y + 1);
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt < 1.5* M; j--){
		cnt++;
		j = j>0 ? j : lens + j;
		cout << j << endl;
		double tempCosAngle = getCosAngle(contoursPoint[j], pt, v0);
		//cout << "cosangle=" << tempCosAngle << endl;
		if (maxCosAngle < tempCosAngle && fabs(maxCosAngle - tempCosAngle)>1e-6){
			maxCosAngle = tempCosAngle;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP25(vector<Point>contoursPoint, Point p26){//P0  --   neck of head
	int cnt = 0;
	int lens = contoursPoint.size();
	double minCosAngle = 1;
	int i = find(contoursPoint.begin(), contoursPoint.end(), p26) - contoursPoint.begin();
	int j = 0;
	Point temp;
	Point pt(p26.x, p26.y + 1);
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt <1.5* M; j--){
		cnt++;
		j = j>0 ? j : lens + j;
		//cout << j << endl;
		double tempCosAngle = getCosAngle(contoursPoint[j], pt, p26);
		//cout << "cosangle=" << tempCosAngle << endl;
		if (minCosAngle > tempCosAngle && fabs(minCosAngle - tempCosAngle) > 1e-6){
			minCosAngle = tempCosAngle;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP1(vector<Point>contoursPoint, Point p0){
	int cnt = 0;
	int lens = contoursPoint.size();
	double minCosAngle = 1;
	int i = find(contoursPoint.begin(), contoursPoint.end(), p0) - contoursPoint.begin();
	int j = 0;
	Point temp;
	Point pt(p0.x, p0.y + 1);
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 1.5* M; j++){//if i>size-1 then i start from 0
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		double tempCosAngle = getCosAngle(contoursPoint[j], pt, p0);
		//cout << "cosangle=" << tempCosAngle << endl;
		if (minCosAngle > tempCosAngle && fabs(minCosAngle - tempCosAngle) > 1e-6){
			minCosAngle = tempCosAngle;
			temp = contoursPoint[j];
		}
	}
	return temp;
}

Point getPointV0(vector<Point>contoursPoint){//V0  --   top of head 
	Point temp;
	int min = 1024;
	for (int i = 0; i < contoursPoint.size(); i++){
		if (contoursPoint[i].y < min){
			min = contoursPoint[i].y;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointV1(vector<Point>contoursPoint, Point p1){//V1  --   right_fingertip   point.y  ->max  during 4M
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), p1) - contoursPoint.begin();
	int j = 0;
	int maxy = 0;
	Point temp;
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 4 * M; j++){
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		if (maxy < contoursPoint[j].y){
			maxy = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointV2(vector<Point>contoursPoint, Point p25){//V2  --   left_fingertip   point.x  ->max
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), p25) - contoursPoint.begin();
	int j = 0;
	int maxy = 0;
	Point temp;
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt <4 * M; j--){
		cnt++;
		j = j>0 ? j : lens + j;
		if (maxy < contoursPoint[j].y){
			maxy = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP2(vector<Point>contoursPoint, Point p1, Point v1){
	double p1v1Dis = getDistance(p1, v1);
	double minDis = p1v1Dis;
	Point temp;
	int from = find(contoursPoint.begin(), contoursPoint.end(), p1) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v1) - contoursPoint.begin();
	for (int i = from + 1; i < to; i++){
		double PFromdis = getDistance(contoursPoint[i], p1);
		double PToDis = getDistance(contoursPoint[i], v1);
		double diff1 = fabs(PFromdis - p1v1Dis * 2.0 / 5.0);
		double diff2 = fabs(PToDis - p1v1Dis * 3.0 / 5.0);
		double diff = diff1 + diff2;
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	//temp = contoursPoint[from + (to - from) / 3];
	return temp;
}
Point getPointP24(vector<Point>contoursPoint, Point v2, Point p25){
	double p1v1Dis = getDistance(v2, p25);
	double minDis = p1v1Dis;
	Point temp;
	int from = find(contoursPoint.begin(), contoursPoint.end(), v2) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p25) - contoursPoint.begin();
	for (int i = from + 1; i < to; i++){
		double PFromdis = getDistance(contoursPoint[i], v2);
		double PToDis = getDistance(contoursPoint[i], p25);
		double diff1 = fabs(PFromdis - p1v1Dis *3.0 / 5.0);
		double diff2 = fabs(PToDis - p1v1Dis *2.0 / 3.0);
		double diff = diff1 + diff2;
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	//temp = contoursPoint[from + (to - from) / 3];
	return temp;
}
Point getPointP3(vector<Point>contoursPoint, Point p1, Point p2, Point v1){
	double p1p2Dis = getDistance(p1, p2);
	double p2v1Dis = getDistance(v1, p2);
	double minDis = p2v1Dis;
	Point temp;
	int from = find(contoursPoint.begin(), contoursPoint.end(), p2) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v1) - contoursPoint.begin();
	//double p1p2Dis = getDistance(p1, p2);
	/*for (int i = from + 1; i < to; i++){
		double PFromDis = getDistance(contoursPoint[i], p2);
		double PToDis = getDistance(contoursPoint[i], v1);
		double diff1 = fabs(PFromDis - p2v1Dis*2.0/3.0);
		double diff2 = fabs(PToDis - p2v1Dis / 3.0);
		double diff = diff1+diff2;
		if (minDis>diff && fabs(minDis - diff)>1e-6){
		minDis = diff;
		temp = contoursPoint[i];
		}
		}*/
	for (int i = from + 1; i < to; i++){
		double PP2Dis = getDistance(contoursPoint[i], p2);
		double diff = fabs(p1p2Dis*0.80447 - PP2Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	//temp = contoursPoint[from + (to - from) / 3];
	return temp;
}
Point getPointP23(vector<Point>contoursPoint, Point p24, Point p25, Point v2){
	double p24p25Dis = getDistance(p24, p25);
	double v2p24Dis = getDistance(v2, p24);
	double minDis = v2p24Dis;
	Point temp;
	int from = find(contoursPoint.begin(), contoursPoint.end(), v2) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p24) - contoursPoint.begin();
	//double p1p2Dis = getDistance(p1, p2);
	/*for (int i = from + 1; i < to; i++){
		double PFromDis = getDistance(contoursPoint[i], v2);
		double PToDis = getDistance(contoursPoint[i], p24);
		double diff1 = fabs(PFromDis - v2p24Dis / 3.0);
		double diff2 = fabs(PToDis - v2p24Dis *2.0 / 3.0);
		double diff = diff1 + diff2;
		if (minDis>diff && fabs(minDis - diff)>1e-6){
		minDis = diff;
		temp = contoursPoint[i];
		}
		}*/
	for (int i = from + 1; i < to; i++){
		double PP24Dis = getDistance(contoursPoint[i], p24);
		double diff = fabs(p24p25Dis*0.80447 - PP24Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	//temp = contoursPoint[from + (to - from) / 3];
	return temp;
}
Point getPointP4(vector<Point>contoursPoint, Point p2, Point p3, Point v1, Point p6){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v1) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p6) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP3P2CosAngle = getCosAngle(p2, contoursPoint[i], p3);
		double diff = fabs(PP3P2CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP5(vector<Point>contoursPoint, Point p1, Point p2, Point v1, Point p6){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v1) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p6) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP3P2CosAngle = getCosAngle(p1, contoursPoint[i], p2);
		double diff = fabs(PP3P2CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP21(vector<Point>contoursPoint, Point p25, Point p24, Point p20, Point v2){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p20) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v2) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP3P2CosAngle = getCosAngle(p25, contoursPoint[i], p24);
		double diff = fabs(PP3P2CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP7(vector<Point>contoursPoint, Point p5, Point p6, Point v3){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p6) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	Point temp;
	double p5p6Dis = getDistance(p5, p6);
	double minDis = p5p6Dis;
	for (int i = from + 1; i < to; i++){
		double PP6Dis = getDistance(contoursPoint[i], p6);
		double diff = fabs(p5p6Dis - PP6Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP19(vector<Point>contoursPoint, Point p21, Point p20, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p20) - contoursPoint.begin();
	Point temp;
	double p20p21Dis = getDistance(p20, p21);
	double minDis = p20p21Dis;
	for (int i = from + 1; i < to; i++){
		double PP20Dis = getDistance(contoursPoint[i], p20);
		double diff = fabs(p20p21Dis - PP20Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP8(vector<Point>contoursPoint, Point p6, Point p7, Point v3){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p7) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	Point temp;
	double p6p7Dis = getDistance(p6, p7);
	double minDis = p6p7Dis;
	for (int i = from + 1; i < to; i++){
		double PP20Dis = getDistance(contoursPoint[i], p7);
		double diff = fabs(p6p7Dis - PP20Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP9(vector<Point>contoursPoint, Point p6, Point p8, Point v3){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p8) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	Point temp;
	double p6p8Dis = getDistance(p6, p8);
	double minDis = p6p8Dis;
	for (int i = from + 1; i < to; i++){
		double PP8Dis = getDistance(contoursPoint[i], p8);
		double diff = fabs(p6p8Dis - PP8Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP10(vector<Point>contoursPoint, Point p8, Point p9, Point v3){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p9) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	Point temp;
	double p8p9Dis = getDistance(p9, p8);
	double minDis = p8p9Dis;
	for (int i = from + 1; i < to; i++){
		double PP8Dis = getDistance(contoursPoint[i], p9);
		double diff = fabs(p8p9Dis*0.8885 - PP8Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP11(vector<Point>contoursPoint, Point p9, Point p10, Point v3, Point p13){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p13) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP10P9CosAngle = getCosAngle(p9, contoursPoint[i], p10);
		double diff = fabs(PP10P9CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP12(vector<Point>contoursPoint, Point p8, Point p9, Point v3, Point p13){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p13) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP9P8CosAngle = getCosAngle(p8, contoursPoint[i], p9);
		double diff = fabs(PP9P8CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
/*Point getPointP10(vector<Point>contoursPoint, Point p9, Point p13, Point v3){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p9) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	Point temp;
	double p13p9YDiff = fabs(1.0*(p13.y - p9.y));
	double minYDiff = p13p9YDiff;
	for (int i = from + 1; i < to; i++){
	double PP9YDiff = fabs(1.0*(p9.y-contoursPoint[i].y));
	double diff = fabs(PP9YDiff - p13p9YDiff);
	if (minYDiff>diff && fabs(minYDiff - diff)>1e-6){
	minYDiff = diff;
	temp = contoursPoint[i];
	}
	}
	return temp;
	}*/
Point getPointP18(vector<Point>contoursPoint, Point p20, Point p19, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p19) - contoursPoint.begin();
	Point temp;
	double p19p20Dis = getDistance(p20, p19);
	double minDis = p19p20Dis;
	for (int i = from + 1; i < to; i++){
		double PP19Dis = getDistance(contoursPoint[i], p19);
		double diff = fabs(p19p20Dis - PP19Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP17(vector<Point>contoursPoint, Point p20, Point p18, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p18) - contoursPoint.begin();
	Point temp;
	double p18p20Dis = getDistance(p20, p18);
	double minDis = p18p20Dis;
	for (int i = from + 1; i < to; i++){
		double PP18Dis = getDistance(contoursPoint[i], p18);
		double diff = fabs(p18p20Dis - PP18Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP16(vector<Point>contoursPoint, Point p18, Point p17, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), p17) - contoursPoint.begin();
	Point temp;
	double p17p18Dis = getDistance(p17, p18);
	double minDis = p17p18Dis;
	for (int i = from + 1; i < to; i++){
		double PP17Dis = getDistance(contoursPoint[i], p17);
		double diff = fabs(p17p18Dis *0.8885 - PP17Dis);
		if (minDis>diff && fabs(minDis - diff) > 1e-6){
			minDis = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP15(vector<Point>contoursPoint, Point p17, Point p16, Point p13, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p13) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP16P17CosAngle = getCosAngle(p17, contoursPoint[i], p16);
		double diff = fabs(PP16P17CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP14(vector<Point>contoursPoint, Point p18, Point p17, Point p13, Point v4){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p13) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v4) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP17P18CosAngle = getCosAngle(p18, contoursPoint[i], p17);
		double diff = fabs(PP17P18CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}

Point getPointP22(vector<Point>contoursPoint, Point p24, Point p23, Point p20, Point v2){
	int from = find(contoursPoint.begin(), contoursPoint.end(), p20) - contoursPoint.begin();
	int to = find(contoursPoint.begin(), contoursPoint.end(), v2) - contoursPoint.begin();
	Point temp;
	double minCosAngle = 1;
	for (int i = from + 1; i < to; i++){
		double PP3P2CosAngle = getCosAngle(p24, contoursPoint[i], p23);
		double diff = fabs(PP3P2CosAngle) - 0;
		if (minCosAngle>diff && fabs(minCosAngle - diff) > 1e-6){
			minCosAngle = diff;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointP6(vector<Point>contoursPoint, Point v1){
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), v1) - contoursPoint.begin();
	int j = 0;
	int miny = 1024;
	Point temp;
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 2 * M; j++){
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		if (miny > contoursPoint[j].y){
			miny = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP20(vector<Point>contoursPoint, Point v2){
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), v2) - contoursPoint.begin();
	int j = 0;
	int miny = 1024;
	Point temp;
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt <2 * M; j--){
		cnt++;
		j = j>0 ? j : lens + j;
		if (miny > contoursPoint[j].y){
			miny = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointV3(vector<Point>contoursPoint, Point p6){  //v3 -- right_toe Point maxy
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), p6) - contoursPoint.begin();
	int j = 0;
	int maxy = 0;
	Point temp;
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 4 * M; j++){
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		if (maxy < contoursPoint[j].y){
			maxy = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointV4(vector<Point>contoursPoint, Point p20){ //v4 -- left_toe Point maxy
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), p20) - contoursPoint.begin();
	int j = 0;
	int maxy = 0;
	Point temp;
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt <4 * M; j--){
		cnt++;
		j = j>0 ? j : lens + j;
		if (maxy < contoursPoint[j].y){
			maxy = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
Point getPointP13(vector<Point>contoursPoint, Point v3){
	int cnt = 0;
	int lens = contoursPoint.size();
	int i = find(contoursPoint.begin(), contoursPoint.end(), v3) - contoursPoint.begin();
	int j = 0;
	int miny = 1024;
	Point temp;
	for (int j = i + 1 > lens - 1 ? i + 1 - lens : i + 1; cnt < 4 * M; j++){
		cnt++;
		j = j>lens - 1 ? j - lens : j;
		if (miny > contoursPoint[j].y){
			miny = contoursPoint[j].y;
			temp = contoursPoint[j];
		}
	}
	return temp;
}
void on_mouse(int event, int x, int y, int flags, void* img)
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		IplImage *timg = cvCloneImage((IplImage *)img);
		CvPoint pt = cvPoint(x, y);
		char temp[16];
		sprintf(temp, "(%d,%d)", x, y);
		cvPutText(timg, temp, pt, &font, CV_RGB(250, 0, 0));
		cvCircle(timg, pt, 2, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		cvShowImage("src", timg);
		cvReleaseImage(&timg);
	}
}
void fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
void getSizeContours(vector<vector<Point>> &contours)
{
	int cmin = 100;   // 最小轮廓长度  
	int cmax = 1000;   // 最大轮廓长度  
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) < cmin || (itc->size()) > cmax)
		{
			itc = contours.erase(itc);
		}
		else ++itc;
	}
}
void getBodyContoursPoint(vector<vector<Point>> &contours)
{
	int cmax = 0;   // 最大轮廓长度  
	vector<Point>temp;
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) > cmax)
		{
			cmax = itc->size();
			temp.clear();
			temp = *itc;
		}
		itc++;
	}
	vector<Point>contoursPoint1 = temp;
	cout << "contoursPoint1_size()=" << contoursPoint1.size() << endl;
}
void getCircle200(IplImage *srcBw, vector<vector<Point>> &contours){
	for (int i = 0; i < contours.size(); i++){
		for (int j = 0; j < contours[i].size(); j += contours[i].size() / pointinterval + 1){
			cvCircle(srcBw, contours[i][j], 3, CV_RGB(255, 0, 0), -1, 8, 0);
			contoursPoint2.push_back(contours[i][j]);
		}
	}
	//cvCircle(srcBw, contoursPoint2[2], 5, CV_RGB(0, 255, 0), -1, 8, 0);
	cout << "contoursPoint2_size()=" << contoursPoint2.size() << endl;
	cout << "contoursPoint2[0]_y()=" << contoursPoint2[0].y << endl;
}
void getSpecialPoint27(IplImage *srcBw, vector<Point>contoursPoint){
	//v0   top of head  v0-vcontoursPoint.size;
	//cvCircle(srcBw, contoursPoint[contoursPoint.size()-1], 6, CV_RGB(0, 0, 255), -1, 8, 0);
	Point v0 = getPointV0(contoursPoint);
	cout << "V0_y=" << v0.y << endl;
	cvCircle(srcBw, v0, 6, CV_RGB(0, 0, 255), -1, 8, 0);
	//Point p0 = getPointP0(contoursPoint, v0);
	Point p0 = getPointP0(contoursPoint, v0);
	cvCircle(srcBw, p0, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p1 = getPointP1(contoursPoint, p0);
	cvCircle(srcBw, p1, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p26 = getPointP26(contoursPoint, v0);
	cvCircle(srcBw, p26, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p25 = getPointP25(contoursPoint, p26);
	cvCircle(srcBw, p25, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point v1 = getPointV1(contoursPoint, p1);
	cvCircle(srcBw, v1, 6, CV_RGB(0, 0, 255), -1, 8, 0);
	cout << "V1_y=" << v1.y << endl;
	Point v2 = getPointV2(contoursPoint, p25);
	cvCircle(srcBw, v2, 6, CV_RGB(0, 0, 255), -1, 8, 0);
	Point p6 = getPointP6(contoursPoint, v1);
	cvCircle(srcBw, p6, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p20 = getPointP20(contoursPoint, v2);
	cvCircle(srcBw, p20, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point v3 = getPointV3(contoursPoint, p6);
	cvCircle(srcBw, v3, 5, CV_RGB(0, 0, 255), -1, 8, 0);
	Point v4 = getPointV4(contoursPoint, p20);
	cvCircle(srcBw, v4, 5, CV_RGB(0, 0, 255), -1, 8, 0);
	Point p13 = getPointP13(contoursPoint, v3);
	cvCircle(srcBw, p13, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p2 = getPointP2(contoursPoint, p1, v1);
	cvCircle(srcBw, p2, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p24 = getPointP24(contoursPoint, v2, p25);
	cvCircle(srcBw, p24, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p3 = getPointP3(contoursPoint, p1, p2, v1);
	cvCircle(srcBw, p3, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p23 = getPointP23(contoursPoint, p24, p25, v2);
	cvCircle(srcBw, p23, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p4 = getPointP4(contoursPoint, p2, p3, v1, p6);
	cvCircle(srcBw, p4, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p5 = getPointP5(contoursPoint, p1, p2, v1, p6);
	cvCircle(srcBw, p5, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p21 = getPointP21(contoursPoint, p25, p24, p20, v2);
	cvCircle(srcBw, p21, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p22 = getPointP22(contoursPoint, p24, p23, p20, v2);
	cvCircle(srcBw, p22, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p7 = getPointP7(contoursPoint, p5, p6, v3);
	cvCircle(srcBw, p7, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p19 = getPointP19(contoursPoint, p21, p20, v4);
	cvCircle(srcBw, p19, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p8 = getPointP8(contoursPoint, p6, p7, v3);
	cvCircle(srcBw, p8, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p18 = getPointP18(contoursPoint, p20, p19, v4);
	cvCircle(srcBw, p18, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p9 = getPointP9(contoursPoint, p6, p8, v3);
	cvCircle(srcBw, p9, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p17 = getPointP17(contoursPoint, p20, p18, v4);
	cvCircle(srcBw, p17, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p10 = getPointP10(contoursPoint, p8, p9, v3);
	//Point p10 = getPointP10(contoursPoint,p9,p13,v3);
	cvCircle(srcBw, p10, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p16 = getPointP16(contoursPoint, p18, p17, v4);
	cvCircle(srcBw, p16, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p11 = getPointP11(contoursPoint, p9, p10, v3, p13);
	cvCircle(srcBw, p11, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p12 = getPointP12(contoursPoint, p8, p9, v3, p13);
	cvCircle(srcBw, p12, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p14 = getPointP14(contoursPoint, p18, p17, p13, v4);
	cvCircle(srcBw, p14, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p15 = getPointP15(contoursPoint, p17, p16, p13, v4);
	cvCircle(srcBw, p15, 5, CV_RGB(0, 255, 0), -1, 8, 0);
}
int main()
{
	IplImage *plmgsrc = cvLoadImage("cccc.png");
	//IplImage *plmgsrc = cvLoadImage("bbbb.png");
	if (!plmgsrc->imageData)
	{
		cout << "Fail to load image" << endl;
		return 0;
	}
	IplImage *plmg8u = NULL;
	IplImage *plmgCanny = NULL;
	plmg8u = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);
	plmgCanny = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);

	cvCvtColor(plmgsrc, plmg8u, CV_BGR2GRAY);
	cvCanny(plmg8u, plmgCanny, 20, 200, 3);

	cvNamedWindow("resulttemp", 1);
	cvShowImage("canny", plmgCanny);
	Mat cannyImage(plmgCanny);
	Mat resulttemp;
	fillHole(cannyImage, resulttemp);
	namedWindow("resulttemp2", 1);
	imshow("afterfillhole", resulttemp);
	/*int rows = plmgsrc->height;
	int cols = plmgsrc->width;
	int cnt = 0;
	for (int i = 0;i < rows;i++) {
	uchar* ptr = (uchar*)(plmgCanny->imageData + i * plmgCanny->widthStep);
	for (int j = 0;j < cols;j++) {
	int color = (int)ptr[j];
	cout << color;
	if (color == 255)
	cnt++;
	}
	cout << endl;
	}
	cout << cnt << endl;
	system("pause");*/
	vector<vector<Point>> contours;
	///////
	//CV_CHAIN_APPROX_NONE  获取每个轮廓每个像素点  
	findContours(resulttemp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//getSizeContours(contours);
	getBodyContoursPoint(contours);
	cout << contours[contours.size() - 1].size() << endl;
	Mat result(cannyImage.size(), CV_8U, Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);   // -1 表示所有轮廓  
	namedWindow("result", 1);
	//cvSetMouseCallback("result", on_mouse, (IplImage *) result);
	imshow("result", result);/**/
	Mat src(plmgsrc);
	getCircle200(plmgsrc, contours);
	getSpecialPoint27(plmgsrc, contoursPoint2);
	drawContours(src, contours, -1, Scalar(0, 0, 255, 0), 1);   // -1 表示所有轮廓
	imshow("src", src);/**/
	waitKey(0);
	cvDestroyWindow("resulttemp");
	cvDestroyWindow("resulttemp2");
	cvDestroyWindow("result");
	cvReleaseImage(&plmgsrc);
	cvReleaseImage(&plmg8u);
	cvReleaseImage(&plmgCanny);
	//cvReleaseImage(&resultImage);
	return 0;
}