
#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Wrench.h>
ros::Publisher *pubptr;
ros::Publisher *pubptr1;
geometry_msgs::Point msg;
geometry_msgs::Point Um;
geometry_msgs::Point Us;
geometry_msgs::Wrench msg1;
geometry_msgs::Wrench msg2;
geometry_msgs::Wrench Vs;

float b=0.25;
void positionreceived(const geometry_msgs::Point& Um){
    ROS_INFO_STREAM(" Um delay wave received from the network:" << " x=" << Um.x << " y=" << Um.y << " z=" << Um.z);
    Us.x = Um.x;
    Us.y = Um.y;
    Us.z = Um.z;
    msg.x=1/b*( sqrt(2*b)*Um.x-msg2.force.x);
    msg.y=1/b*( sqrt(2*b)*Um.y-msg2.force.y);
    msg.z=1/b*( sqrt(2*b)*Um.z-msg2.force.z);
  
  ROS_INFO_STREAM("position sent to the follower  :" << "x=" << msg.x << " y=" << msg.y << " z=" << msg.z);
  pubptr->publish(msg);  
 
  
}

void forcereceived(const geometry_msgs::Wrench& msg1){
  ROS_INFO_STREAM("force received from the follower:" << " Fx=" << msg1.force.x << " Fy=" << msg1.force.y << " Fz=" << msg1.force.z);
  msg2.force.x=msg1.force.x;
  msg2.force.y=msg1.force.y;
  msg2.force.z=msg1.force.z;
  Vs.force.x=1/sqrt(2*b)*(1/b*Us.x-msg1.force.x);
  Vs.force.y=1/sqrt(2*b)*(1/b*Us.y-msg1.force.y);
  Vs.force.z=1/sqrt(2*b)*(1/b*Us.z-msg1.force.z);
  ROS_INFO_STREAM("Vs wave sent to the network :" << "Fx=" << Vs.force.x << " Fy=" << Vs.force.y << " Fz=" << Vs.force.z);
  pubptr1->publish(Vs);  
  
}
int main(int argc,char  **argv){
  ros::init(argc,argv,"wave_follower_node");
  ros::NodeHandle n;
  pubptr= new ros::Publisher(n.advertise<geometry_msgs::Point>("position_delay",1000));
  pubptr1= new ros::Publisher(n.advertise<geometry_msgs::Wrench>("force_wave",1000));
  ros::Subscriber sub = n.subscribe("position_wave_delay",1000,&positionreceived);
  ros::Subscriber sub1 = n.subscribe("force1",1000,&forcereceived);
  ros::spin();
  delete pubptr;
  delete pubptr1;
}