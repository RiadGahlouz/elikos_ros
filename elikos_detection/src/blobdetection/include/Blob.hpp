#ifndef DETECTION_BLOB_H
#define DETECTION_BLOB_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace elikos_detection {
class Color;
class Blob {
   public:
    Blob(const Color* color, unsigned x, unsigned y, double& area);

    void drawOn(cv::Mat& output) const;

    const Color* getColor() const { return color_; };
    unsigned getX() const { return x_; };
    unsigned getY() const { return y_; };
    double getArea() const { return area_; };

   private:
    const Color* color_;
    unsigned x_;
    unsigned y_;
    double area_;
};
}
#endif