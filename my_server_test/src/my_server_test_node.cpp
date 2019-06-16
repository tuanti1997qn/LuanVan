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
int my_sw_flag = 0;
int my_luixe_dir = 0; // 0: lui thang, 1:  trai ,2 : phai, 3 tien len
double my_inf = std::numeric_limits<double>::infinity();
int main(int argc, char **argv)
{
  ros::init(argc, argv, "my_tf_listener");

  tf::TransformListener listener;
  // phan init cho transform listener
  ros::NodeHandle node;

  // phan init cho nhan cmd_vel

  // init cho robot1
  ros::NodeHandle r1_nht, r1_nhr, r1_nhscan;
  r1_pub = r1_nht.advertise<geometry_msgs::Twist>("/robot1/cmd_vel", 1);
  ros::Subscriber r1_sub = r1_nhr.subscribe("/robot1/c_cmd_vel", 1, Robot1_Callback);
  ros::Subscriber r1_sub_Scan = r1_nhscan.subscribe("/robot1/scan", 1, Robot1_Scan_Callback);

  // init cho robot2
  ros::NodeHandle r2_nht, r2_nhr;
  r2_pub = r2_nht.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1);
  ros::Subscriber r2_sub = r2_nhr.subscribe("/robot2/c_cmd_vel", 1, Robot2_Callback);

  ros::Rate rate(5);
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
      // std::cout << my_scan.ranges.at(6) << std::endl;
    }

    // std::cout << " chi so x " << transform.getOrigin().x() << std::endl;
    // std::cout << " chi so y " << transform.getOrigin().y() << std::endl;
    // std::cout << " chi so z " << transform.getOrigin().z() << std::endl;

    // test zone
    // robot 1 move
    float x = transform.getOrigin().x();
    float y = transform.getOrigin().y();
    float m_dist = sqrt(x * x + y * y);
    // std::cout <<x << std::endl;
    // std::cout <<y << std::endl;
    // std::cout <<m_dist << std::endl;
    geometry_msgs::Twist my_vel;

    if (m_dist == 0)
    {
    }                      // >0.5
    else if (m_dist > 0.5) // >0.5
    {
      //my_vel.linear.x = 0;
      //r1_pub.publish(my_vel);
      if (my_sw_flag == 1)
      {
        my_sw_flag = 0;
        my_vel.linear.x = 0;
        my_vel.angular.z = 0;
        r1_pub.publish(my_vel);
      }
    }

    else if ((m_dist < 0.5) && (m_dist > 0.35)) // >0.4 , <0.5
    {
      /* code */
      // std::cout <<"khoang cach giua 2 xe:"<< m_dist<< std::endl;
      // std::cout << "lui tranh xe"<< std::endl;
      // std::cout << ">0.4 <0.5"<<std::endl;
      static int cnt_temp_yeah_ahihi = 0;
      if (cnt_temp_yeah_ahihi > 10000)
      {
        cnt_temp_yeah_ahihi = 0;
        my_vel.linear.x = 0;
        my_vel.angular.z = 0;
        r1_pub.publish(my_vel);
      }
    }
    else // <0.6
    {
      //       float x = transform.getOrigin().x();
      // float y = transform.getOrigin().y();
      // std::cout << "<0.4"<<std::endl;
      // std::cout << "khoang cach giua 2 xe:" << m_dist << std::endl;
      // std::cout << "lui tranh xe" << std::endl;

      my_sw_flag = 1;
      std::cout << my_luixe_dir << std::endl;
      static int cnt_temp_yeah = 0;
      std::cout << cnt_temp_yeah++ << std::endl;
      if (cnt_temp_yeah > 10000)
      {
        cnt_temp_yeah = 0;
        if (my_luixe_dir == 0)
        {
          /* code */
          my_vel.linear.x = -0.3;
          my_vel.angular.z = 0;
          r1_pub.publish(my_vel);
        }
        else if (my_luixe_dir == 1)
        {
          my_vel.linear.x = 0;
          my_vel.angular.z = 0.3;
          r1_pub.publish(my_vel);
        }
        else if (my_luixe_dir == 2)
        {
          my_vel.linear.x = 0;
          my_vel.angular.z = -0.3;
          r1_pub.publish(my_vel);
        }
        else if (my_luixe_dir == 3)
        {
          my_vel.linear.x = 0.3;
          my_vel.angular.z = 0;
          r1_pub.publish(my_vel);
        }
      }
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
  float x = transform.getOrigin().x();
  float y = transform.getOrigin().y();
  float m_dist = sqrt(x * x + y * y);
  if (m_dist > 0.5) // >0.8
  {
    r1_pub.publish(msg);
    std::cout << "robot 1 move" << std::endl;
  }
  // else if ((m_dist < 0.5) && (m_dist > 0.4))
  // {
  //     my_vel.linear.x = 0;
  //     my_vel.angular.z = 0;
  //     r1_pub.publish(my_vel);
  // }
  // else // <0.6
  // {
  //   if (my_luixe_dir == 0)
  //   {
  //     /* code */
  //     my_vel.linear.x = -0.3;
  //     my_vel.angular.z = 0;
  //     r1_pub.publish(my_vel);
  //   }
  //   else if (my_luixe_dir == 1)
  //   {
  //     my_vel.linear.x = 0;
  //     my_vel.angular.z = 0.3;
  //     r1_pub.publish(my_vel);
  //   }
  //   else if (my_luixe_dir == 2)
  //   {
  //     my_vel.linear.x = 0;
  //     my_vel.angular.z = -0.3;
  //     r1_pub.publish(my_vel);
  //   }
  //   else if (my_luixe_dir == 3)
  //   {
  //     my_vel.linear.x = 0.3;
  //     my_vel.angular.z = 0;
  //     r1_pub.publish(my_vel);
  //   }
  // }
}

