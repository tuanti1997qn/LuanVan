#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

using namespace std;
double r1_goal_x;
double r1_goal_y;
double r1_goal_theta;
double r2_goal_x;
double r2_goal_y;
double r2_goal_theta;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_goals");
    ros::NodeHandle nh;
    // Define the goal
    cout << "dia chi robot 1 : ";
    cin >> r1_goal_x >> r1_goal_y >> r1_goal_theta;
    cout << endl;
    cout << "dia chi robot 2 : ";
    cin >> r2_goal_x >> r2_goal_y >> r2_goal_theta;
    cout <<endl;

    // create the action client
    MoveBaseClient ac1("/robot1/move_base", true);
    MoveBaseClient ac2("/robot2/move_base", true);
    // Wait for the action server to become available
    ROS_INFO("Waiting for the move_base action server");
    ac1.waitForServer(ros::Duration(5));
    ROS_INFO("Connected to move base server");

    // Send a goal to move_base
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = r1_goal_x;
    goal.target_pose.pose.position.y = r1_goal_y;

    // Convert the Euler angle to quaternion
    double radians = r1_goal_theta * (M_PI / 180);
    tf::Quaternion quaternion;
    quaternion = tf::createQuaternionFromYaw(radians);
    geometry_msgs::Quaternion qMsg;
    tf::quaternionTFToMsg(quaternion, qMsg);
    goal.target_pose.pose.orientation = qMsg;
    ROS_INFO("Sending goal to %s: x = %f, y = %f, theta = %f", "robot1", r1_goal_x, r1_goal_y, r1_goal_theta);
    ac1.sendGoal(goal);

////////////////////////////////////////
    ROS_INFO("Waiting for the move_base action server");
    ac2.waitForServer(ros::Duration(5));
    ROS_INFO("Connected to move base server");

    // Send a goal to move_base
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = r2_goal_x;
    goal.target_pose.pose.position.y = r2_goal_y;

    // Convert the Euler angle to quaternion
    radians = r2_goal_theta * (M_PI / 180);
    quaternion = tf::createQuaternionFromYaw(radians);
    tf::quaternionTFToMsg(quaternion, qMsg);
    goal.target_pose.pose.orientation = qMsg;
    ROS_INFO("Sending goal to %s: x = %f, y = %f, theta = %f", "robot2", r2_goal_x, r2_goal_y, r2_goal_theta);
    ac2.sendGoal(goal);


    return 0;
}