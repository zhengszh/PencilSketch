#include "stroke.h"

Mat getGradientImage(Mat src) {
    Mat grad_x, grad_y, grad;
    int row = src.rows;
    int col = src.cols;
    Rect leftPart(0, 0, col - 1, row);
    Rect rightPart(1, 0, col - 1, row);
    absdiff(src(leftPart), src(rightPart), grad_x);
    copyMakeBorder(grad_x, grad_x, 0, 0, 0, 1, BORDER_CONSTANT, 0);

    Rect upPart(0, 0, col, row - 1);
    Rect downPart(0, 1, col, row - 1);
    absdiff(src(upPart), src(downPart), grad_y);
    copyMakeBorder(grad_y, grad_y, 0, 1, 0, 0, BORDER_CONSTANT, 0);

    add(grad_x, grad_y, grad);
    
    return grad;
}

vector<Mat*> getKernals(int directNum, int lineWidth) {
    int kernalSize = 17;
    Scalar white(255, 255, 255);
    double angleDiff = M_PI / directNum;
    double radius = (double)kernalSize / 2 * sqrt(2.0);
    double angle = 0;
    vector<Mat*> result;
    for (int i = 0; i < directNum; ++i) {
        double x1 = sin(angle) * radius + (double)kernalSize / 2.0;
        double y1 = cos(angle) * radius + (double)kernalSize / 2.0;
        double x2 = -sin(angle) * radius + (double)kernalSize / 2.0;
        double y2 = -cos(angle) * radius + (double)kernalSize / 2.0;
        Mat* kernal = new Mat(kernalSize, kernalSize, CV_16U, Scalar(0, 0, 0));
        line(*kernal, Point2d(x1, y1), Point2d(x2, y2), white, lineWidth, CV_AA);
        angle += angleDiff;
        result.push_back(kernal);
    }
    return result;
}
vector<Mat*> getResponseMaps(Mat src, vector<Mat*> kernals) {
    vector<Mat*> responseMaps;
    for (Mat* k : kernals) {
        Mat* filtered = new Mat;
        filter2D(src, *filtered, CV_64F, *k);
        responseMaps.push_back(filtered);
    }
    return responseMaps;
}
vector<Mat> getClassificationMap(vector<Mat*> responseMaps, Mat grad) {
    vector<Mat> result;
    int row = responseMaps[0] -> rows;
    int col = responseMaps[0] -> cols;
    for (int i = 0; i < responseMaps.size(); ++i) {
        Mat classification = Mat(row, col, CV_8U, Scalar(0));
        result.push_back(classification);
    }
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            int maxDirection = 0;
            for (int k = 1; k < responseMaps.size(); ++k) {
                if (responseMaps[k] -> at<double>(i, j) > responseMaps[maxDirection] -> at<double>(i, j)) {
                    maxDirection = k;
                }
            }
            result[maxDirection].at<uchar>(i, j) = grad.at<uchar>(i, j);
        }
    }
    return result;
}
Mat getStrokeImage(Mat src) {
    medianBlur(src, src, 3);
    Mat grad = getGradientImage(src);
    vector<Mat*> kernals = getKernals(DIRECT_NUM);
    vector<Mat*> responseMaps = getResponseMaps(grad, kernals);
    kernals = getKernals(DIRECT_NUM, 3);
    vector<Mat> classificationMaps = getClassificationMap(responseMaps, grad);
    Mat strokeImage(src.rows, src.cols, CV_16U);
    vector<Mat> filteredMaps;
    for (int i = 0; i < classificationMaps.size(); ++i) {
        Mat filtered;
        filter2D(classificationMaps[i], filtered, CV_64F, *kernals[i]);
        filteredMaps.push_back(filtered);
    }
    double max = -9999999.0;
    double min = 9999999.0;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double pixel = 0.0;
            for (int k = 0; k < filteredMaps.size(); ++k) {
                pixel += filteredMaps[k].at<double>(i, j);
            }
            if (pixel > max)
                max = pixel;
            if (pixel < min)
                min = pixel;
        }
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double pixel = 0.0;
            for (int k = 0; k < filteredMaps.size(); ++k) {
                pixel += filteredMaps[k].at<double>(i, j);
            }
            pixel = (1.0 - (pixel - min) / (max - min)) * 65535.0;
            strokeImage.at<ushort>(i, j) = (ushort)pixel;
        }
    }
    return strokeImage;
}