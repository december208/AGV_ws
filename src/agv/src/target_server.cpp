#include <agv/targetAction.h>
#include <agv/turnAction.h>
#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>
#include <tf2/transform_datatypes.h>
#include <tf2/LinearMath/Transform.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

typedef actionlib::SimpleActionServer<agv::targetAction> Server;
actionlib::SimpleActionClient<agv::turnAction> turn_ac("turnAction", true);

void execute(const agv::targetGoalConstPtr& goal, Server* as)  
{
  ros::Rate rate(20);
  ROS_INFO("Waiting for turn action server to start.");
  turn_ac.waitForServer();
  agv::turnGoal turn_goal;
  turn_goal.trigger.data = "Turn!";
  turn_ac.sendGoal(turn_goal);
  while(ros::ok() && (!as->isPreemptRequested())){
    if(turn_ac.getState())
    ros::spinOnce();
    rate.sleep();
  }
  as->setSucceeded();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "target_server");
  ros::NodeHandle n;
  Server server(n, "targetAction", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}