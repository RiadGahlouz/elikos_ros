#include "Color.hpp"

namespace elikos_detection {

using namespace cv;
using namespace std;

Color::Color(unsigned hMin, unsigned hMax, unsigned sMin, unsigned sMax,
             unsigned vMin, unsigned vMax, unsigned pEro, unsigned d,
             unsigned postEro)
    : hueMin(hMin),
      hueMax(hMax),
      saturationMin(sMin),
      saturationMax(sMax),
      valueMin(vMin),
      valueMax(vMax),
      preErosions(pEro),
      dilations(d),
      postErosions(postEro) {}

void Color::detectColoredObjects(cv::Mat& input,
                                 std::vector<Blob>& result) const {
    Mat thresh = generateThreshold(input);

    // these two vectors needed for output of findContours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // find contours of filtered image using openCV findContours function
    findContours(thresh, contours, hierarchy, CV_RETR_CCOMP,
                 CV_CHAIN_APPROX_SIMPLE);
    // use moments method to find our filtered object
    double refArea = 0;

    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        // if number of objects greater than MAX_NUM_OBJECTS we have a noisy
        // filter
        if (numObjects < MAX_NUM_OBJECTS) {
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                result.emplace_back(Blob(this, unsigned(moment.m10 / area),
                                         unsigned(moment.m01 / area), area));
            }
        } else
            putText(input, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2,
                    Scalar(0, 0, 255), 2);
    }
}

cv::Mat Color::generateThreshold(const cv::Mat& in) const {
    cv::Mat threshold, hsv;
    cvtColor(in, hsv, COLOR_BGR2HSV);
    unsigned BLUR_AMOUNT = PRE_BLUR + 1;
    blur(hsv, hsv, Size(BLUR_AMOUNT, BLUR_AMOUNT), Point(-1, -1));

    // TODO optimize
    if (hueMin > hueMax) {
        Mat1b maxCol, minCol;
        inRange(hsv, Scalar(0, saturationMin, valueMin),
                Scalar(hueMax, saturationMax, valueMax), maxCol);
        inRange(hsv, Scalar(hueMin, saturationMin, valueMin),
                Scalar(180, saturationMax, valueMax), minCol);
        threshold = maxCol | minCol;
    } else {
        inRange(hsv, Scalar(hueMin, saturationMin, valueMin),
                Scalar(hueMax, saturationMax, valueMax), threshold);
    }
    // Consolidate the colored parts into one big blob to delimit the robot
    erode(threshold, threshold,
          getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1),
          preErosions);
    dilate(threshold, threshold,
           getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1),
           dilations);
    erode(threshold, threshold,
          getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1, -1),
          postErosions);

    return threshold;
}
}