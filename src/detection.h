#include <vector>
#include "string"
#include "opencv2/core.hpp"
#include "opencv2/dnn.hpp"

#ifndef CPPND_CAPSTONE_DEEPSORT_DETECTION_H
#define CPPND_CAPSTONE_DEEPSORT_DETECTION_H
class Detection{
private:
    cv::Rect box_;
    int classIdx_;
    float confidence_;

public:
    //This class represents a bounding box detection in a single image.
    Detection(cv::Rect box, float confidence, int classIdx);

    // Convert bounding box to format `(min x, min y, max x, max y)`
    std::vector<float> to_tlbr();

    // Convert bounding box to format `(center x, center y, aspect ratio,height)`,
    // where the aspect ratio is `width / height`
    std::vector<float> to_xyah();
};


class Detector{
private:
    // Initialize the parameters
    float confThreshold_ = 0.5;                                     // Confidence threshold
    float nmsThreshold_ = 0.4;                                      // Non-maximum suppression threshold
    int inpWidth_ = 416;                                            // Width of network's input image
    int inpHeight_ = 416;                                           // Height of network's input image
    std::vector<std::string> outLayerNames_;                        // names of network's out layers
    std::vector<std::string> classNames_;                           // network's class names

    void loadClassNames_();                                         // Load names of classes
    void getOutLayersNames();                                       // Load out layer names
    std::vector<Detection> postprocess_(const std::vector<cv::Mat> &outs, const cv::Mat &frame);

    cv::dnn::Net net_;
    std::vector<std::string> _yolo_layers;
public:
    Detector();
    std::vector<Detection> detect(const cv::Mat &frame);

};


#endif //CPPND_CAPSTONE_DEEPSORT_DETECTION_H
