//
// Created by olivier on 06/07/16.
//

#ifndef ELIKOS_DETECTION_ABSTRACTWRAPPER_H
#define ELIKOS_DETECTION_ABSTRACTWRAPPER_H

#include <opencv2/opencv.hpp>

class AbstractCVWrapper
{
    AbstractCVWrapper() = default;
    ~AbstractCVWrapper() = default;

    virtual void uploadMat(const cv::Mat& src) = 0;
    virtual cv::Mat& downloadMat() = 0;

    virtual void blur(cv::Size ksize, cv::Point anchor=cv::Point(-1, -1)) = 0;

    virtual void erode(cv::InputArray kernel, cv::Point anchor = cv::Point(-1, -1), int iterations = 1) = 0;

    virtual void dilate(cv::InputArray kernel, cv::Point anchor = cv::Point(-1, -1), int iterations=1) = 0;
};

#endif //ELIKOS_DETECTION_ABSTRACTWRAPPER_H
