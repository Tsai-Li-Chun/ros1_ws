#!/usr/bin/env python  
import roslib
import rospy
import actionlib
import math
import tf
import tf2_ros
import tf2_geometry_msgs
from geometry_msgs.msg import PoseStamped, Twist
from nav_msgs.msg import Odometry

from move_base_msgs.msg import *

class fix_cmd_vel(object):
    def __init__(self):
        self.goal_pose = None
        self.odom_pose = None
        self.carto_pose = None
        self.move_base_result = 99
        self.cmd_vel = Twist
        
        self.odom_sub = rospy.Subscriber("/odom_converted", Odometry, self.odom_callback, queue_size=1)
        self.sub_move_base_result = rospy.Subscriber("/move_base/result", MoveBaseActionResult, self.get_move_base_result)
        self.sub_move_base_goal = rospy.Subscriber("/move_base_simple/goal", PoseStamped, self.get_move_base_goal)
        self.sub_raw_cmd_vel = rospy.Subscriber("/raw_cmd_vel", Twist, self.get_raw_cmd_vel)
        
        self.pub_cmd_vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        
    def odom_callback(self, msg):
        self.odom_pose = msg.pose
        
    def get_move_base_result(self, msg):
        self.move_base_result =  msg.status.status
        
    def get_move_base_goal(self, msg):
        self.goal_pose = msg.pose
        
    def get_raw_cmd_vel(self, msg):
        self.cmd_vel = msg
        
    def fix_cmd_vel(self):
        dist = math.sqrt((self.carto_pose.pose.position.x - self.goal_pose.position.x)**2 + (self.carto_pose.pose.position.y - self.goal_pose.position.y)**2)
        rospy.loginfo(dist)
        if dist >= 1:
            self.pub_cmd_vel.publish(self.cmd_vel)
        else:
            #k = 1/(2.71828**(1-dist))
            self.cmd_vel.linear.x = self.cmd_vel.linear.x * 0.75
            self.cmd_vel.angular.z = self.cmd_vel.angular.z * 0.5
            self.pub_cmd_vel.publish(self.cmd_vel)

if __name__ == '__main__':
    rospy.init_node('cmd_vel_fix', anonymous = True)
    tf_buffer = tf2_ros.Buffer()  # tf buffer length
    tf_listener = tf2_ros.TransformListener(tf_buffer)
    velocity = fix_cmd_vel()
    rate = rospy.Rate(10)
    rate.sleep
    
    client = actionlib.SimpleActionClient('/move_base', MoveBaseAction)
    rospy.loginfo("Waiting for move base server")
    client.wait_for_server()
    goal = MoveBaseGoal()
        
    rospy.loginfo("cmd_vel_fix node is ready")
    while not rospy.is_shutdown():
    
        if velocity.odom_pose is not None and velocity.goal_pose is not None:
        
            try:
                transform = tf_buffer.lookup_transform("map", "odom_combined", rospy.Time(0), rospy.Duration(3.0))
                velocity.carto_pose = tf2_geometry_msgs.do_transform_pose(velocity.odom_pose, transform)
                velocity.fix_cmd_vel()
  
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException) as error:
                rospy.loginfo(error)
                continue
            

    rospy.spin()
