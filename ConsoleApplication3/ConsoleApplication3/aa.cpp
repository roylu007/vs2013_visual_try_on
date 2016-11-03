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
Point getPointV1(vector<Point>contoursPoint){//V1  --   right_fingertip   point.x  ->min
	Point temp;
	int minx = 1024;
	for (int i = 0; i < contoursPoint.size(); i++){
		if (minx > contoursPoint[i].x){
			minx = contoursPoint[i].x;
			temp = contoursPoint[i];
		}
	}
	return temp;
}
Point getPointV2(vector<Point>contoursPoint){//V2  --   left_fingertip   point.x  ->max
	Point temp;
	int maxx = 0;
	for (int i = 0; i < contoursPoint.size(); i++){
		if (maxx < contoursPoint[i].x){
			maxx = contoursPoint[i].x;
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
	for (int j = i - 1 >= 0 ? i - 1 : lens - 1 + i; cnt <4* M; j--){
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
	Point v1 = getPointV1(contoursPoint);
	cvCircle(srcBw, v1, 6, CV_RGB(0, 0, 255), -1, 8, 0);
	cout << "V1_y=" << v1.y << endl;
	Point v2 = getPointV2(contoursPoint);
	cvCircle(srcBw, v2, 6, CV_RGB(0, 0, 255), -1, 8, 0);
	//Point p0 = getPointP0(contoursPoint, v0);
	Point p0 = getPointP0(contoursPoint, v0);
	cvCircle(srcBw, p0, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p1 = getPointP1(contoursPoint, p0);
	cvCircle(srcBw, p1, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p26 = getPointP26(contoursPoint, v0);
	cvCircle(srcBw, p26, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	Point p25 = getPointP25(contoursPoint, p26);
	cvCircle(srcBw, p25, 5, CV_RGB(0, 255, 0), -1, 8, 0);
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
}
int main()
{
	IplImage *plmgsrc = cvLoadImage("cccc.png");
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