#include "tone.h"

vector<double> getHistogram(Mat src) {
    vector<double> result(256, 0.0);
    double total = 0.0;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            result[(int)src.at<uchar>(i, j)] += 1.0;
            total += 1.0;
        }
    }
    for (double& d : result) {
        d /= total;
    }
    return result;
}
void showHistogram(vector<double> histogram) {
    int hist_height = 256;
    int scale = 5;
    int bins = histogram.size();
    double max_val = 0.0;
    for (double d : histogram) {
        if (d > max_val)
            max_val = d;
    }
    Mat hist_img = (Mat)Mat::zeros(hist_height, bins * scale, CV_8UC3);
    for(int i = 0; i < bins; ++i) {
        float bin_val = histogram[i]; //
        int intensity = cvRound(bin_val * hist_height / max_val);
        rectangle(hist_img, Point(i * scale, hist_height - 1),
                  Point((i+1) * scale - 1, hist_height - intensity),
                  Scalar(255, 255, 255));
    }
    imshow("Histogram", hist_img);
}

vector<double> getTargetHistogram() {
    vector<double> result(256, 0.0);
    double omega1 = 76.0;
    double omega2 = 22.0;
    double omega3 = 2.0;
    double sigmab = 9.0;
    double ua = 105.0;
    double ub = 225.0;
    double miud = 90.0;
    double sigmad = 11.0;
    double total = 0.0;
    for (int i = 0; i < result.size(); ++i) {
        double p;
        if (i < ua || i > ub) {
            p = 0.0;
        } else {
            p = 1.0 / (ub - ua);
        }
        result[i] = 0.01 * (omega1 * 1.0 / sigmab * exp(-(255 - i) / sigmab) +
                    omega2 * p +
                    omega3 * 1.0 / sqrt(2 * M_PI * sigmad) * exp(-pow(i - miud, 2.0) / (2 * pow(sigmad, 2.0))));
        total += result[i];
    }
    for (double& d : result) {
        d /= total;
    }
    return result;
}
Mat histogramMatching(Mat src, vector<double> targetHist) {
    Mat result(src.rows, src.cols, src.type());
    vector<double> srcHist = getHistogram(src);
    int levelNum = srcHist.size();
    vector<double> s(levelNum, 0.0);
    vector<double> g(levelNum, 0.0);
    vector<uchar> t(levelNum, 0);
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < levelNum; ++i) {
        sum1 += srcHist[i];
        sum2 += targetHist[i];
        s[i] = sum1;
        g[i] = sum2;
    }
    int pg = 0;
    for (int i = 0; i < levelNum; ++i) {
        double min = 1.0;
        for (int j = 0; j < levelNum; ++j) {
            if ((g[j] - s[i]) < min && (g[j] - s[i]) >= 0) {
                min = g[j] - s[i];
                pg = j;
            }
        }
        t[i] = (uchar)pg;
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            result.at<uchar>(i, j) = t[src.at<uchar>(i, j)];
        }
    }
    return result;
}
Mat getToneImage(Mat src) {
    vector<double> targetHist = getTargetHistogram();
    Mat result = histogramMatching(src, targetHist);
    blur(result, result, Size(10, 10));
//    showHistogram(getHistogram(result));
    return result;
}
