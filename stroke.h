//
// Created by einzelkampfer on 17-2-23.
//

#ifndef PENCILSKETCH_STROKE_H
#define PENCILSKETCH_STROKE_H
#include "base.h"
#define DIRECT_NUM 8

using namespace std;
using namespace cv;

Mat getGradientImage(Mat src);
vector<Mat*> getKernals(int directNum, int lineWidth = 1);
vector<Mat*> getResponseMaps(Mat src, vector<Mat*> kernals);
vector<Mat> getClassificationMap(vector<Mat*> responseMaps, Mat grad);
Mat getStrokeImage(Mat src, int strokeWidth);

#endif //PENCILSKETCH_STROKE_H
