//
// Created by andre on 15/07/15.
//

#include <elikos_lib/params_helper.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include "PoseOffsetNode.h"
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Header.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/Pose.h>
#include <elikos_lib/PositionHelper.h>
#include <pose_offset/SvoCtrl.h>
#include <pose_offset/McptamCtrl.h>

PoseOffsetNode::PoseOffsetNode() :
    _n("/pose_offset"),
    _local_pos_valid(false),
    _vo_valid(false)
{
    //setup parameters
    this->_publish_covariance = elikos::getParam<bool>("publish_covariance", true);
    this->_recv_covariance = elikos::getParam<bool>("receive_pose_with_covariance", true);
    this->_rate = elikos::getParam<int>("rate", 100);
    this->_timeout = elikos::getParam<int>("timeout", 1000);
    this->_vo_method_s = elikos::getParam<std::string>("vo_method", "svo");


    if(this->_publish_covariance && !this->_recv_covariance){
        ROS_ERROR("How the fuck do you want me to publish a covariance matrix if you set me to not receive one.");
        ROS_WARN("PoseOffsetNode defaulting to publish_covariance = false and receive_pose_with_covariance = false");
        this->_publish_covariance = false;
        this->_recv_covariance;
    }
}


void PoseOffsetNode::setupVoCtrl(std::string method){
    std::transform(method.begin(), method.end(), method.begin(), ::tolower);
    if(method.compare("svo")){
        this->_vo_method = elikos::SupportedVO::SVO;
        this->_vo_controller = new elikos::SvoCtrl(this->_n);
    } else if (method.compare("mcptam")) {
        this->_vo_method = elikos::SupportedVO::MCPTAM;
        this->_vo_controller = new elikos::McptamCtrl(this->_n);
    } else {
        ROS_ERROR("Unsupported VO method configured for PoseOffsetNode");
        std::exception e();
    }
}

void PoseOffsetNode::LocalPosCallback(const geometry_msgs::PoseStampedConstPtr pose)
{
    if(!_local_pos_valid) {
        ROS_WARN("Local pose now valid.");
    }
    _local_pose = *pose;
    _local_pos_valid = true;
}

void PoseOffsetNode::VOPoseCallback_Cov(const geometry_msgs::PoseWithCovarianceStampedPtr pose)
{
    if(!_vo_valid){
        ROS_WARN("VO pose now valid.");
        //save the offset position ever since VO started or came back
        this->_offset_pose = _local_pose;
    }
    this->_vo_pose = *pose;
    this->_vo_valid = true;

    //publish pose offset at the same rate as VO
    if(_local_pos_valid && _vo_valid){
        if (this->_publish_covariance) {
            geometry_msgs::PoseWithCovarianceStamped offset_pose;
            offset_pose.header.stamp = this->_vo_pose.header.stamp;

            //hopefully what this does is keep the VO covariance matrix
            //and update position and orientation with offsets
            offset_pose.pose = _vo_pose.pose;
            offset_pose.pose.pose.position = elikos::tools::addPoints(offset_pose.pose.pose.position, this->_offset_pose.pose.position);
            offset_pose.pose.pose.orientation = elikos::tools::addQuaternions(offset_pose.pose.pose.orientation, this->_offset_pose.pose.orientation);

            this->_pose_pub.publish(offset_pose);
        } else {
            geometry_msgs::PoseStamped offset_pose;
            offset_pose.header.stamp = this->_vo_pose.header.stamp;
            offset_pose.pose.position = elikos::tools::addPoints(this->_vo_pose.pose.pose.position, this->_offset_pose.pose.position);
            offset_pose.pose.orientation = elikos::tools::addQuaternions(this->_vo_pose.pose.pose.orientation, this->_offset_pose.pose.orientation);

            this->_pose_pub.publish(offset_pose);
        }
    }
}

void PoseOffsetNode::VOPoseCallback(const geometry_msgs::PoseStampedPtr pose)
{
    if(!_vo_valid){
        ROS_WARN("VO pose now valid.");
        //save the offset position ever since VO started or came back
        this->_offset_pose = _local_pose;
    }

    this->_vo_pose.pose.pose = pose->pose;

    this->_vo_valid = true;

    if(_local_pos_valid && _vo_valid){
        //no covariance matrix to send out :(
        geometry_msgs::PoseStamped offset_pose;
        offset_pose.header.stamp = this->_vo_pose.header.stamp;

        offset_pose.pose.position = elikos::tools::addPoints(this->_vo_pose.pose.pose.position, this->_offset_pose.pose.position);
        offset_pose.pose.orientation = elikos::tools::addQuaternions(this->_vo_pose.pose.pose.orientation, this->_offset_pose.pose.orientation);

        this->_pose_pub.publish(offset_pose);
    }
}

int PoseOffsetNode::main(int argc, char** argv)
{
    ros::init(argc, argv, "pose_offset");
    ros::Rate r(this->_rate);   //Not sure what rate to put...
                                //it's not so bad since the node will be driven by VO publishing
    ros::Duration timeout(this->_timeout/1000);

    //setup publications
    if (this->_publish_covariance) {
        _pose_pub = _n.advertise<geometry_msgs::PoseWithCovarianceStamped>("pose", 1);
    } else {
        _pose_pub = _n.advertise<geometry_msgs::PoseStamped>("pose", 1);
    }

    //setup subscriptions and callbacks
    _local_pos_sub = _n.subscribe("local_position", 1, &PoseOffsetNode::LocalPosCallback, this);
    if(this->_recv_covariance) {
        _vo_sub = _n.subscribe("pose", 1, &PoseOffsetNode::VOPoseCallback_Cov, this);
    } else {
        _vo_sub = _n.subscribe("pose", 1, &PoseOffsetNode::VOPoseCallback, this);
    }

    setupVoCtrl(this->_vo_method_s);

    while(ros::ok()){
        if(ros::Time::now() - _local_pose.header.stamp > timeout){
            if(this->_local_pos_valid){
                ROS_ERROR("Local Pose timed out.");
            }
            this->_local_pos_valid = false;
        }

        if(ros::Time::now() - _vo_pose.header.stamp > timeout) {
            if(this->_vo_valid){
                ROS_ERROR("VO timed out.");
            }
            this->_vo_valid = false;
        }

        if(!this->_vo_valid){
            //shit timed out yo, try to reset and restart
            this->_vo_controller->reset();
            this->_vo_controller->init();
        }

        ros::spinOnce();
        r.sleep();
    }
}
