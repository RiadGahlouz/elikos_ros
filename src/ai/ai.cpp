#include <memory>
#include <ros/ros.h>
#include <ros/package.h>
#include "MessageHandler.h"
#include "Agent.h"

int main(int argc, char* argv[])
{
    // ROS Init
    ros::init( argc, argv, "elikos_ai" );
    ros::Rate r(30);

    std::unique_ptr<ai::Agent> agent;
    agent = std::unique_ptr<ai::Agent>(new ai::Agent());
    ai::MessageHandler mh(agent.get());

    // Endless loop
    while(ros::ok()) 
    {
        mh.lookupTransform();
        ros::spinOnce();
        r.sleep();
    }
}