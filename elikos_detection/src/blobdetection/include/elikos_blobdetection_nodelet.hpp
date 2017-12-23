#ifndef ELIKOS_DETECTION_NODELET
#define ELIKOS_DETECTION_NODELET

#include <image_transport/image_transport.h>
#include <nodelet/nodelet.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <memory>
#include "BlobDetection.hpp"

namespace elikos_detection {

class BlobDetectionNodelet : public nodelet::Nodelet {
   private:
    void onInit();

    /**************************************************************************
     * Nodelet IN/OUT
     * ***********************************************************************/
    ros::NodeHandle nodeHandle_;
    std::shared_ptr<image_transport::ImageTransport> imageTransport_;
    image_transport::Publisher publisher_;
    image_transport::Subscriber subscriber_;

    /**************************************************************************
     * BLOB DETECTION ALGO
     * ***********************************************************************/
    BlobDetection blobDetection_;

    /**************************************************************************
     * CallBack
     * ***********************************************************************/
    void onImageReceived(const sensor_msgs::ImageConstPtr& image);
};

}  // namespace elikos_detection
#endif