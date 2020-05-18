#include <vector>

#ifndef CPPND_CAPSTONE_DEEPSORT_IOU_MATCHING_H
#define CPPND_CAPSTONE_DEEPSORT_IOU_MATCHING_H


enum TLBR {top, left, bottom, right};

using namespace std;
double area(vector<double> &tlbr);
double iou(vector<double> &tlbr1, vector<double> &tlbr2);
vector<vector<double>> iou_cost(vector<vector<double>> &t_boxes,
                                vector<vector<double>> &det_boxes,
                                double max_iou_distance);



#endif //CPPND_CAPSTONE_DEEPSORT_IOU_MATCHING_H
