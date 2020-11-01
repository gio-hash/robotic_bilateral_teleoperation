#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Wrench.h>
ros::Publisher *pubptr;
ros::Publisher *pubptr1;
geometry_msgs::Point Um;
geometry_msgs::Wrench msg1;
geometry_msgs::Point msg2;
geometry_msgs::Wrench Vm;
geometry_msgs::Point msg;
float b=0.25;
void positionreceived(const geometry_msgs::Point& msg){
    ROS_INFO_STREAM(" position received from the leader:" << " x=" << msg.x << " y=" << msg.y << " z=" << msg.z);
    msg2.x  = msg.x;
    msg2.y = msg.y;
    msg2.z  = msg.z;
    Um.x= 1/sqrt(2*b)*(2*b*(msg.x)-sqrt(2*b)*Vm.force.x);
    Um.y=1/sqrt(2*b)*(2*b*(msg.y)-sqrt(2*b)*Vm.force.y);
    Um.z=1/sqrt(2*b)*(2*b*(msg.z)-sqrt(2*b)*Vm.force.z);
    
  
  ROS_INFO_STREAM("Um wave  sent  to the network  :" << "x=" << Um.x << " y=" << Um.y << " z=" << Um.z);
  pubptr->publish(Um);  
 
  
}

void forcereceived(const geometry_msgs::Wrench& Vs){
  ROS_INFO_STREAM("Vs delay wave received from the network:" << " Fx=" << Vs.force.x << " Fy=" << Vs.force.y << " Fz=" << Vs.force.z);
  Vm.force.x= Vs.force.x;
  Vm.force.y= Vs.force.y;
  Vm.force.z= Vs.force.z;
  msg1.force.x=b*msg2.x-sqrt(2*b)*Vs.force.x;
  msg1.force.y=b*msg2.y-sqrt(2*b)*Vs.force.y;
  msg1.force.z=b*msg2.z-sqrt(2*b)*Vs.force.z;
  ROS_INFO_STREAM("force sent to the leader :" << "Fx=" << msg1.force.x << " Fy=" << msg1.force.y << " Fz=" << msg1.force.z);
  pubptr1->publish(msg1);  
  
}
int main(int argc,char  **argv){
  ros::init(argc,argv,"wave_leader_node");
  ros::NodeHandle n;
  pubptr= new ros::Publisher(n.advertise<geometry_msgs::Point>("position_wave",1000));
  pubptr1= new ros::Publisher(n.advertise<geometry_msgs::Wrench>("force_delay",1000));
  ros::Subscriber sub = n.subscribe("position",1000,&positionreceived);
  ros::Subscriber sub1 = n.subscribe("force_wave_delay",1000,&forcereceived);
  ros::spin();
  delete pubptr;
  delete pubptr1;
}