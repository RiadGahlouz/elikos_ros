#include "BlobDetection.hpp"

namespace elikos_detection {

void BlobDetection::detect(cv::Mat& in, std::vector<Blob>& result) const {
    for (unsigned i = 0; i < colors_.size(); ++i) {
        const Color& color = colors_[i];
        color.detectColoredObjects(in, result);
    }
}

void BlobDetection::addColor(const Color& color) { colors_.push_back(color); }
}