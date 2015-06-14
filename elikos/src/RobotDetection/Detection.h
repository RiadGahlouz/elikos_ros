
#ifndef DETECT_DETECTION_H
#define DETECT_DETECTION_H

#include <sstream>
#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "std_msgs/String.h"
#include "RobotDesc.h"
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "./../defines.cpp"
#include <sensor_msgs/image_encodings.h>
#include <elikos_ros/RobotPos.h>
#include <elikos_ros/RobotsPos.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

using namespace std;
using namespace cv;

namespace elikos_detection {

    class Detection {
    public:

        Detection(ros::NodeHandle *nh);
        ~Detection();

        void init();

        void cameraCallback(const sensor_msgs::ImageConstPtr &msg);

        Mat getCurrentImage(){return currentImage;}
        cv_bridge::CvImagePtr getNextImage(){return nextImage;}

        void setCurrentImage(Mat image){currentImage = image;}
        void setNextImage(cv_bridge::CvImagePtr image){nextImage = image;}

        void setCapture(VideoCapture capture) {Detection::capture = capture;}

        VideoCapture getCapture(){return capture;}

        void trackFilteredObjects(Mat threshold,Mat HSV, Mat &cameraFeed);
        void drawObject(vector<RobotDesc> vecRobot,Mat &frame);
        void morphOps(Mat &thresh);
        void cannyEdge();
        void sendMsg();

        string intToString(int number);

        // TF methods
        void initCameraTF();
        void computeTargetPosition();

    /* *************************************************************************************************
     * ***              DEBUG FUNCTIONS
     * *************************************************************************************************
     */

        void createTrackbars();
        void setupDebug();
        void captureFrame();
        void trackRobots();
        void showCurrentImage();
        void showThreshold();


    private:

    /* *************************************************************************************************
     * ***           ROS RELATED FUNCTIONS
     * *************************************************************************************************
     */
        void setPublishers();
        void setSubscribers();

    /* *************************************************************************************************
     * ***           ATTRIBUTES
     * *************************************************************************************************
     */

        Mat currentImage;
        cv_bridge::CvImagePtr nextImage;
        elikos_ros::RobotsPos robotsPos_msg;
        vector<RobotDesc> vecRobot;
        ros::NodeHandle* nh_;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        ros::Publisher robots_publish;

        // TF transforms
        tf::TransformBroadcaster tf_broadcaster_;
        tf::TransformListener tf_listener_;
        tf::Transform camera_;
        tf::Transform turret_;
        tf::StampedTransform turret_world_;
        tf::Vector3 turret_world_x_;
        tf::Transform target_robot_;


    /* *************************************************************************************************
     * ***              DEBUG ATTRIBUTES
     * *************************************************************************************************
     */

        Mat threshold;
        Mat hsv;
        VideoCapture capture;

    /* *************************************************************************************************
     * ***               CLASS CONSTANTS
     * *************************************************************************************************
     */

        int H_MIN;
        int H_MAX;
        int S_MIN;
        int S_MAX;
        int V_MIN;
        int V_MAX;

        const int FRAME_WIDTH = 640;
        const int FRAME_HEIGHT = 480;

        const int MAX_NUM_OBJECTS = 50;

        const int MIN_OBJECT_AREA = 40 * 40;
        const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

        const string windowName = "Original Image";
        const string windowName1 = "HSV Image";
        const string windowName2 = "Thresholded Image";
        const string windowName3 = "After Morphological Operations";
        const string trackbarWindowName = "Trackbars";


    };

}

#endif