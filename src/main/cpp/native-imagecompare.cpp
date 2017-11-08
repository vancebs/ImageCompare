#include <jni.h>
#include <string>

#include "inc.h"

#include "BitmapUtils.h"
#include "ImageCompare.h"
//#include <android/bitmap.h>

#define CMAT(pointer) ((cv::Mat*)(pointer))

ImageCompare gImageCompare;

extern "C"
JNIEXPORT jint JNICALL Java_com_hf_imagecompare_ImageCompare_nativeDistance(JNIEnv *env, jobject /* this */, jlong mat1, jlong mat2, jint direction, jint threshold) {
    return gImageCompare.distance(*CMAT(mat1), *CMAT(mat2), direction, threshold);
}

extern "C"
JNIEXPORT void JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMerge(JNIEnv *env, jobject /* this */, jlong mat1, jlong mat2, jint distance, jlong matOut) {
    gImageCompare.merge(*CMAT(mat1), *CMAT(mat2), distance, *CMAT(matOut));
}

extern "C"
JNIEXPORT int JNICALL Java_com_hf_imagecompare_ImageCompare_nativeAutoMerge(JNIEnv *env, jobject /* this */, jlong mat1, jlong mat2, jint direction, jint threshold, jlong matOut) {
    int distance = gImageCompare.distance(*CMAT(mat1), *CMAT(mat2), direction, threshold);
    if (distance != INVALID_DISTANCE) {
        gImageCompare.merge(*CMAT(mat1), *CMAT(mat2), distance, *CMAT(matOut));
    }
    return distance;
}

extern "C"
JNIEXPORT jint JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatWidth(JNIEnv *env, jobject /* this */, jlong pointer) {
    if (pointer == NULL) {
        return 0;
    }

    return CMAT(pointer)->cols;
}

extern "C"
JNIEXPORT jint JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatHeight(JNIEnv *env, jobject /* this */, jlong pointer) {
    if (pointer == NULL) {
        return 0;
    }

    return CMAT(pointer)->rows;
}

extern "C"
JNIEXPORT jlong JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatFromBitmap(JNIEnv *env, jobject /* this */, jobject bmp) {
    Mat* pMat = new Mat();
    BitmapUtils::bitmapToMat(env, bmp, pMat, false);
    return (jlong) pMat;
}

extern "C"
JNIEXPORT void JNICALL Java_com_hf_imagecompare_ImageCompare_nativeBitmapFromMat(JNIEnv *env, jobject /* this */, jlong pointer, jobject bmpOut) {
    if (pointer == 0) {
        return;
    }

    BitmapUtils::mat2Bitmap(env, CMAT(pointer), bmpOut, false);
}

extern "C"
JNIEXPORT jlong JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatCreate(JNIEnv *env, jobject /* this */) {
    return (jlong) new cv::Mat();
}

extern "C"
JNIEXPORT void JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatRelease(JNIEnv *env, jobject /* this */, jlong pointer) {
    if (pointer == 0) {
        return;
    }

    delete CMAT(pointer);
}

extern "C"
JNIEXPORT jlong JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatFromFile(JNIEnv *env, jobject /* this */, jstring path) {
    const char* szPath = env->GetStringUTFChars(path, false);
    Mat mat = cv::imread(szPath);
    env->ReleaseStringUTFChars(path, szPath);

    return (jlong) new Mat(mat);
}

extern "C"
JNIEXPORT void JNICALL Java_com_hf_imagecompare_ImageCompare_nativeMatToFile(JNIEnv *env, jobject /* this */, jstring path, jlong pointer) {
    if ((pointer == 0) || (CMAT(pointer)->rows == 0) || (CMAT(pointer)->cols == 0)) {
        LOGD(DEBUG_TAG, "Invalid mat to save. pointer: %ld, width: %d, height: %d", pointer, (pointer==0)?0:CMAT(pointer)->cols, (pointer==0)?0:CMAT(pointer)->rows);
        return;
    }

    const char* szPath = env->GetStringUTFChars(path, false);
    cv::imwrite(szPath, *CMAT(pointer));
    env->ReleaseStringUTFChars(path, szPath);
}