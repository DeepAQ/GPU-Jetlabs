#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int16.h"

#define LINEAR_SPEED 0.7
#define ANGULAR_SPEED 0.5
#define OBJECT_DIST 70

int state = 0;
geometry_msgs::Twist vel_msg;

ros::Subscriber sonar_sub;
ros::Publisher vel_pub;

void sonar_cb(const std_msgs::Int16::ConstPtr& msg)
{
  ROS_INFO("Sonar: %d cm, State: %d", msg->data, state);
  if (msg->data < OBJECT_DIST && msg->data > 0) {
    ROS_INFO("Reverse");
    state = 2;
    vel_msg.linear.x = -LINEAR_SPEED;
    vel_msg.angular.z = 0;
    vel_pub.publish(vel_msg);
  } else if ((msg->data > OBJECT_DIST || msg->data == 0)) {
    ROS_INFO("Forward");
    state = 1;
    vel_msg.linear.x = LINEAR_SPEED;
    vel_msg.angular.z = 0;
    vel_pub.publish(vel_msg);
  }
}

int main(int argc, char **argv)
{
  ROS_INFO("Init");
  ros::init(argc, argv, "sense_and_avoid");

  ros::NodeHandle nh;
  sonar_sub = nh.subscribe<std_msgs::Int16>("/arduino/sonar_1", 10, sonar_cb);
  vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  ROS_INFO("Spin");
  ros::spin();
}
