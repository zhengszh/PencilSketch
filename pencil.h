//
// Created by einzelkampfer on 17-2-28.
//

#ifndef PENCILSKETCH_PENCIL_H
#define PENCILSKETCH_PENCIL_H
#include "stroke.h"
#include "tone.h"
#include "textual.h"

std::string getTypeOfMat(Mat mat) {
    std::string r;
    switch (mat.type()) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }
    return r;
}

Mat pencilSketch(Mat src, Mat texture, bool isColor = false, int strokeWidth = 3) {
    Mat src_gray;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    Mat strokeImage = getStrokeImage(src_gray, strokeWidth);
    Mat toneImage = getToneImage(src_gray);
    cvtColor(texture, texture, CV_BGR2GRAY);
    texture = getTextureImage(src_gray, texture, toneImage);
    vector<Mat> channels;
    if (isColor) {
        cvtColor(src, src, CV_BGR2YCrCb);
        split(src, channels);
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double pixel1 = (double)(int)strokeImage.at<ushort>(i, j) / 65535.0;
            double pixel2 = (double)(int)texture.at<uchar>(i, j);
            if (isColor)
                channels[0].at<uchar>(i, j) = (uchar)(pixel1 * pixel2);
            else
                src_gray.at<uchar>(i, j) = (uchar)(pixel1 * pixel2);
        }
    }
    if (isColor) {
        merge(channels, src);
        cvtColor(src, src, CV_YCrCb2BGR);
    }
    return isColor ? src : src_gray;
}
#endif //PENCILSKETCH_PENCIL_H
