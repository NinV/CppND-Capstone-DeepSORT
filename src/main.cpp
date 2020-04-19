#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "detection.h"


using namespace std;


void drawDetection(Detection &det, cv::Mat &frame){
    cv::rectangle(frame, det.getTl(), det.getBr(), cv::Scalar(255, 178, 50), 3);
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Not enough parameters" << std::endl;
        return -1;
    }
    Detector detector = Detector();
    const string videoInput = argv[1];

    int frameNum = -1;          // Frame counter
    cv::VideoCapture capture(videoInput);
    if (!capture.isOpened())
    {
        cout  << "Could not open reference " << videoInput << endl;
        return -1;
    }

    // Windows
    namedWindow("tracking", cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    while (true) //Show the image captured in the window and repeat
    {
        capture >> frame;
        if (frame.empty())
        {
            cout << " < < <  Videos input ended!  > > > ";
            break;
        }
        ++frameNum;
        cout << "Frame: " << frameNum << "# ";
        cout << endl;
        std::vector<Detection> detections = detector.detect(frame);
        for(Detection det: detections){
            drawDetection(det, frame);
        }
        imshow("tracking", frame);
        char c = (char)cv::waitKey(1);
        if (c == 27) break;
    }
    return 0;
}
