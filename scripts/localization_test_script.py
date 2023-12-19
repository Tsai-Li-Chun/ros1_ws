#!/usr/bin/env python  
import roslib
import rospy
import actionlib
import math
import time
import tf
import tf2_ros
import tf2_geometry_msgs
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry
from move_base_msgs.msg import *
from gazebo_msgs.srv import GetModelState

class script_way_point(object):
    def __init__(self):
        self.move_base_result = 99
        self.path_id = "p2"
        self.sub_move_base_result = rospy.Subscriber("/move_base/result", MoveBaseActionResult, self.get_move_base_result)
        
        
    def get_move_base_result(self, msg):
        self.move_base_result =  msg.status.status
        #self.switch_way_point(self.move_base_result)
        #rospy.loginfo(self.move_base_result)

    def switch_way_point(self, result):
        if result == 3 and self.path_id == "p1":
            self.path_id = "p2"
        elif result == 3 and self.path_id == "p2":
            self.path_id = "p1"
        #elif result == 3 and self.path_id == "p3":
        #    self.path_id = "p4"
        #elif result == 3 and self.path_id == "p4":
        #    self.path_id = "p1"
        
            

if __name__ == '__main__':
    rospy.init_node('localization_test_script', anonymous = True)
    AMR_nav = script_way_point()
    rate = rospy.Rate(10)
    rate.sleep
    
    client = actionlib.SimpleActionClient('/move_base', MoveBaseAction)
    rospy.loginfo("Waiting for move base server")
    client.wait_for_server()
    goal = MoveBaseGoal()
        
    rospy.loginfo("localization_test_script is ready")
    while not rospy.is_shutdown():
    
        if AMR_nav.path_id == "p1":

            goal.target_pose.header.frame_id = 'map'
            goal.target_pose.pose.position.x = 4.0
            goal.target_pose.pose.position.y = 0.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = 0.99999
            goal.target_pose.pose.orientation.w = 0.003899
            client.send_goal(goal)
            client.wait_for_result()
             
            goal.target_pose.pose.position.x = 0.0
            goal.target_pose.pose.position.y = 0.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = 0.99999
            goal.target_pose.pose.orientation.w = 0.0038991
            client.send_goal(goal)
            client.wait_for_result()

            rospy.loginfo("==p1 arrived!==")
            AMR_nav.switch_way_point(AMR_nav.move_base_result)
            time.sleep(30)
        
            
        elif AMR_nav.path_id == "p2":

            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 4.0
            goal.target_pose.pose.position.y = 0.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = -0.70788
            goal.target_pose.pose.orientation.w = 0.70633
            client.send_goal(goal)
            client.wait_for_result()
            
            goal.target_pose.pose.position.x = 4.0
            goal.target_pose.pose.position.y = -6.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = -0.71273
            goal.target_pose.pose.orientation.w = 0.70143
            client.send_goal(goal)
            client.wait_for_result()

            rospy.loginfo("==p2 arrived!==")
            AMR_nav.switch_way_point(AMR_nav.move_base_result)
            time.sleep(30)

        
        #elif AMR_nav.path_id == "p3":
        #    goal.target_pose.header.frame_id = 'map' 
        #    goal.target_pose.pose.position.x = 7.5
        #    goal.target_pose.pose.position.y = 2.5
        #    goal.target_pose.pose.orientation.x = 0.0
        #    goal.target_pose.pose.orientation.y = 0.0
        #    goal.target_pose.pose.orientation.z = -0.8946
        #    goal.target_pose.pose.orientation.w = 0.4468
        #    client.send_goal(goal)
        #    client.wait_for_result()
        #    rospy.loginfo("==p3 arrived!==")
        #    time.sleep(30)


        #elif AMR_nav.path_id == "p4":
        #    goal.target_pose.header.frame_id = 'map' 
        #    goal.target_pose.pose.position.x = 4.0
        #    goal.target_pose.pose.position.y = -4.0
        #    goal.target_pose.pose.orientation.x = 0.0
        #    goal.target_pose.pose.orientation.y = 0.0
        #    goal.target_pose.pose.orientation.z = 0.9301
        #    goal.target_pose.pose.orientation.w = 0.3671
        #    client.send_goal(goal)
        #    client.wait_for_result()
        #    rospy.loginfo("==p4 arrived!==")
        #    time.sleep(30)


    rospy.spin()
