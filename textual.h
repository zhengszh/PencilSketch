//
// Created by einzelkampfer on 17-2-24.
//

#ifndef PENCILSKETCH_TEXTUAL_H
#define PENCILSKETCH_TEXTUAL_H
#include "base.h"
#include <fstream>
using namespace cv;
using namespace std;
using namespace Eigen;

Mat customReshape(Mat mat);
Mat getTextureImage(Mat src, Mat texture, Mat tone);
#endif //PENCILSKETCH_TEXTUAL_H
