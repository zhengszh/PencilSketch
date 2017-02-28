#include "stroke.h"
#include "tone.h"
#include "textual.h"

int main(int argc, char** argv) {
    Mat src, src_gray;
    /// Read the image
    src = imread(argv[1], 1);

    if (!src.data) {
        return -1;
    }
    /// Convert it to gray
    cvtColor(src, src_gray, CV_BGR2GRAY);
    Mat strokeImage = getStrokeImage(src_gray);

    Mat toneImage = getToneImage(src_gray);
    Mat texture = imread("/home/einzelkampfer/PencilDrawing/pencils/pencil0.jpg");
    cvtColor(texture, texture, CV_BGR2GRAY);
    getTextureImage(src, texture, toneImage);
    strokeImage.convertTo(strokeImage, toneImage.type());
    add(strokeImage, toneImage, toneImage);
//    imshow("tone", toneImage);
    waitKey(0);
    return 0;
}
