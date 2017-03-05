#include "pencil.h"

int main(int argc, char** argv) {
    Mat src = imread(argv[1], 1);
    if (!src.data) {
        return -1;
    }
    Mat texture = imread(argv[2]);
    Mat result = pencilSketch(src, texture);
    namedWindow("", CV_WINDOW_NORMAL);
    imshow("", result);
    waitKey(0);
    imwrite("/home/einzelkampfer/test.jpg", result);
    return 0;
}
