//
// Created by vance on 2017/11/3.
//

#include "inc.h"
#include "ImageCompare.h"

#define THRESHOLD_X 10

ImageCompare::ImageCompare() {
}

ImageCompare::~ImageCompare() {

}

int ImageCompare::distance(Mat& mat1, Mat& mat2, int direction, int threshold) {
    TRACE_INIT(DEBUG_TAG);
    LOGD("==MyTest==", "distance()# 1");

    // get points & descriptors
    TRACE_BEGIN
    cv::Ptr<cv::FeatureDetector> detector = ORB::create();
    std::vector<KeyPoint> points1, points2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(mat1, noArray(), points1, descriptors1);
    detector->detectAndCompute(mat2, noArray(), points2, descriptors2);
    TRACE_END("get points & descriptors")

    LOGD("==MyTest==", "distance()# 2");
    // match
    TRACE_BEGIN
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create();
    std::vector<DMatch> matches;
    matcher->match(descriptors1, descriptors2, matches, noArray());
    TRACE_END("match")

    LOGD("==MyTest==", "distance()# 3");
    // generate distance map
    TRACE_BEGIN
    std::map<int, int> distanceMap;
    std::vector<DMatch>::iterator it;
    std::map<int, int>::iterator it_find;
    for (it = matches.begin(); it != matches.end(); it++) {
        KeyPoint& point1 = points1[(*it).queryIdx];
        KeyPoint& point2 = points2[(*it).trainIdx];

        if (abs(point1.pt.x - point2.pt.x) < THRESHOLD_X) {
            int distance = (int) (point1.pt.y - point2.pt.y);
            it_find = distanceMap.find(distance);
            if (it_find != distanceMap.end()) {
                (*it_find).second ++;
            } else {
                distanceMap.insert(std::make_pair(distance, 1));
            }
        }
    }
    TRACE_END("generate distance map")

    LOGD("==MyTest==", "distance()# 4");
    // find best match distance
    TRACE_BEGIN
    int bestDistance = INVALID_DISTANCE;
    int bestDistanceCount = 0;
    std::map<int, int>::iterator map_item;
    for (map_item = distanceMap.begin(); map_item != distanceMap.end(); map_item++) {
        LOGD("==MyTest==", "distance: %d, count: %d, threshold: %d", (*map_item).first, (*map_item).second, threshold);
        if (isAvaliableDistance((*map_item).first, direction) && ((*map_item).second >= threshold) && ((*map_item).second > bestDistanceCount)) {
            bestDistance = (*map_item).first;
            bestDistanceCount = (*map_item).second;
        }
    }
    TRACE_END("find best match")

    TRACE_FINISH
    return bestDistance;
}

int ImageCompare::distance2(Mat& mat1, Mat& mat2, int direction, int threshold) {
    TRACE_INIT(DEBUG_TAG);
    LOGD("==MyTest==", "distance()# 1");

    // generate mask
    TRACE_BEGIN
    Mat mask;
    cv::absdiff(mat1, mat2, mask);
    cv::cvtColor(mask, mask, CV_BGR2GRAY);
    cv::threshold(mask, mask, 0, 255, cv::THRESH_BINARY);
    TRACE_END("generate mask")

    LOGD("==MyTest==", "distance()# 2");

    // get points & descriptors
    TRACE_BEGIN
    cv::Ptr<cv::FeatureDetector> detector = ORB::create();
    std::vector<KeyPoint> points1, points2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(mat1, mask, points1, descriptors1);
    detector->detectAndCompute(mat2, mask, points2, descriptors2);
    TRACE_END("get points & descriptors")

    LOGD("==MyTest==", "distance()# 3");
    // match
    TRACE_BEGIN
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create();
    std::vector<DMatch> matches;
    matcher->match(descriptors1, descriptors2, matches, noArray());
    TRACE_END("match")

    LOGD("==MyTest==", "distance()# 4");
    // generate distance map
    TRACE_BEGIN
    std::map<int, int> distanceMap;
    std::vector<DMatch>::iterator it;
    std::map<int, int>::iterator it_find;
    for (it = matches.begin(); it != matches.end(); it++) {
        KeyPoint& point1 = points1[(*it).queryIdx];
        KeyPoint& point2 = points2[(*it).trainIdx];

        if (abs(point1.pt.x - point2.pt.x) < THRESHOLD_X) {
            int distance = (int) (point1.pt.y - point2.pt.y);
            it_find = distanceMap.find(distance);
            if (it_find != distanceMap.end()) {
                (*it_find).second ++;
            } else {
                distanceMap.insert(std::make_pair(distance, 1));
            }
        }
    }
    TRACE_END("generate distance map")

    LOGD("==MyTest==", "distance()# 5");
    // find best match distance
    TRACE_BEGIN
    int bestDistance = INVALID_DISTANCE;
    int bestDistanceCount = 0;
    std::map<int, int>::iterator map_item;
    for (map_item = distanceMap.begin(); map_item != distanceMap.end(); map_item++) {
        LOGD("==MyTest==", "distance: %d, count: %d, threshold: %d", (*map_item).first, (*map_item).second, threshold);
        if (isAvaliableDistance((*map_item).first, direction) && ((*map_item).second >= threshold) && ((*map_item).second > bestDistanceCount)) {
            bestDistance = (*map_item).first;
            bestDistanceCount = (*map_item).second;
        }
    }
    TRACE_END("find best match")

    TRACE_FINISH
    return (bestDistance == 0) ? INVALID_DISTANCE : bestDistance;
}

void ImageCompare::merge(Mat& mat1, Mat& mat2, int distance, Mat& matOut) {
    Mat* pMatPrev;
    Mat* pMatNext;
    if (distance == 0) {
        mat1.copyTo(matOut);
        return;
    } else if (distance < 0) {
        pMatPrev = &mat2;
        pMatNext = &mat1;
        distance = -distance;
    } else {
        pMatPrev = &mat1;
        pMatNext = &mat2;
    }

    // copy
    int width = pMatPrev->cols;
    int duplicateRange = pMatPrev->rows - distance;
    int fromPrevY = 0;
    int fromPrevHeight = pMatPrev->rows - (duplicateRange / 2);
    int toPrevY = 0;
    int toPrevHeight = fromPrevHeight;
    int fromNextY = duplicateRange - (duplicateRange / 2);
    int fromNextHeight = pMatNext->rows - fromNextY;
    int toNextY = toPrevHeight;
    int toNextHeight = fromNextHeight;
    matOut.create(pMatPrev->rows + distance, pMatPrev->cols, pMatPrev->type());
    (*pMatPrev)(Rect(0, fromPrevY, width, fromPrevHeight)).copyTo(matOut(Rect(0, toPrevY, width, toPrevHeight)));
    (*pMatNext)(Rect(0, fromNextY, width, fromNextHeight)).copyTo(matOut(Rect(0, toNextY, width, toNextHeight)));
}

bool ImageCompare::isAvaliableDistance(int distance, int direction) {
    switch (direction) {
        case DIRECTION_ANY:
            return distance != 0;
        case DIRECTION_UP:
            return distance < 0;
        case DIRECTION_DOWN:
            return distance > 0;
    }
}