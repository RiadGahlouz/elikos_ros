//
// Created by tonio on 28/05/15.
//

#ifndef RCRECEIVER_H
#define RCRECEIVER_H

#include <ros/ros.h>
#include <mavros/RCIn.h>
#include "./../../src/defines.cpp"

enum RCChannel { ROLL,
                 PITCH,
                 YAW,
                 THRUST,
                 OFFBOARD_SWITCH,
                 MODE_SWITCH,
                 POSCTL_SWITCH,
                 NUMBER_OF_CHANNELS};

class RCReceiver {
public:
    RCReceiver(ros::NodeHandle nh);
    ~RCReceiver();

    std::vector<unsigned int> getRCChannels() const;
    unsigned int operator[](std::size_t i) const;

private:
    void RCCallback(const mavros::RCInConstPtr rc);
    ros::NodeHandle* _nh;
    ros::Subscriber _rc_sub;
    std::vector<unsigned int> _rc_channels;
};


#endif