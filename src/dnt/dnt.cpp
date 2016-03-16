#include <memory>
#include <ros/ros.h>

#include "MessageHandler.h"

int main(int argc, char* argv[])
{
    // ROS Init
    ros::init( argc, argv, "elikos_dnt" );
   
    MessageHandler messageHandler;

    ros::Rate r(30);
    // Endless loop
    while(ros::ok()) 
    {
        ros::spinOnce();
        r.sleep();
    }
}