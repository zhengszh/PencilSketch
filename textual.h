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
Mat customReshape(Mat mat) {
    Mat result(mat.rows * mat.cols, 1, mat.type());
    for (int i = 0; i < mat.cols; ++i) {
        for (int j = 0; j < mat.rows; ++j) {
            result.at<uchar>(i * mat.rows + j, 0) = mat.at<uchar>(j, i);
        }
    }
    return result;
}
Mat getTextureImage(Mat src, Mat texture, Mat tone) {
    int col = src.cols;
    int row = src.rows;
    float theta = 0.2;
    resize(texture, texture, Size(src.cols, src.rows));
    Mat textureTemp = customReshape(texture);
    SparseMatrix<float> logP(col * row, col * row);
    for (int i = 0; i < textureTemp.rows; ++i) {
        logP.insert(i, i) = log((float)(int)textureTemp.at<uchar>(i, 0) / 255.0);
    }
    tone = customReshape(tone);
    SparseMatrix<float> logJ(row * col, 1);
    for (int i = 0; i < tone.rows; ++i) {
        float pixel = (float)(int)tone.at<uchar>(i, 0);
        pixel = log(pixel / 255.0);
        logJ.insert(i, 0) = pixel;
    }
    SparseMatrix<float> dX(col * row, col * row), dY(col * row, col * row);
    for (int i = 0; i < col * row; ++i) {
        dX.insert(i, i) = -1.0;
        dY.insert(i, i) = -1.0;
        if (i + 1 < col * row)
            dY.insert(i, i + 1) = 1.0;
        if (i + row < col * row)
            dX.insert(i, i + row) = 1.0;
    }
    dX = dX * dX.adjoint();
    dY = dY * dY.adjoint();
    SparseMatrix<float> A = dX + dY;
    A *= theta;
    A += logP.adjoint() * logP;
    logP = logP.adjoint() * logJ;

    ConjugateGradient<SparseMatrix<float>, Lower|Upper> cg;
    cg.setMaxIterations(60);
    cg.compute(A);
    SparseMatrix<float> beta;
    beta = cg.solve(logP);

    double min = 99999.0;
    double max = -99999.0;
    for (int i = 0; i < texture.rows; ++i) {
         for (int j = 0; j < texture.cols; ++j) {
             float pixel = (float)(int)texture.at<uchar>(i, j) / 255.0;
             pixel = pow(pixel, beta.coeff(j * texture.rows + i, 0));
             pixel *= 255.0;
             if (pixel > max)
                 max = pixel;
             if (pixel < min)
                 min = pixel;
         }
    }
    cout << max << " " << min << "\n";
    double min2 = 99999.0;
    double max2 = -99999.0;
    for (int i = 0; i < texture.rows; ++i) {
        for (int j = 0; j < texture.cols; ++j) {
            float pixel = (float)(int)texture.at<uchar>(i, j)  / 255.0;
            pixel = pow(pixel, beta.coeff(j * texture.rows + i, 0));
            pixel *= 255.0;
            pixel = (pixel - min) / (max - min) * 255.0;
            if (pixel > max2)
                max2 = pixel;
            if (pixel < min2)
                min2 = pixel;
            texture.at<uchar>(i, j) = (uchar)(int)(pixel);
        }
    }
    Mat temp;
    resize(texture, temp, Size(texture.cols / 2, texture.rows / 2));
    imshow("", temp);
    waitKey(0);
    return texture;
}

#endif //PENCILSKETCH_TEXTUAL_H
