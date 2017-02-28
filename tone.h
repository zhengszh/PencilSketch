#ifndef PENCILSKETCH_TONE_H
#define PENCILSKETCH_TONE_H
#include "base.h"

using namespace std;
using namespace cv;

vector<double> getHistogram(Mat src);
void showHistogram(vector<double> histogram);
vector<double> getTargetHistogram();
Mat getToneImage(Mat src);

#endif //PENCILSKETCH_TONE_H
