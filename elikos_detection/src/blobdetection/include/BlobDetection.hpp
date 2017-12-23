#ifndef BLOB_DETECTION
#define BLOB_DETECTION

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Blob.hpp"
#include "Color.hpp"

namespace elikos_detection {
class BlobDetection {
   public:
    void detect(cv::Mat& in, std::vector<Blob>& result) const;
    void addColor(const Color& color);

   private:
    std::vector<Color> colors_;
};
}

#endif