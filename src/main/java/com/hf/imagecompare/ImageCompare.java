package com.hf.imagecompare;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

/**
 * Image Compare<br>
 * Compare image with openCV
 */
public class ImageCompare {
    private static final int DEFAULT_THRESHOLD = 10;
    public static final int INVALID_DISTANCE = Integer.MAX_VALUE;

    public static final int DIRECTION_ANY = 0;
    public static final int DIRECTION_UP = 1;
    public static final int DIRECTION_DOWN = 2;

    public int distance(Mat mat1, Mat mat2, int direction) {
        return distance(mat1, mat2, direction, DEFAULT_THRESHOLD);
    }

    public int distance(Mat mat1, Mat mat2, int direction, int threshold) {
        if (mat1.mNativePointer == 0 || mat2.mNativePointer == 0) {
            throw new NullPointerException("invalid mat");
        }

        return nativeDistance(mat1.mNativePointer, mat2.mNativePointer, direction, threshold);
    }

    public Mat merge(Mat mat1, Mat mat2, int distance) {
        if (mat1.mNativePointer == 0 || mat2.mNativePointer == 0) {
            throw new NullPointerException("invalid mat");
        }

        Mat outMat = Mat.createMat();
        nativeMerge(mat1.mNativePointer, mat2.mNativePointer, distance, outMat.mNativePointer);
        return outMat;
    }

    public Mat autoMerge(Mat mat1, Mat mat2, int direction,  int[] outDistance) {
        return autoMerge(mat1, mat2, direction, DEFAULT_THRESHOLD, outDistance);
    }

    public Mat autoMerge(Mat mat1, Mat mat2, int direction, int threshold, int[] outDistance) {
        if (mat1.mNativePointer == 0 || mat2.mNativePointer == 0) {
            throw new NullPointerException("invalid mat");
        }

        // do auto merge
        Mat outMat = Mat.createMat();
        int distance = nativeAutoMerge(mat1.mNativePointer, mat2.mNativePointer, direction, threshold, outMat.mNativePointer);

        // return out distance
        if (outDistance != null && outDistance.length > 0) {
            outDistance[0] = distance;
        }

        // return
        return outMat;
    }

    private static native int nativeDistance(long mat1, long mat2, int direction, int threshold);
    private static native void nativeMerge(long mat1, long mat2, int distance, long matOut);
    private static native int nativeAutoMerge(long mat1, long mat2, int direction, int threshold, long matOut);

    private static native long nativeMatFromBitmap(Bitmap bmp);
    private static native void nativeBitmapFromMat(long pointer, Bitmap bmpOut);
    private static native long nativeMatFromFile(String path);
    private static native void nativeMatToFile(String path, long pointer);
    private static native long nativeMatCreate();
    private static native void nativeMatRelease(long pointer);
    private static native int nativeMatWidth(long pointer);
    private static native int nativeMatHeight(long pointer);

    public static class Mat {
        private long mNativePointer;

        private Mat(long pointer) {
            mNativePointer = pointer;
        }

        public int getWidth() {
            return nativeMatWidth(mNativePointer);
        }

        public int getHeight() {
            return nativeMatHeight(mNativePointer);
        }

        public void release() {
            if (mNativePointer != 0) {
                nativeMatRelease(mNativePointer);
            }
        }

        public Bitmap toBitmap() {
            return toBitmap(this);
        }

        public void toFile(String path) {
            toFile(path, this);
        }

        public static Mat createMat() {
            return new Mat(nativeMatCreate());
        }

        public static Mat fromFile(String path) {
            return new Mat(nativeMatFromFile(path));
        }

        public static void toFile(String path, Mat mat) {
            nativeMatToFile(path, mat.mNativePointer);
        }

        public static Mat fromBitmap(Bitmap bitmap) {
            long pointer = nativeMatFromBitmap(bitmap);
            if (pointer == 0) {
                return null;
            } else {
                return new Mat(pointer);
            }
        }

        public static Bitmap toBitmap(Mat mat) {
            if (mat.mNativePointer == 0) {
                return null;
            } else {
                Bitmap bitmap = Bitmap.createBitmap(mat.getWidth(), mat.getHeight(), Bitmap.Config.ARGB_8888);
                nativeBitmapFromMat(mat.mNativePointer, bitmap);
                return bitmap;
            }
        }
    }

    static {
        System.loadLibrary("libImageCompare");
    }
}
