
#include "moveit_move_group.h"

Moveit_move_group::Moveit_move_group():
  parent_frame_("elikos_arena_origin"),
  child_frame_("elikos_setpoint"),
  toleranceAchieveGoal_(0.4),
  toleranceNextGoal_(0.4),
  toleranceFreeOctomap_(0.1),
  isFirst_(true)
{
  //move_group settings
  group_.setPlanningTime(1.0);//In seconds
  //group_.setGoalTolerance (0.1);//In meters
  //The workspace represents the boundaries of the planning volume.
  group_.setWorkspace(-10,-10,0,10,10,3);

  group_.allowReplanning(true);

  group_.setNumPlanningAttempts(20);
}

Moveit_move_group::~Moveit_move_group()
{
}

void Moveit_move_group::move(geometry_msgs::PoseStamped target)
{
  std::vector<double> quad_variable_values;

  group_.getCurrentState()->copyJointGroupPositions(group_.getCurrentState()->getRobotModel()->getJointModelGroup(group_.getName()), quad_variable_values);

  //This is the new position and orientation for the quadcopter
  quad_variable_values[0] = target.pose.position.x;//position.x
  quad_variable_values[1] = target.pose.position.y;//position y
  quad_variable_values[2] = target.pose.position.z;//position z
  quad_variable_values[3] = 0;//target.pose.orientation.x;//quaternion x
  quad_variable_values[4] = 0;//target.pose.orientation.y;//quaternion y
  quad_variable_values[5] = 0;//target.pose.orientation.z;//quaternion z
  quad_variable_values[6] = 1;//target.pose.orientation.w;//quaternion w

  //Plan the goal pose
  group_.setJointValueTarget(quad_variable_values);

  //Plan and execute the trajectory
  //group_.asyncMove();
  moveit::planning_interface::MoveGroup::Plan plan;

  try
  {
    tf::StampedTransform currentPosition;
    listener.lookupTransform(parent_frame_, "elikos_fcu",
                              ros::Time(0), currentPosition);
    if(isFirst_ ||
        pow(target.pose.position.x-currentPosition.getOrigin().x(), 2)+
        pow(target.pose.position.y-currentPosition.getOrigin().y(), 2)+
        pow(target.pose.position.z-currentPosition.getOrigin().z(), 2) > pow(toleranceAchieveGoal_, 2))
    {

      ROS_ERROR_STREAM("New trajectory!");
      moveit_msgs::MoveItErrorCodes err = group_.plan(plan);

      if(err.val == moveit_msgs::MoveItErrorCodes::SUCCESS)
      {
        //Execute first point in trajectory.
        trajectory_msgs::MultiDOFJointTrajectory trajectoryToExecute = plan.trajectory_.multi_dof_joint_trajectory;
        listener.lookupTransform(parent_frame_, "elikos_fcu",
                                ros::Time(0), currentPosition);
        int i = 0;
        while(i < trajectoryToExecute.points.size()-1)
        {
            geometry_msgs::Vector3 targetTranslation = trajectoryToExecute.points[i].transforms[0].translation;
            if(pow(targetTranslation.x-currentPosition.getOrigin().x(), 2)+
                pow(targetTranslation.y-currentPosition.getOrigin().y(), 2)+
                pow(targetTranslation.z-currentPosition.getOrigin().z(), 2) > pow(toleranceNextGoal_,2))
              break;
            i++;
        }
        trajectoryPoint_ = trajectoryToExecute.points[i].transforms[0];
      }
      else
      {
        trajectoryPoint_.translation.x = currentPosition.getOrigin().x();
        trajectoryPoint_.translation.y = currentPosition.getOrigin().y();
        trajectoryPoint_.translation.z = 1.0;

        std_srvs::Empty::Request req;
        std_srvs::Empty::Response res;
        ros::service::call("/clear_octomap", req, res);
      }
      //Set the rotation to face the direction which it is heading.
      tf::Quaternion rotation = tf::createIdentityQuaternion();
      double direction = cv::fastAtan2(trajectoryPoint_.translation.y - currentPosition.getOrigin().y(), trajectoryPoint_.translation.x - currentPosition.getOrigin().x()) / 360 * 2 *PI;
      rotation.setRPY((double) 0.0 , (double) 0.0, direction);

      tf::quaternionTFToMsg(rotation, trajectoryPoint_.rotation);

      isFirst_ = false;
    }

    publishTrajectoryPoint(trajectoryPoint_);

  }
  catch (tf::TransformException ex){
   ROS_ERROR("%s",ex.what());
   ros::Duration(1.0).sleep();
  }
}

void Moveit_move_group::publishTrajectoryPoint(geometry_msgs::Transform_<std::allocator<void> > trajectoryPoint)
{
  //Convert geometry_msgs::Transform to tf::Transform
  tf::Transform tfTrajectoryPoint;
  tf::transformMsgToTF(trajectoryPoint, tfTrajectoryPoint);

  //Broadcast command
  tf_broadcaster_.sendTransform(tf::StampedTransform(tfTrajectoryPoint, ros::Time::now(), parent_frame_, child_frame_));
}
