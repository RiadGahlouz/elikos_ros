#ifndef DETECTION_AND_TRACKING_COLOR_H
#define DETECTION_AND_TRACKING_COLOR_H

#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include "Blob.hpp"
namespace elikos_detection {
class Color {
   public:
    Color(unsigned hMin, unsigned hMax, unsigned sMin, unsigned sMax,
          unsigned vMin, unsigned vMax, unsigned pEro, unsigned d,
          unsigned postEro);

    void detectColoredObjects(cv::Mat& input, std::vector<Blob>& result) const;

   private:
    unsigned hueMin;
    unsigned hueMax;
    unsigned saturationMin;
    unsigned saturationMax;
    unsigned valueMin;
    unsigned valueMax;

    unsigned preErosions;
    unsigned dilations;
    unsigned postErosions;

    unsigned PRE_BLUR = 8;
    unsigned MAX_NUM_OBJECTS = 50;

    cv::Mat generateThreshold(const cv::Mat& in) const;
};
}

#endif