#ifndef AI_TRANSLATOR
#define AI_TRANSLATOR

#include <string>
#include <ros/ros.h>
#include <tf/transform_listener.h>

namespace ai
{

class Agent;

class MessageHandler
{
public:
    static const int N_TRGT = 10;
    static const int N_OBS = 4;

    static const std::string MAV_FRAME;
    static const std::string WORLD_FRAME;
    static const std::string TRGT_FRAME;
    static const std::string OBS_FRAME;

    static MessageHandler* getInstance();
    static void freeInstance();

    void lookForTf();
    void lookForMessages();

private:
    ~MessageHandler() = default;
    MessageHandler();

    static MessageHandler* instance_;

    ros::NodeHandle nh_;
    tf::TransformListener listener_;
    Agent* agent_{ nullptr };

    void lookForMAV();
};

}

#endif /// MESSAGE_HANDLER
