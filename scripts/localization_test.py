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
        self.move_base_result = 99
        self.path_id = "go"
        self.fig_num = 1
        self.model_name = '/home/ros1/ros1_ws/src/delta_amr2st_gazebo/urdf/AMR_tag/TY2_SW.urdf.xacro'
        self.odom_sub = rospy.Subscriber("/odom_converted", Odometry, self.odom_callback, queue_size=1)
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
            
    def err_calculation(self):
        self.carto_poses.append(self.carto_pose.pose.position)
        self.model_poses.append(self.model_pose.pose.position)
        dist = math.sqrt((self.carto_pose.pose.position.x - self.model_pose.pose.position.x)**2 + (self.carto_pose.pose.position.y - self.model_pose.pose.position.y)**2)
        self.errs.append(dist)
        rospy.loginfo(dist)
        
    def draw_graph(self):
        X_carto = []
        X_model = []
        Y_carto = []
        Y_model = []
        
        for pose in self.carto_poses:
            X_carto.append(pose.x)
            Y_carto.append(pose.y)
        for pose in self.model_poses:
            X_model.append(pose.x)
            Y_model.append(pose.y)
            
        plt.figure(1)
        fig, axes = plt.subplots(nrows=2, ncols=1, figsize=(10, 7))
        fig.tight_layout()
        
        fig.subplots_adjust(hspace=0.25, wspace=0.7)
        plt.subplot(211)
        plt.plot(X_carto, Y_carto, label='true pose trajectory')
        plt.plot(X_model, Y_model, label='carto pose trajectory')
        plt.xlabel('x coordinate(m)', color='black')
        plt.ylabel('y coordinate(m)', color='black')
        plt.title('cartographer localization vs ground trouth', color='black')
        leg = plt.legend(loc='upper center')
        
        plt.subplot(212)
        plt.plot(self.errs, label='error(m)')
        plt.xlabel('num of pose', color='black')
        plt.ylabel('distance between carto pose and true pose(m)', color='black')
        plt.title('cartographer localization estimation', color='black')
        plt.axhline(y=0.05, color='r', linestyle='--')
        plt.axhline(y=0.01, color='r', linestyle='--')
        leg = plt.legend(loc='upper right')

        #plt.show()
        fig.savefig('/home/ros1/localization_test/dwa_obs/'+ str(self.fig_num) +'.png')   # save the figure to file
        self.fig_num+=1
        plt.close(fig)
        
        self.carto_pose = None
        self.odom_pose = None
        self.model_pose = None
        self.carto_poses = []
        self.model_poses = []
        self.errs = []
            

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
    
        if calculation.odom_pose is not None and calculation.move_base_result == 1:
        
            try:
                transform = tf_buffer.lookup_transform("map", "odom_combined", rospy.Time(0), rospy.Duration(3.0))
                calculation.carto_pose = tf2_geometry_msgs.do_transform_pose(calculation.odom_pose, transform)
                calculation.call_model_pose()
                calculation.err_calculation()
  
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException) as error:
                rospy.loginfo(error)
                continue
                
        elif calculation.move_base_result == 3:
            
            calculation.draw_graph()
            calculation.move_base_result = 99
            
            if calculation.path_id == "go":
                calculation.path_id = "back"
                
            elif calculation.path_id == "back":
                calculation.path_id = "go"
            
        elif calculation.move_base_result == 99 and calculation.path_id == "go":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 5.0
            goal.target_pose.pose.position.y = 5.0
            goal.target_pose.pose.orientation.z = 0.0
            goal.target_pose.pose.orientation.w = 1.0
            client.send_goal(goal)
            calculation.move_base_result = 1

        
        elif calculation.move_base_result == 99 and calculation.path_id == "back":
            goal.target_pose.header.frame_id = 'map' 
            goal.target_pose.pose.position.x = 0.0
            goal.target_pose.pose.position.y = 0.0
            goal.target_pose.pose.orientation.z = 0.0
            goal.target_pose.pose.orientation.w = 1.0
            client.send_goal(goal)
            calculation.move_base_result = 1

    rospy.spin()
