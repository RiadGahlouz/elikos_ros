#include "elikos_blobdetection_nodelet.hpp"
#include <cv_bridge/cv_bridge.h>
#include <pluginlib/class_list_macros.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

PLUGINLIB_EXPORT_CLASS(elikos_detection::BlobDetectionNodelet, nodelet::Nodelet)

namespace elikos_detection {

void BlobDetectionNodelet::onInit() {
    nodeHandle_ = this->getPrivateNodeHandle();  // NodeHandle("~")
    imageTransport_ = std::shared_ptr<image_transport::ImageTransport>(
        new image_transport::ImageTransport(nodeHandle_));
    publisher_ = imageTransport_->advertise("image_debug", 1);
    subscriber_ = imageTransport_->subscribe(
        "/camera/image_raw", 1, &BlobDetectionNodelet::onImageReceived, this);
}

void BlobDetectionNodelet::onImageReceived(
    const sensor_msgs::ImageConstPtr& image) {
    cv::Mat currentImage =
        cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8)->image;
    sensor_msgs::ImagePtr msgDebug =
        cv_bridge::CvImage(std_msgs::Header(), "bgr8", currentImage)
            .toImageMsg();
    publisher_.publish(msgDebug);
}

}  // namespace elikos_detection