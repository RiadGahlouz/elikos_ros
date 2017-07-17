#ifndef AI_MESSAGE_HANDLER_H
#define AI_MESSAGE_HANDLER_H

#include <string>
#include <ros/ros.h>
#include <tf/transform_listener.h>

#include <elikos_ros/TargetRobot.h>
#include <elikos_ros/TargetRobotArray.h>
#include <elikos_ros/AICmd.h>
#include <tf/transform_broadcaster.h>
#include <CmdDefines.h>

namespace tf
{
    class Vector3;
}

namespace ai
{
const std::string TRGT_TOPIC { "/elikos_track_robot_array" };
const std::string SETPOINT_TOPIC { "elikos_ai_sim" };
const std::string CMD_TOPIC { "elikos_ai_cmd" };
const std::string WORLD_FRAME = { "elikos_arena_origin" };
const std::string MAV_FRAME = { "elikos_fcu" };
class Agent;

class MessageHandler
{
public:

    static MessageHandler* getInstance();
    static void freeInstance();

    void lookForMessages();
    void lookForMav();
    void sendDestination(const tf::Vector3& destination, CmdCode cmd_code);

private:
    static MessageHandler* instance_;

    ros::NodeHandle nh_;
    ros::Subscriber trgtSub_;
    tf::TransformListener mavListener_;

    ros::Publisher simPub_;
    ros::Publisher cmdPub_;
    tf::TransformBroadcaster br_;
    bool is_simulation_;

    Agent* agent_{ nullptr };

    void handleTrgtMsg(const elikos_ros::TargetRobotArray::ConstPtr& input);

    MessageHandler();
    ~MessageHandler() = default;
};

}

#endif /// MESSAGE_HANDLER
