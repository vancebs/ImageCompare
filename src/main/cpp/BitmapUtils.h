//
// Created by vance on 2017/11/3.
//

#ifndef HSCREENSHOT_UTILS_H
#define HSCREENSHOT_UTILS_H

#include "opencv2/opencv.hpp"
using namespace cv;

#include <jni.h>

class BitmapUtils {
public:
    static void bitmapToMat(JNIEnv * env, jobject bitmap, Mat* pMat, bool needUnPremultiplyAlpha);
    static void mat2Bitmap(JNIEnv * env, Mat* pMat, jobject bitmap, bool needPremultiplyAlpha);

};


#endif //HSCREENSHOT_UTILS_H
