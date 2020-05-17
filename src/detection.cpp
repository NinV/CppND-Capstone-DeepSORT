#include "detection.h"
#include <fstream>
#include <iostream>

Detection::Detection(cv::Rect2d &_box, float _confidence, int _classIdx):
        confidence(_confidence),
        classIdx(_classIdx),
        x(_box.x),
        y(_box.y),
        w(_box.width),
        h(_box.height),
        box(_box){}

Detector::Detector(){
    std::string modelConfiguration = "../data/yolov3-tiny.cfg";
    std::string modelWeights = "../data/yolov3-tiny.weights";

    // Load the network
    net_ = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // load class name
    loadClassNames_();

    // get out layer names
    getOutLayersNames();

    std::cout << "Construct detector\n";

}

void Detector::loadClassNames_() {
    std::string classesFile = "../data/coco.names";
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) classNames_.push_back(line);
    ifs.close();
}

void Detector::getOutLayersNames(){
    // Get the indices of the output layers, i.e. the layers with unconnected outputs
    std::vector<int> outLayersIdx = net_.getUnconnectedOutLayers();

    // get the names of all the layers in the network
    std::vector<std::string> layersNames = net_.getLayerNames();

    // Get the names of the output layers in names
    outLayerNames_.resize(outLayersIdx.size());
    for (size_t i = 0; i < outLayersIdx.size(); ++i)
        outLayerNames_[i] = layersNames[outLayersIdx[i] - 1];
}

std::vector<Detection> Detector::detect(const cv::Mat &frame) {
    // Create a 4D blob from a frame.
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth_, inpHeight_),
                     cv::Scalar(0,0,0), true, false);

    // Sets the input to the network
    net_.setInput(blob);

    // Runs the forward pass to get output of the output layers
    std::vector<cv::Mat> outs;
    net_.forward(outs, outLayerNames_);

    // Remove the bounding boxes with low confidence
    return postprocess_(outs, frame);
}

std::vector<Detection> Detector::postprocess_(const std::vector<cv::Mat> &outs, const cv::Mat &frame){
    {
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect2d> boxes;
        std::vector<Detection> detections;

        for (const auto & out : outs)
        {
            // Scan through all the bounding boxes output from the network and keep only the
            // ones with high confidence scores. Assign the box's class label as the class
            // with the highest score for the box.
            auto* data = (float*)out.data;
            for (int j = 0; j < out.rows; ++j, data += out.cols)
            {
                cv::Mat scores = out.row(j).colRange(5, out.cols);
                cv::Point classIdPoint;
                double confidence;
                // Get the value and location of the maximum score
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold_)
                {
                    auto cx = (double)(data[0] * frame.cols);
                    auto cy = (double)(data[1] * frame.rows);
                    auto w = (double)(data[2] * frame.cols);
                    auto h = (double)(data[3] * frame.rows);
                    auto left = cx - w / 2;
                    auto top = cy - h / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.emplace_back(cv::Rect2d(left, top, w, h));
                }
            }
        }

        // Perform non maximum suppression to eliminate redundant overlapping boxes with
        // lower confidences
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confThreshold_, nmsThreshold_, indices);
        for (int idx : indices)
        {
            detections.emplace_back(boxes[idx], confidences[idx], classIds[idx]);
        }
        return detections;
    }
}