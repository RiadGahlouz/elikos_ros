//
// Created by olivier on 06/07/16.
//

#ifndef ELIKOS_DETECTION_CPUWRAPPER_H
#define ELIKOS_DETECTION_CPUWRAPPER_H

#include "AbstractCVWrapper.h"


class CpuCV : public AbstractCVWrapper
{
    CpuCV() = default;
    ~CpuCV() = default;

    virtual void uploadMat(const cv::Mat& src);
    virtual cv::Mat& downloadMat();

    virtual void blur(cv::Size ksize, cv::Point anchor=cv::Point(-1, -1));

    virtual void erode(cv::InputArray kernel, cv::Point anchor = cv::Point(-1, -1), int iterations = 1);

    virtual void dilate(cv::InputArray kernel, cv::Point anchor = cv::Point(-1, -1), int iterations=1);

private:
    cv::Mat mat_;
};


#endif //ELIKOS_DETECTION_CPUWRAPPER_H
