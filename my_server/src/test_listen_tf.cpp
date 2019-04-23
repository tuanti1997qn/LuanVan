#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

void Robot1_Callback(const geometry_msgs::Twist::ConstPtr& msg);
void Robot2_Callback(const geometry_msgs::Twist::ConstPtr& msg);

ros::Publisher r1_pub, r2_pub;
int main(int argc, char **argv)
{
    ros::init(argc , argv , "my_server_node");
    
    // init cho robot1
    ros::NodeHandle r1_nht, r1_nhr;
    r1_pub = r1_nht.advertise<geometry_msgs::Twist>("/robot1/r_cmd_vel", 1000);
    ros::Subscriber r1_sub = r1_nhr.subscribe("/robot1/cmd_vel", 1000, Robot1_Callback);

    // init cho robot2
    ros::NodeHandle r2_nht, r2_nhr;
    r2_pub = r2_nht.advertise<geometry_msgs::Twist>("/robot2/r_cmd_vel", 1000);
    ros::Subscriber r2_sub = r2_nhr.subscribe("/robot2/cmd_vel", 1000, Robot2_Callback);    

    ros::Rate loop_rate(10);

    // while (ros::ok())
    // {
    //     ros::spinOnce();
    // }
    ros::spin();
    return 0;
}


void Robot1_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
    r1_pub.publish(msg);
}

void Robot2_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
    r2_pub.publish(msg);
}


