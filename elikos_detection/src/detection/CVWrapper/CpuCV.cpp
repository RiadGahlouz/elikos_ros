//
// Created by olivier on 06/07/16.
//

#include "CpuCV.h"

void CpuCV::uploadMat(const cv::Mat& src)
{
    mat_ = src;
}

cv::Mat& CpuCV::downloadMat()
{
    return mat_;
}

void CpuCV::blur(cv::Size ksize, cv::Point anchor)
{
    cv::blur(mat_, mat_, ksize, anchor);
}

void CpuCV::erode(cv::InputArray kernel, cv::Point anchor, int iterations)
{
    cv::erode(mat_, mat_, kernel, anchor, iterations);
}

void CpuCV::dilate(cv::InputArray kernel, cv::Point anchor, int iterations)
{
    cv::erode(mat_, mat_, kernel, anchor, iterations);
}


