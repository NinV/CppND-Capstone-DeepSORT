#include "iou_matching.h"

using namespace std;
double area(vector<double> &tlbr){
    return (tlbr[bottom] - tlbr[top]) * (tlbr[right] - tlbr[left]);
}


double iou(vector<double> &tlbr1, vector<double> &tlbr2){
    double vertical_overlap = max(min(tlbr1[bottom], tlbr2[bottom]) - max(tlbr1[top], tlbr2[top]), 0.0);
    double horizontal_overlap = max(min(tlbr1[right], tlbr2[right]) - max(tlbr1[left], tlbr2[left]), 0.0);
    double intersection = vertical_overlap * horizontal_overlap;
    return intersection / (area(tlbr1) + area(tlbr2) - intersection);
}

vector<vector<double>> iou_cost(vector<vector<double>> &t_boxes,
                                vector<vector<double>> &det_boxes,
                                double max_iou_distance) {
    vector<vector<double>> cost_matrix;
    for(int row = 0; row < t_boxes.size(); row++){
        cost_matrix.emplace_back(vector<double >());
        for(int col = 0; col < det_boxes.size(); col++){
            double cost = 1 - iou(t_boxes[row], det_boxes[col]);
            if (cost > max_iou_distance){cost = 1.0;}
            cost_matrix[row].push_back(cost);
        }    
    }
    return cost_matrix;
}
