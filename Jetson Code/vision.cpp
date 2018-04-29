#include <opencv2/opencv.hpp>
#include <math.h>
#include <ctime>
#include <networktables/NetworkTable.h>
#include <networktables2/type/NumberArray.h>

#define PI 3.14159265
NetworkTable *visionTable;
const cv::Scalar RED = cv::Scalar(0, 0, 255),
        BLUE = cv::Scalar(255, 0, 0),
        GREEN = cv::Scalar(0, 255, 0),
        BLACK = cv::Scalar(0, 0, 0),
        YELLOW = cv::Scalar(0, 255, 255),
        LOWER_BOUNDS = cv::Scalar(59, 134, 45),
        UPPER_BOUNDS = cv::Scalar(89, 255, 255);
cv::VideoCapture videoCapture;
cv::Mat matOriginal, matHSV, matThresh, clusters, matHeirarchy;
const int TOP_TARGET_HEIGHT = 97,
        TOP_CAMERA_HEIGHT = 13.3;
const double VERTICAL_FOV = 39.6;
const double HORIZONTAL_FOV = 52.8;
const double CAMERA_ANGLE = 30;
bool shouldRun = true;
int biggestArea = 0;
int biggestAreaIndex = 0;
bool buttonPressed = false;
std::vector<std::vector<cv::Point> > contours;
std::vector<std::vector<cv::Point> > selected;

double normalize360(double angle) {
    while (angle >= 360.0) {
        angle -= 360.0;
    }
    while (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

void processImage() {
    double x, y, targetX, targetY, distance, azimuth;
    int FrameCount = 0;
    while (true) {
        contours.clear();
        selected.clear();
        videoCapture.read(matOriginal);
        try {
            visionTable->PutNumber("width", matOriginal.cols);
        }
        catch (std::exception &e) {}
        cv::cvtColor(matOriginal, matHSV, cv::COLOR_BGR2HSV);
        cv::inRange(matHSV, LOWER_BOUNDS, UPPER_BOUNDS, matThresh);
        //imshow("yes", matThresh);
        cv::findContours(matThresh, contours, matHeirarchy, cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE);
        biggestArea = 0;
        cv::Rect biggestRect;
        for (int i = 0; i < contours.size(); i++) {
            cv::approxPolyDP(cv::Mat(contours[i]), contours[i], 3, true);
            cv::Rect rec = cv::boundingRect(contours[i]);
            float aspect = (float) rec.width / (float) rec.height;
            cv::rectangle(matOriginal, rec, YELLOW);
            if (aspect > 1.0) {
                if (rec.area() > biggestArea && rec.area() < 80000) {
//                                      selected.clear();
                    std::ostringstream a;
                    a << rec.area();
                    biggestArea = rec.area();
                    biggestRect = rec;
                    //cv::putText(matOriginal, a.str(), rec.tl(), cv::FONT_HERSHEY_PLAIN, 1, GREEN);
//                                      selected.push_back(contours[i]);
                }
            }
        }
//              std::cout << "Size of selected: " << selected.size() << std::endl;
//              for(unsigned int i = 0; i < selected.size(); i++){
//                      cv::Rect rec = cv::boundingRect(selected[i]);
//                      std::cout << "looping on selected!" << std::endl;
        cv::rectangle(matOriginal, biggestRect, RED);
//              }
//                     if there is only 1 target, then we have found the target we want
        if (biggestArea > 50) {
            try {
                visionTable->PutNumber("detected", 1);
            }
            catch (std::exception &e) {}
        } else {
            try {
                visionTable->PutNumber("detected", 0);
            }
            catch (std::exception &e) {}
        }
        if (biggestArea > 0) {
//                      std::cout << "selected is one!" << std::endl;
            cv::Rect rec = biggestRect;
            //                             "fun" math brought to you by miss daisy (team 341)!
            y = rec.br().y + rec.height / 2;
            y = -((2 * (y / matOriginal.cols)) - 1);
            distance = (TOP_TARGET_HEIGHT - TOP_CAMERA_HEIGHT) /
                       tan((y * VERTICAL_FOV / 2.0 + CAMERA_ANGLE) * PI / 180);
            //                             angle to target...would not rely on this
            targetX = rec.tl().x + rec.width / 2;
            targetX = (2 * (targetX / matOriginal.rows)) - 1;
            azimuth = normalize360(targetX * HORIZONTAL_FOV / 2.0 + 25);
            std::ostringstream s;
            s << distance;
            std::ostringstream a;
            a << azimuth;
            //                             drawing info on target
            cv::Point center = cv::Point(rec.br().x - rec.width / 2 - 15, rec.br().y - rec.height / 2);
            cv::Point centerw = cv::Point(rec.br().x - rec.width / 2 - 15, rec.br().y - rec.height / 2 - 20);
            cv::putText(matOriginal, "Distance: " + s.str(), center, cv::FONT_HERSHEY_PLAIN, 1, GREEN);
            cv::putText(matOriginal, "xAngle: " + a.str(), centerw, cv::FONT_HERSHEY_PLAIN, 1, BLUE);
            try {
                visionTable->PutNumber("xAngle", azimuth);
                visionTable->PutNumber("xValue", rec.br().x - rec.width / 2);
                visionTable->PutNumber("distance", distance);
            }
            catch (std::exception &e) {}
            //cv::putText(matOriginal, a.str(), centerw, cv::FONT_HERSHEY_PLAIN, 1, GREEN);
        }
        //                     output an image for debugging
        //cv::imshow("output.jpg", matOriginal);
        cv::line(matOriginal, cv::Point((640 / 3) + 20, 0), cv::Point((640 / 3) + 20, 480), BLUE, 4, 8, 0);
        cv::line(matOriginal, cv::Point((640 / 3 * 2) - 20, 0), cv::Point((640 / 3 * 2) - 20, 480), BLUE, 4, 8, 0);
        cv::imwrite("output/output.jpg", matOriginal);
        cv::waitKey(30);
        FrameCount++;
    }
    //shouldRun = false;
}

/**
*
* @param args command line arguments
* just the main loop for the program and the entry points
*/
int main() {
    // TODO Auto-generated method stub
    NetworkTable::SetClientMode();
    NetworkTable::SetIPAddress("10.1.14.2");
    visionTable = NetworkTable::GetTable("visionTable");
    matOriginal = cv::Mat();
    matHSV = cv::Mat();
    matThresh = cv::Mat();
    clusters = cv::Mat();
    matHeirarchy = cv::Mat();
//              main loop of the program
    while (shouldRun) {
        try {
//                              opens up the camera stream and tries to load it
            videoCapture = cv::VideoCapture();
            videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
            videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
//                              replaces the ##.## with your team number
            videoCapture.open(0);
//                              Example
//                              cap.open("http://10.30.19.11/mjpg/video.mjpg");
//                              wait until it is opened
            while (!videoCapture.isOpened()) {
                printf("videoCapture could not open!\n");
            }
//                              time to actually process the acquired images
            processImage();
        } catch (std::exception &e) {
            std::cout << "MyException caught" << std::endl;
            std::cout << e.what() << std::endl;
            break;
        }
    }

    videoCapture.release();
    exit(0);
}
