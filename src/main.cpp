#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "detection.h"
#include "tracker.h"


using namespace std;


void drawDetection(Detection &det, cv::Mat &frame){
    auto tlbr = det.to_tlbr();
    std::cout << "[Detection] ";
    std::cout << "top: " << tlbr[Top] << " left: " << tlbr[Left] << " bottom: " << tlbr[Bottom] << " right: " << tlbr[Right] << "\n";
    cv::rectangle(frame, det.box, cv::Scalar(255, 178, 50), 3);
}

void drawTrack(Track &t, cv::Mat &frame){
    auto tlbr = t.to_tlbr();
    std::cout << "[TrackID = " << t.trackID() <<"] ";
    std::cout << "top: " << tlbr[Top] << " left: " << tlbr[Left] << " bottom: " << tlbr[Bottom] << " right: " << tlbr[Right] << "\n";
//    std::cout << t.track_value().state_vector << "\n";
    cv::rectangle(frame, t.box(), cv::Scalar(20, 100, 50), 3);
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Not enough parameters" << std::endl;
        return -1;
    }
    Detector detector = Detector();
    Tracker tracker = Tracker();
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
        std::vector<Detection> detections = detector.detect(frame);
        cout << "Frame: " << frameNum << ". Found: " << detections.size() << endl;
        tracker.predict();
        tracker.update(detections);
        for(Detection &det: detections){
            drawDetection(det, frame);
        }
        for(auto &t: tracker.tracks()){
            drawTrack(*t, frame);
        }
        imshow("tracking", frame);
        char c = (char)cv::waitKey(1);
        if (c == 27) break;
    }
    return 0;
}
