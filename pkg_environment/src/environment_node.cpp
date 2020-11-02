#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Wrench.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Accel.h>
//global variables to set up
float k1=0.5,k2=0.5,k3=0.5;
float b1=0.1,b2 = 0.1,b3=0.1;
float mass=30;
float time_step=0.28;
geometry_msgs::Point position;

geometry_msgs::Point positiontmp;
geometry_msgs::Twist velocity;

geometry_msgs::Wrench msg1;
geometry_msgs::Accel acceleration;




ros::Publisher *pubptr;
void positionreceived(const geometry_msgs::Point& msg){
  ROS_INFO_STREAM("receiver position:" << " x=" << msg.x << " y=" << msg.y << " z=" << msg.z);
  position.x=msg.x;
  position.y=msg.y;
  position.z=msg.z;
  position.x+=positiontmp.x;
  position.y+=positiontmp.y;
  position.z+=positiontmp.z;
  msg1.force.x = -k1*(position.x -0.0) -b1*velocity.linear.x;
  msg1.force.y = -k2*(position.y-0.0) -b2*velocity.linear.y+ mass*9.81;
  msg1.force.z = -k3*(position.z-0.0) -b3*velocity.linear.z;
  ROS_INFO_STREAM("sender force :" << "x=" << msg1.force.x << " y=" << msg1.force.y << " z=" << msg1.force.z);
  pubptr->publish(msg1);  
  acceleration.linear.x =msg1.force.x/mass;
  acceleration.linear.y = msg1.force.y/mass;
  acceleration.linear.z= msg1.force.z/mass;
  velocity.linear.x +=acceleration.linear.x*time_step;
  velocity.linear.y +=acceleration.linear.y*time_step;
  velocity.linear.z+=acceleration.linear.z*time_step;
  positiontmp.x += velocity.linear.x*time_step;
  positiontmp.y += velocity.linear.y*time_step;
  positiontmp.z+= velocity.linear.z*time_step;
 /* k1 = msg1.force.x/position.x;
  k2 = msg1.force.y/position.y;
  k3 = msg1.force.z/position.z;
  b1 = msg1.force.x/velocity.linear.x;
  b2 = msg1.force.y/velocity.linear.y;
  b3 = msg1.force.z/velocity.linear.z;*/
}
int main(int argc,char  **argv){
  ros::init(argc,argv,"environment_node");
  ros::NodeHandle n;
  pubptr= new ros::Publisher(n.advertise<geometry_msgs::Wrench>("force",1000));
  ros::Subscriber sub = n.subscribe("position_delay1",1000,&positionreceived);
  ros::spin();
  delete pubptr;
}