void Robot2_Callback(const geometry_msgs::Twist::ConstPtr &msg)
{
  // tf::TransformListener listener;
  // listener.lookupTransform("/robot1/base_footprint", "/robot2/base_footprint", ros::Time(0), transform);
  //if (is_available(transform, 0.5))
  //{
  geometry_msgs::Twist my_vel;
  float x = transform.getOrigin().x();
  float y = transform.getOrigin().y();
  float m_dist = sqrt(x * x + y * y);
  if (m_dist > 0.5) // >0.8
  {
    r2_pub.publish(msg);
    //std::cout << "robot 2 move" << std::endl;
  }
  else
  {
    my_vel.linear.x = -0.2;
    my_vel.angular.z = 0;
    r2_pub.publish(my_vel);
  }
  //}
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
  // std::cout <<"khoang cach giua 2 xe:"<< dist << std::endl;
  min_dist *= min_dist;

  return (dist > min_dist);
}

void Robot1_Scan_Callback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
  my_scan.angle_increment = msg->angle_increment;
  my_scan.ranges = msg->ranges;
  // std::cout << msg->angle_increment *360 /(3.14159*2) << std::endl;
  // std::cout << my_scan.ranges[360 - 90] << std::endl;
  // std::cout << my_inf << std::endl;

  int cnt_temp;
  int flag = 0;
  my_luixe_dir = 0;
  for (cnt_temp = 150; cnt_temp < 170; cnt_temp++)
  {
    flag = flag || (my_scan.ranges[cnt_temp] < 0.25) || (my_scan.ranges[cnt_temp] == my_inf);
  }
  if (flag)
  {
    my_luixe_dir = 1; // vat can ben trai, ne trai
  }
  //////////////////////
  flag = 0;
  for (cnt_temp = 190; cnt_temp < 210; cnt_temp++)
  {
    flag = flag || (my_scan.ranges[cnt_temp] < 0.25) || (my_scan.ranges[cnt_temp] == my_inf);
  }
  if (flag)
  {
    if (my_luixe_dir)
    {
      my_luixe_dir = 3; // vat can hai ben, tien len
    }
    else
    {
      my_luixe_dir = 2; // vat can ben phai, re phai
    }
  }
  //////////////////////////////
  flag = 0;
  for (cnt_temp = 170; cnt_temp < 190; cnt_temp++)
  {
    flag = flag || (my_scan.ranges[cnt_temp] < 0.25) || (my_scan.ranges[cnt_temp] == my_inf);
  }
  if (flag)
  {
    if (my_luixe_dir == 0)
    {
      my_luixe_dir = 1; // neu ko can hai ben can o giua mac dinh re trai
    }
    if (my_luixe_dir == 3)
    {
      my_luixe_dir = 1; // neu can hai ben can luon giua thi mac dinh quay san trai
    }
  }
}