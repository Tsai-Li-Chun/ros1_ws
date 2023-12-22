#!/usr/bin/env python  
import roslib
import rospy
import actionlib
import math
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

class localization_error_calculation(object):
    def __init__(self):
        self.carto_pose = None
        self.odom_pose = None
        self.model_pose = None
        self.carto_poses = []
        self.model_poses = []
        self.errs = []
        self.p1_err = []
        self.p1_err_yaw = []
        self.p2_err = []
        self.p2_err_yaw = []
        self.p3_err = []
        self.p3_err_yaw = []
        self.p4_err = []
        self.p4_err_yaw = []
        self.move_base_result = 99
        self.path_id = "p2"
        self.fig_num = 1
        self.model_name = '/home/ros1/ros1_ws/src/delta_amr2st_gazebo/urdf/AMR_tag/TY2_SW.urdf.xacro'
        self.odom_sub = rospy.Subscriber("/odom", Odometry, self.odom_callback, queue_size=1)
        self.sub_move_base_result = rospy.Subscriber("/move_base/result", MoveBaseActionResult, self.get_move_base_result)
        
    def odom_callback(self, msg):
        self.odom_pose = msg.pose
        
    def get_move_base_result(self, msg):
        self.move_base_result =  msg.status.status
        
    def call_model_pose(self):
        rospy.wait_for_service('/gazebo/get_model_state')
        try:
            model_pose_srv_call = rospy.ServiceProxy('/gazebo/get_model_state', GetModelState)
            self.model_pose = model_pose_srv_call(self.model_name, '')

        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)
    
    
    def quaternion_to_euler(self, q):
        sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z)
        cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y)
        roll = math.atan2(sinr_cosp, cosr_cosp)

        sinp = 2.0 * (q.w * q.y - q.z * q.x)
        if abs(sinp) >= 1:
            pitch = math.copysign(math.pi / 2, sinp)  # use 90 degrees if out of range
        else:
            pitch = math.asin(sinp)

        siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
        cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
        yaw = math.atan2(siny_cosp, cosy_cosp)
    
        return yaw
        
    def point_err_calculation(self, p):
        #calculate distance
        dist = math.sqrt((self.carto_pose.pose.position.x - self.model_pose.pose.position.x)**2 + (self.carto_pose.pose.position.y - self.model_pose.pose.position.y)**2)
        dist = round(dist,2)
        
        #caluculate angular error
        odom_yaw = self.quaternion_to_euler(self.carto_pose.pose.orientation)
        model_yaw = self.quaternion_to_euler(self.model_pose.pose.orientation)
        yaw_err = abs(model_yaw - odom_yaw)
        yaw_err = round(yaw_err,2)
        
        rospy.loginfo("in point_err_calculation")
        if p == "p1":
            self.p1_err.append(dist)
            self.p1_err_yaw.append(yaw_err)
        elif p == "p2":
            self.p2_err.append(dist)
            self.p2_err_yaw.append(yaw_err)
        elif p == "p3":
            self.p3_err.append(dist)
            self.p3_err_yaw.append(yaw_err)
        elif p == "p4":
            self.p4_err.append(dist)
            self.p4_err_yaw.append(yaw_err)
            

            

if __name__ == '__main__':
    rospy.init_node('localization_test', anonymous = True)
    tf_buffer = tf2_ros.Buffer()  # tf buffer length
    tf_listener = tf2_ros.TransformListener(tf_buffer)
    calculation = localization_error_calculation()
    rate = rospy.Rate(10)
    rate.sleep
    
    client = actionlib.SimpleActionClient('/move_base', MoveBaseAction)
    rospy.loginfo("Waiting for move base server")
    client.wait_for_server()
    goal = MoveBaseGoal()
        
    rospy.loginfo("localization_test node is ready")
    while not rospy.is_shutdown():
        
                
        if calculation.move_base_result == 1:
            rospy.loginfo("next point")
            calculation.move_base_result = 99
            
            if calculation.path_id == "p1":
                calculation.path_id = "p2"
            elif calculation.path_id == "p2":
                calculation.path_id = "p3"
            elif calculation.path_id == "p3":
                calculation.path_id = "p4"
            elif calculation.path_id == "p4":
                calculation.path_id = "p1"
            
        elif calculation.move_base_result == 99 and calculation.path_id == "p1":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 0.0
            goal.target_pose.pose.position.y = 0.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = 0.0
            goal.target_pose.pose.orientation.w = 1.0
            client.send_goal(goal)
            client.wait_for_result()
            rospy.loginfo("==p1 arrived!==")


        
        elif calculation.move_base_result == 99 and calculation.path_id == "p2":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 5.0
            goal.target_pose.pose.position.y = 5.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = -0.3609
            goal.target_pose.pose.orientation.w = 0.9325
            client.send_goal(goal)
            client.wait_for_result()
            rospy.loginfo("==p2 arrived!==")

            
        elif calculation.move_base_result == 99 and calculation.path_id == "p3":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 7.5
            goal.target_pose.pose.position.y = 2.5
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = -0.8946
            goal.target_pose.pose.orientation.w = 0.4468
            client.send_goal(goal)
            client.wait_for_result()
            rospy.loginfo("==p3 arrived!==")

            
        elif calculation.move_base_result == 99 and calculation.path_id == "p4":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 4.0
            goal.target_pose.pose.position.y = -4.0
            goal.target_pose.pose.orientation.x = 0.0
            goal.target_pose.pose.orientation.y = 0.0
            goal.target_pose.pose.orientation.z = 0.9301
            goal.target_pose.pose.orientation.w = 0.3671
            client.send_goal(goal)
            client.wait_for_result()
            rospy.loginfo("==p4 arrived!==")
            
            
        elif calculation.move_base_result == 3:
            try:
                transform = tf_buffer.lookup_transform("map", "raw_odom", rospy.Time(0), rospy.Duration(3.0))
                calculation.carto_pose = tf2_geometry_msgs.do_transform_pose(calculation.odom_pose, transform)
  
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException) as error:
                rospy.loginfo(error)
                continue
                
            calculation.call_model_pose()
            calculation.point_err_calculation(calculation.path_id)
            rospy.loginfo(calculation.p1_err)
            rospy.loginfo(calculation.p2_err)
            rospy.loginfo(calculation.p3_err)
            rospy.loginfo(calculation.p4_err)
            rospy.loginfo(calculation.p1_err_yaw)
            rospy.loginfo(calculation.p2_err_yaw)
            rospy.loginfo(calculation.p3_err_yaw)
            rospy.loginfo(calculation.p4_err_yaw)
            calculation.move_base_result = 1

    rospy.spin()
