//
// Created by vance on 2017/11/3.
//

#ifndef HSCREENSHOT_IMAGECOMPARE_H
#define HSCREENSHOT_IMAGECOMPARE_H

#include "opencv2/opencv.hpp"
using namespace cv;

#define DEFAULT_THRESHOLD 10

#define DIRECTION_ANY 0
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2

class ImageCompare {
public:
    ImageCompare();
    ~ImageCompare();

    int distance(Mat& mat1, Mat& mat2, int direction=DIRECTION_ANY, int threshold=DEFAULT_THRESHOLD);
    int distance2(Mat& mat1, Mat& mat2, int direction=DIRECTION_ANY, int threshold=DEFAULT_THRESHOLD);
    void merge(Mat& mat1, Mat& mat2, int distance, Mat& matOut);

private:
    inline bool isAvaliableDistance(int distance, int direction);
};


#endif //HSCREENSHOT_IMAGECOMPARE_H
