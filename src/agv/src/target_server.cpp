#include <agv/target_poseAction.h>
#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>

typedef actionlib::SimpleActionServer<agv::target_poseAction> Server;

void execute(const agv::target_poseGoalConstPtr& goal, Server* as)  // Note: "Action" is not appended to DoDishes here
{
  // Do lots of awesome groundbreaking robot stuff here
  as->setSucceeded();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "target_server");
  ros::NodeHandle n;
  Server server(n, "target_server", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}