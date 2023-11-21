#!/usr/bin/env python3
# license removed for brevity

from geometry_msgs.msg import PoseWithCovarianceStamped, Twist
from apriltag_ros.msg import *
import rospy
import math

class cabinet_docking(object):
    def  __init__(self):
        self.tag_pose = None
        self.pub_cmd_vel = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        
    def tag_detections_callback(self, msg):
        self.tag_pose = msg
        print(self.tag_pose)

        self.vel_calculation()

    def vel_calculation(self):
        print("here")




if __name__ == '__main__':
    #ROS node
    rospy.init_node('cabinet_docking', anonymous = True)
    print("cabinet_docking node is ready")
    docking = cabinet_docking()
    rospy.Subscriber("/tag_detections", AprilTagDetectionArray, docking.tag_detections_callback)
    rospy.spin()
