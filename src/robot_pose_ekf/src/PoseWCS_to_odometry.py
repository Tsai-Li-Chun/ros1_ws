#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry

def pose_with_covariance_to_odometry(pose_with_covariance):
    odometry = Odometry()
    
    odometry.header.frame_id = "odom_combined"
    odometry.child_frame_id = "base_footprint"
    odometry.header.seq = pose_with_covariance.header.seq
    odometry.header.stamp.secs = pose_with_covariance.header.stamp.secs
    odometry.header.stamp.nsecs = pose_with_covariance.header.stamp.nsecs
    odometry.pose.pose = pose_with_covariance.pose.pose
    odometry.pose.covariance = pose_with_covariance.pose.covariance
    
    return odometry
    
def callback(pose_with_covariance):
    odometry = pose_with_covariance_to_odometry(pose_with_covariance)
    
    odometry_pub.publish(odometry)
    
if __name__ == '__main__':
    #ROS node
    rospy.init_node('PoseWCS_to_odometry', anonymous = True)
    print("PoseWCS_to_odometry node is ready")
    rospy.Subscriber("/robot_pose_ekf/odom_combined", PoseWithCovarianceStamped, callback)
    odometry_pub = rospy.Publisher('/odom_converted', Odometry, queue_size = 10)
    rospy.spin()
