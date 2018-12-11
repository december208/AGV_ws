#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_target_tf2_listener");

  ros::NodeHandle node;

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  ros::Rate rate(60.0);
  while (node.ok()){
    geometry_msgs::TransformStamped tf_robot;
    geometry_msgs::TransformStamped tf_target;
    try{
      tf_robot = tfBuffer.lookupTransform("odemetry", "robot",ros::Time(0));
      tf_target = tfBuffer.lookupTransform("robot", "target",ros::Time(0));
    }
    catch (tf2::TransformException &ex) {
      ROS_WARN("%s",ex.what());
      ros::Duration(0.5).sleep();
      continue;
    }

    geometry_msgs::Pose robot_pose;
    geometry_msgs::Pose targetWRTrobot;

    

    rate.sleep();
  }
  return 0;
};