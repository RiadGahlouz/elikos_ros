#include "Blob.hpp"
#include <sstream>
#include <string>

namespace elikos_detection {

std::string intToString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}
Blob::Blob(const Color* color, unsigned x, unsigned y, double& area)
    : color_(color), x_(x), y_(y), area_(area) {}

void Blob::drawOn(cv::Mat& output) const {
    cv::circle(output, cv::Point(x_, y_), 10, cv::Scalar(0, 0, 255));
    cv::putText(output, intToString(x_) + " , " + intToString(y_),
                cv::Point(x_, y_ + 20), 1, 1, cv::Scalar(0, 255, 0));
}
}