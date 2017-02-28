#include "pencil.h"

int main(int argc, char** argv) {
    Mat src = imread(argv[1], 1);
    if (!src.data) {
        return -1;
    }
    Mat texture = imread(argv[2]);
    pencilSketch(src, texture);
    return 0;
}
