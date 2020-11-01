#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Wrench.h>
#include <time.h>
#include <stdlib.h>
ros::Publisher *pubptr1;
ros::Publisher *pubptr;

void forcereceived(const geometry_msgs::Wrench& Vs){
  ROS_INFO_STREAM("Vs wave receveid from the wave_follower:" << " Fx=" << Vs.force.x << " Fy=" << Vs.force.y << " Fz=" << Vs.force.z);
  
  
  
  ros::Time begintime1=ros::Time::now();
  ros::Duration messagetime1=ros::Duration(1);
  ros::Time endtime1= begintime1+messagetime1;
  while(ros::Time::now()<endtime1);
  
  ROS_INFO_STREAM("Vs delay wave sent to the wave_leader  :" << "Fx=" << Vs.force.x << " Fy=" << Vs.force.y << " Fz=" << Vs.force.z);
  pubptr1->publish(Vs);  
 
  
}

void positionreceived(const geometry_msgs::Point& Um){
  ROS_INFO_STREAM("Um wave receveid from the wave_leader:" << " x=" << Um.x << " y=" << Um.y << " z=" << Um.z);
  
  
  //do something!!
  ros::Time begintime=ros::Time::now();
  ros::Duration messagetime=ros::Duration(1);
  ros::Time endtime= begintime+messagetime;
  while(ros::Time::now()<endtime);
  
  ROS_INFO_STREAM("Um delay wave sent to the wave_follower  :" << "x=" << Um.x << " y=" << Um.y << " z=" << Um.z);
  pubptr->publish(Um);  
  
  
}
int main(int argc,char  **argv){
  ros::init(argc,argv,"network_node");
  
  
  ros::NodeHandle n;
  pubptr= new ros::Publisher(n.advertise<geometry_msgs::Point>("position_wave_delay",1000));
  pubptr1= new ros::Publisher(n.advertise<geometry_msgs::Wrench>("force_wave_delay",1000));
  ros::Subscriber sub = n.subscribe("position_wave",1000,&positionreceived);
  ros::Subscriber sub1 = n.subscribe("force_wave",1000,&forcereceived);

  ros::spin();
  delete pubptr1;
  delete pubptr;
}
