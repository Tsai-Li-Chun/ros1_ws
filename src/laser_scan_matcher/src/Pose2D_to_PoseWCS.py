#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import PoseWithCovarianceStamped, PoseStamped
from std_msgs.msg import Header
import tf

def pose2D_to_pose_with_covariance(pose):
    scan_matcher_pose = PoseWithCovarianceStamped()
    
    scan_matcher_pose.header = Header()
    scan_matcher_pose.header.stamp = rospy.Time.now()
    scan_matcher_pose.header.frame_id = "odom_combined"
        
    scan_matcher_pose.pose.pose.position.x = pose.pose.position.x
    scan_matcher_pose.pose.pose.position.y = pose.pose.position.y
    scan_matcher_pose.pose.pose.orientation.z = pose.pose.orientation.z
    scan_matcher_pose.pose.pose.orientation.w = pose.pose.orientation.w
    scan_matcher_pose.pose.covariance = [0.1] * 36
    
    return scan_matcher_pose
    
def callback(pose):
    scan_matcher_pose = pose2D_to_pose_with_covariance(pose)
    
    scan_matcher_pose_pub.publish(scan_matcher_pose)
    
if __name__ == '__main__':
    #ROS node
    rospy.init_node('Pose2D_to_PoseWCS', anonymous = True)
    print("Pose2D_to_PoseWCS node is ready")
    rospy.Subscriber("/pose_stamped", PoseStamped, callback)
    scan_matcher_pose_pub = rospy.Publisher('/laser_scan_matcher_pose', PoseWithCovarianceStamped, queue_size = 10)
    rospy.spin()
    

