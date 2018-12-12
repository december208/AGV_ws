#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>     
#include <agv/targetAction.h>
#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h> 
#include <tf2/transform_datatypes.h>
#include <tf2/LinearMath/Transform.h>
#include <math.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

typedef actionlib::SimpleActionServer<agv::targetAction> Server;
ros::Publisher cmd_pub;
geometry_msgs::TransformStamped targetTF;
tf2::Stamped<tf2::Transform> targetTF2;

double getRemainDeg(){
  double dx, dy, dTheta;
  dx = targetTF.transform.translation.x;
  dy = targetTF.transform.translation.y;
  dTheta = atan2(dy,dx);

  return dTheta*57.2957795;
}
double getRemainDist(){
  double dx,dy;
  
  dx = targetTF.transform.translation.x;
  dy = targetTF.transform.translation.y;
  
  return sqrt(dx*dx+dy*dy)*1000.0;

}

void execute(const agv::targetGoalConstPtr& goal, Server* as)  
{ 
  static tf2_ros::Buffer tfBuffer;
  static tf2_ros::TransformListener tfListener(tfBuffer);
  ros::Rate rate(20);
  ROS_INFO("Rotating!");
  double angle=0;
  double degTolerance = 5.0;
  double distTolerance = 5.0;
  double degError=0;
  double distError=0;
  int rotateSpeedCmd=0;
  int curRotateSpeed=0;
  int speedCmd=0;
  int curSpeed=0;
  std::string moveCmd = "q";
  std::string speedStr = "0";
  std_msgs::String cmd_vel;

  while(ros::ok()){
    rate.sleep();
    if(as->isPreemptRequested()){
      cmd_vel.data = "0q";
      cmd_pub.publish(cmd_vel);
      ROS_INFO("Preempted!");
      as->setPreempted();
      return;
    }
    try{
      targetTF = tfBuffer.lookupTransform("robot","target",ros::Time(0));
    }
    catch(tf2::TransformException &ex){
      ROS_WARN("%s",ex.what());
      ros::Duration(2.0).sleep();
      continue;
    }
    degError = getRemainDeg();
    ROS_INFO("%f\n", degError);
    if(fabs(degError)>degTolerance){
      rotateSpeedCmd = (int)fabs(degError/180.0*10);
      if((rotateSpeedCmd-curRotateSpeed)>2) rotateSpeedCmd = curRotateSpeed+2;
      if(degError>0) moveCmd = 'a';
      else moveCmd = 'd';
      cmd_vel.data = std::to_string(rotateSpeedCmd)+moveCmd;
      cmd_pub.publish(cmd_vel);
      curRotateSpeed = rotateSpeedCmd;
      continue;
    }
    distError = getRemainDist();
    if(distError>distTolerance){
        if(distError>300.0) speedCmd = 9;
        if(300.0>=distError>100.0) speedCmd = 7;
        if(100.0>=distError>30.0) speedCmd = 5;
        else speedCmd = 3;
        if((speedCmd-curSpeed)>2) speedCmd = curSpeed+1;
        cmd_vel.data = std::to_string(speedCmd)+"w";
        cmd_pub.publish(cmd_vel);
        curSpeed = speedCmd;
        continue;
    }
    else{
      cmd_vel.data = "q";
      cmd_pub.publish(cmd_vel);
      break;
    }
  }
  
  as->setSucceeded();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "target_server");
  ros::NodeHandle n;
  cmd_pub = n.advertise<std_msgs::String>("cmd_vel",1);
  Server server(n, "targetAction", boost::bind(&execute, _1, &server), false);
  server.start();
  ros::spin();
  return 0;
}


