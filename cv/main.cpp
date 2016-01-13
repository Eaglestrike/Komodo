#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "stdio.h"
#include <iostream>
#include <stack>

using namespace cv;
int highestarea,highestindex,counter,x,y;

void Traverse(int xs, int ys, cv::Mat &ids,cv::Mat &image, int blobID, cv::Point &leftTop, cv::Point &rightBottom) {
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
int FindBlobs(cv::Mat &image, std::vector<cv::Rect> &out, float minArea) {
    cv::Mat ids = cv::Mat::zeros(image.rows, image.cols,CV_8UC1);
    cv::Mat thresholded;
//    cv::cvtColor(image, thresholded, CV_RGB2GRAY);
    image.copyTo(thresholded);
    const int thresholdLevel = 130;
    cv::threshold(thresholded, thresholded, thresholdLevel, 255, CV_THRESH_BINARY);
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
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    cap.set(CV_CAP_PROP_BUFFERSIZE, 300);
    Mat edges;
    for(;;)
    {
    	double t = (double)cv::getTickCount();
        std::vector<Rect> keypoints;
    	Mat im_with_keypoints;
        Mat frame;
        cap >> frame; // get a new frame from camera
        resize(frame,frame,frame.size()*1); //double the size (not sure if needed anymore)
        frame.copyTo(edges); //copy it to edges for post processings
        frame.copyTo(im_with_keypoints); // copy to im_with_keypoints for the final drawn image
        inRange(edges, cv::Scalar(222, 255, 0), cv::Scalar(255, 255, 255), edges); //Find green reflection only (pretty good rgb)
        erode(edges, edges, 0, Point(-1, -1), 2, 1, 1); //Make the edges cleaner
        FindBlobs(edges,keypoints,0); //Use our FindBlobs function, which looks for white blobs only
        highestarea=10000;
        x=-1;
        y=-1;
        for(int i=1;i<keypoints.size();i++){
        	rectangle(im_with_keypoints,keypoints[i],Scalar(0,0,255)); //For each of our keypoints (detected objects), draw the square around it.
        	if(keypoints[i].area()>highestarea){
        		highestarea=keypoints[i].area();
        		x=keypoints[i].x+(keypoints[i].width)/2);
        		y=keypoints[i].y+(keypoints[i].height/2);
        	}

        }
        if(counter%2==0){
        	std::cout<<"Highest Area is "<<highestarea<<" with an x y of "<<x<<" "<<y<<std::endl;
        	std::cout<<cap.get(CV_CAP_PROP_FRAME_COUNT)<<std::endl;;
        }

        counter++;
        if(counter==2){
        	imshow("keypoints", im_with_keypoints ); //show us the image with squares
        	imshow("edges", edges); //show us just the detected objects
        	counter=0;
        }
        if(waitKey(30) >= 0) break; //press a key to quit

    }
    return 0;
}
//int socketConnection(){
//	try
//	  {
//		// Create the client socket
//		ClientSocket client_socket ( "localhost", 30000 );
//		try{
//			client_socket << ""+
//		}
//		catch ( SocketException& ) {
//
//		}
//
//		// rest of code -
//		// send request, retrieve reply, etc...
//
//	  }
//	catch ( SocketException& e )
//	  {
//		std::cout << "Exception was caught:" << e.description() << "\n";
//	  }
//	return 0;
//}
