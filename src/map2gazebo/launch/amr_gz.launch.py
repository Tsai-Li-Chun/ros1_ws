import os
from launch_ros.substitutions import FindPackageShare
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():

    #package_name = 'nav2_launch_files'
    #package_path = FindPackageShare(package = package_name).find(package_name)

    return LaunchDescription([

        DeclareLaunchArgument(
            name='use_sim_time',
            default_value='True',
            description='Use simulation (Gazebo) clock if true'),
        
        ExecuteProcess(
            cmd=['gazebo', 'src/map2gazebo/worlds/map.sdf'],
            output='screen'),

        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher_RF_scan',
            output='screen',
            parameters=[{'use_sim_time': LaunchConfiguration('use_sim_time')}],
            arguments = ["0.375", "-0.215", "0.3", "-0.78539", "0", "0", "base_link", "scan_RF_link"]),

        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher_LB_scan',
            output='screen',
            parameters=[{'use_sim_time': LaunchConfiguration('use_sim_time')}],
            arguments = ["-0.375", "0.215", "0.3", "2.356194", "0", "0", "base_link", "scan_LB_link"]),

        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher_imu',
            output='screen',
            parameters=[{'use_sim_time': LaunchConfiguration('use_sim_time')}],
            arguments = ["0", "0", "0", "0", "0", "0", "base_link", "imu_link"]),

        #Node(
        #    package='robot_localization',
        #    executable='ekf_node',
        #    name='ekf_filter_node',
        #    output='screen',
        #    parameters=[os.path.join(package_path, 'ekf.yaml'), {'use_sim_time': LaunchConfiguration('use_sim_time')}]),

    ])