#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Wrench.h>
ros::Publisher *pubptr;
ros::Publisher *pubptr1;
void positionreceived(const geometry_msgs::Point& msg){
  ROS_INFO_STREAM("position sent from the wave_follower" << " x=" << msg.x << " y=" << msg.y << " z=" << msg.z);

  
  ROS_INFO_STREAM("position sent to the environment  :" << "x=" << msg.x << " y=" << msg.y << " z=" << msg.z);
  pubptr->publish(msg);  
 
  
}

void forcereceived(const geometry_msgs::Wrench& msg1){
  ROS_INFO_STREAM("force received from the environment:" << " Fx=" << msg1.force.x << " Fy=" << msg1.force.y << " Fz=" << msg1.force.z);
  
  ROS_INFO_STREAM("force sent to the wave_follower :" << "Fx=" << msg1.force.x << " Fy=" << msg1.force.y << " Fz=" << msg1.force.z);
  pubptr1->publish(msg1);  
  
}
int main(int argc,char  **argv){
  ros::init(argc,argv,"follower_node");
  ros::NodeHandle n;
  pubptr= new ros::Publisher(n.advertise<geometry_msgs::Point>("position_delay1",1000));
  pubptr1= new ros::Publisher(n.advertise<geometry_msgs::Wrench>("force1",1000));
  ros::Subscriber sub = n.subscribe("position_delay",1000,&positionreceived);
  ros::Subscriber sub1 = n.subscribe("force",1000,&forcereceived);
  ros::spin();
  delete pubptr;
  delete pubptr1;
}