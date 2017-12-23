#include "elikos_blobdetection_nodelet.hpp"
#include <cv_bridge/cv_bridge.h>
#include <pluginlib/class_list_macros.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

PLUGINLIB_EXPORT_CLASS(elikos_detection::BlobDetectionNodelet, nodelet::Nodelet)

namespace elikos_detection {

void BlobDetectionNodelet::onInit() {
    nodeHandle_ = this->getPrivateNodeHandle();  // NodeHandle("~")
    imageTransport_ = std::shared_ptr<image_transport::ImageTransport>(
        new image_transport::ImageTransport(nodeHandle_));
    publisher_ = imageTransport_->advertise("image_debug", 1);
    subscriber_ = imageTransport_->subscribe(
        "/camera/image_raw", 1, &BlobDetectionNodelet::onImageReceived, this);

    blobDetection_.addColor(Color(0, 48, 93, 256, 141, 256, 12, 25, 5));  // Red
    // TODO Load colors from  YAML
}

void BlobDetectionNodelet::onImageReceived(
    const sensor_msgs::ImageConstPtr& image) {
    cv::Mat currentImage =
        cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8)->image;
    std::vector<Blob> detectedBlobs;
    blobDetection_.detect(currentImage, detectedBlobs);
    for (unsigned i = 0; i < detectedBlobs.size(); ++i) {
        const Blob& blob = detectedBlobs[i];
        blob.drawOn(currentImage);
    }
    sensor_msgs::ImagePtr msgDebug =
        cv_bridge::CvImage(std_msgs::Header(), "bgr8", currentImage)
            .toImageMsg();
    publisher_.publish(msgDebug);
}

}  // namespace elikos_detection