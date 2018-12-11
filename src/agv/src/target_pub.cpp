#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>
#include <tf2/transform_datatypes.h>
#include <tf2/LinearMath/Transform.h>

geometry_msgs::TransformStamped transformStamped;
tf2_ros::TransformBroadcaster br;  


void targetCallback(const geometry_msgs::PoseConstPtr& msg){
  transformStamped.header.stamp = ros::Time::now();
  transformStamped.header.frame_id = "world";
  transformStamped.child_frame_id = "target";
  transformStamped.transform.translation.x = msg->position.x;
  transformStamped.transform.translation.y = msg->position.y;
  transformStamped.transform.translation.z = 0.0;
  transformStamped.transform.rotation.x = msg->orientation.x;
  transformStamped.transform.rotation.y = msg->orientation.y;
  transformStamped.transform.rotation.z = msg->orientation.z;
  transformStamped.transform.rotation.w = msg->orientation.w;

}

int main(int argc, char** argv){
  ros::init(argc, argv, "target_tf2_broadcaster");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("set_target", 1, &targetCallback);
  ros::Rate rate(20);
  transformStamped.header.stamp = ros::Time::now();
  transformStamped.header.frame_id = "world";
  transformStamped.child_frame_id = "target";
  while(ros::ok()){
    br.sendTransform(transformStamped);
    ros::spinOnce();
    rate.sleep();
  }
  
  return 0;
};