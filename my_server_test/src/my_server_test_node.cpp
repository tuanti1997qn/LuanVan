#include <ros/ros.h>
#include "math.h"
#include <tf/transform_listener.h>
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

void Robot1_Callback(const geometry_msgs::Twist::ConstPtr &msg);
void Robot2_Callback(const geometry_msgs::Twist::ConstPtr &msg);
int is_available(tf::StampedTransform my_transform, float min_dist);

void Robot1_Scan_Callback(const sensor_msgs::LaserScan::ConstPtr &msg);

ros::Publisher r1_pub, r2_pub;
tf::StampedTransform transform, transform_x1_g, transform_x2_g;

sensor_msgs::LaserScan my_scan;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "my_tf_listener");

  tf::TransformListener listener;
  // phan init cho transform listener
  ros::NodeHandle node;

  // phan init cho nhan cmd_vel

  // init cho robot1
  ros::NodeHandle r1_nht, r1_nhr, r1_nhscan;
  r1_pub = r1_nht.advertise<geometry_msgs::Twist>("/robot1/cmd_vel", 1000);
  ros::Subscriber r1_sub = r1_nhr.subscribe("/robot1/c_cmd_vel", 1000, Robot1_Callback);
  ros::Subscriber r1_sub_Scan = r1_nhscan.subscribe("/robot1/scan", 1000, Robot1_Scan_Callback);

  // init cho robot2
  ros::NodeHandle r2_nht, r2_nhr;
  r2_pub = r2_nht.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);
  ros::Subscriber r2_sub = r2_nhr.subscribe("/robot2/c_cmd_vel", 1000, Robot2_Callback);

  ros::Rate rate(10.0);
  while (node.ok())
  {
    try
    {
      listener.lookupTransform("/robot1/base_footprint", "/robot2/base_footprint", ros::Time(0), transform);
      listener.lookupTransform("/robot1/base_footprint", "/map", ros::Time(0), transform_x1_g);
      listener.lookupTransform("/robot2/base_footprint", "/map", ros::Time(0), transform_x2_g);
    }
    catch (tf::TransformException ex)
    {
    }
    if (my_scan.ranges.empty() == 0)
    {
      std::cout << my_scan.ranges.at(6) << std::endl;
    }

    // std::cout << " chi so x " << transform.getOrigin().x() << std::endl;
    // std::cout << " chi so y " << transform.getOrigin().y() << std::endl;
    // std::cout << " chi so z " << transform.getOrigin().z() << std::endl;

    // test zone
    // robot 1 move
    geometry_msgs::Twist my_vel;
    if (is_available(transform, 0.8)) // >0.8
    {
    }
    else if (is_available(transform, 0.6)) // >0.6 , <0.8
    {
      /* code */
      my_vel.linear.x = 0;
      r1_pub.publish(my_vel);
    }
    else // <0.6
    {

      my_vel.linear.x = -0.2;
      r1_pub.publish(my_vel);
    }




    ros::spinOnce();
    // rate.sleep();
  }
  return 0;
};

void Robot1_Callback(const geometry_msgs::Twist::ConstPtr &msg)
{
  // tf::TransformListener listener;
  // listener.lookupTransform("/robot1/base_footprint", "/robot2/base_footprint", ros::Time(0), transform);
  geometry_msgs::Twist my_vel;
  if (is_available(transform, 0.8)) // >0.8
  {
    r1_pub.publish(msg);
    std::cout << "robot 1 move" << std::endl;
  }
  else if (is_available(transform, 0.6)) // >0.6 , <0.8
  {
    /* code */
    my_vel.linear.x = 0;
    r1_pub.publish(my_vel);
  }
  else // <0.6
  {

    my_vel.linear.x = -0.2;
    r1_pub.publish(my_vel);
  }
}

void Robot2_Callback(const geometry_msgs::Twist::ConstPtr &msg)
{
  // tf::TransformListener listener;
  // listener.lookupTransform("/robot1/base_footprint", "/robot2/base_footprint", ros::Time(0), transform);
  if (is_available(transform, 0.5))
  {
    r2_pub.publish(msg);
    std::cout << "robot 2 move" << std::endl;
  }
}

// int is_available(tf::StampedTransform p1, tf::StampedTransform p2, float min_dist)
// {
//     float sub_1, sub_2, dist;

//     sub_1 = p1.getOrigin().x() - p2.getOrigin().x();
//     sub_2 = p1.getOrigin().y() - p2.getOrigin().y();

//     sub_1 *= sub_1;
//     sub_2 *= sub_2;

//     dist = sub_1 + sub_2;
//     min_dist *= min_dist;

//     return (dist > min_dist);
// }
int is_available(tf::StampedTransform my_transform, float min_dist)
{
  float x, y, dist;
  x = my_transform.getOrigin().x();
  y = my_transform.getOrigin().y();
  dist = x * x + y * y;
  std::cout << dist << std::endl;
  min_dist *= min_dist;

  return (dist > min_dist);
}

void Robot1_Scan_Callback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
  my_scan.angle_increment = msg->angle_increment;
  my_scan.ranges = msg->ranges;
  // std::cout << msg->angle_increment << std::endl;
}