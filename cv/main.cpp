#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "stdio.h"
#include <iostream>
#include <stack>
using namespace std;
#include "opencv2/cuda.hpp"
#include "opencv2/cudafilters.hpp"
#include <opencv2/cudaarithm.hpp>

using namespace cv;
int hl,sl,vl,vh,sh,hh;
using namespace cv::cuda;
void Traverse(int xs, int ys, cv::cuda::GpuMat &ids,cv::cuda::GpuMat &image, int blobID, cv::Point &leftTop, cv::Point &rightBottom) {
    std::stack<cv::Point> S;
    S.push(cv::Point(xs,ys));
    while (!S.empty()) {
        cv::Point u = S.top();
        S.pop();

        int x = u.x;
        int y = u.y;

        if (image.at<unsigned char>(y,x) == 0 || ids.at<unsigned char>(y,x) > 0)
            continue;

        ids.at<unsigned char>(y,x) = blobID;
        if (x < leftTop.x)
            leftTop.x = x;
        if (x > rightBottom.x)
            rightBottom.x = x;
        if (y < leftTop.y)
            leftTop.y = y;
        if (y > rightBottom.y)
            rightBottom.y = y;

        if (x > 0)
            S.push(cv::Point(x-1,y));
        if (x < ids.cols-1)
            S.push(cv::Point(x+1,y));
        if (y > 0)
            S.push(cv::Point(x,y-1));
        if (y < ids.rows-1)
            S.push(cv::Point(x,y+1));
    }


}
int FindBlobs(cv::cuda::GpuMat &image, std::vector<cv::Rect> &out, float minArea) {
    cv::cuda::GpuMat ids = cv::Mat::zeros(image.rows, image.cols,CV_8UC1);
    cv::cuda::GpuMat thresholded;
//    cv::cvtColor(image, thresholded, CV_RGB2GRAY);
    image.copyTo(thresholded);
    const int thresholdLevel = 130;
    cv::cuda::threshold(thresholded, thresholded, thresholdLevel, 255, THRESH_BINARY);
    int blobId = 1;
    for (int x = 0;x<ids.cols;x++)
        for (int y=0;y<ids.rows;y++){
            if (thresholded.at<unsigned char>(y,x) > 0 && ids.at<unsigned char>(y,x) == 0) {
                cv::Point leftTop(ids.cols-1, ids.rows-1), rightBottom(0,0);
                Traverse(x,y,ids, thresholded,blobId++, leftTop, rightBottom);
                cv::Rect r(leftTop, rightBottom);
                if (r.area() > minArea)
                    out.push_back(r);
            }
        }
    return blobId;
}
int main(int, char**)
{
	int highestarea;
	NetworkTable* visionTable;
	NetworkTable::setClientMode();
	int x,y;
	NetworkTable::setIPAdress("10.1.14.2");
	visionTable = new NetworkTable::GetTable("visionTable");
	visionTable->PutNumber("Tolerance",10);
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	visionTable->PutNumber("width",cap.get(CAP_PROP_FRAME_WIDTH));
	visionTable->PutNumber("height",cap.get(CAP_PROP_FRAME_HEIGHT));
	visionTable->PutNumber("yTicks",0.1);
	visionTable->PutNumber("xTicks",0.1);

    Mat edges;
    for(;;)
    {
        std::vector<Rect> keypoints;
        Mat im_with_keypoints;
        Mat frame;
        cap >> frame; // get a new frame from camera
//        resize(frame,frame,Size(frame.cols/2, frame.rows/2)); //double the size (not sure if needed anymore)
        frame.copyTo(edges); //copy it to edges for post processings
        frame.copyTo(im_with_keypoints); // copy to im_with_keypoints for the final drawn image
        //gpu::inRange(edges, cv::Scalar(222, 255, 0), cv::Scalar(255, 255, 255), edges); //Find green reflection only (pretty good rgb)
        GpuMat g_frame(frame);
        std::vector <cv::cuda::GpuMat> g_frame_split;
        cv::cuda::GpuMat g_frame_splitGE;
        cv::cuda::GpuMat g_frame_splitLE;
        cv::cuda::split(g_frame, g_frame_split);

        int max[3] = {255, 255, 255};
        int min[3] = {222, 255, 0};
        for (size_t i = 0; i < g_frame_split.size(); i++)
        {
                cv::cuda::compare(g_frame_split[i], min[i], g_frame_splitGE, cv::CMP_GE);
                cv::cuda::compare(g_frame_split[i], max[i], g_frame_splitLE, cv::CMP_LE);
                cv::cuda::bitwise_and(g_frame_splitGE, g_frame_splitLE, g_frame_split[i]);
        }
        cv::cuda::bitwise_and(g_frame_split[0], g_frame_split[1], g_frame);
        cv::cuda::bitwise_and(g_frame_split[2], g_frame, g_frame);
        g_frame.download(edges);
        highestarea=10;

        erode(edges, edges, 0, Point(-1, -1), 2, 1, 1); //Make the edges cleaner
        //FindBlobs(edges,keypoints,1); //Use our FindBlobs function, which looks for white blobs only
        if(keypoints.size()>0){
        	visionTable->PutBoolean("detectedObject",true);
        }
        else{
        	visionTable->PutBoolean("detectedObject",false);
        }
        for(int i=1;i<keypoints.size();i++){
                rectangle(im_with_keypoints,keypoints[i],Scalar(0,0,255)); //For each of our keypoints (detected objects), draw the square around it.
                if(keypoints[i].area()>highestarea){
                   highestarea=keypoints[i].area();
                   x=keypoints[i].x+(keypoints[i].width/2);
                   y=keypoints[i].y+(keypoints[i].height/2);
                }
        }
        visionTable->PutNumber("X",x);
        visionTable->PutNumber("Y",y);
        //imshow("keypoints", im_with_keypoints ); //show us the image with squares
        imshow("edges", edges); //show us just the detected objects
        if(waitKey(30) >= 0) break; //press a key to quit

    }
    return 0;
}
