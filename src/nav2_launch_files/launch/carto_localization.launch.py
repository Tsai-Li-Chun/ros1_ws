import os
from launch_ros.substitutions import FindPackageShare
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import ThisLaunchFileDir


def generate_launch_description():
    
    lua_dir = get_package_share_directory('nav2_launch_files')
    pbstream_path = "/home/ros2/ros1_2_ros2_ws/src/nav2_launch_files/maps/map.pbstream"

    return LaunchDescription([
        
        DeclareLaunchArgument(
            name='use_sim_time',
            default_value='True',
            description='Use simulation (Gazebo) clock if true'),
        

        Node(
            package='cartographer_ros',
            executable='cartographer_node',
            name='cartographer_node',
            output='screen',
            parameters=[{'use_sim_time': LaunchConfiguration('use_sim_time')}],
            arguments = [
                '-configuration_directory', FindPackageShare('nav2_launch_files').find('nav2_launch_files') ,
                '-configuration_basename', 'carto_localization.lua',
                '-load_state_filename', pbstream_path,
            ],
            remappings = [('scan', 'scan')]),

        Node(
            package='cartographer_ros',
            executable='cartographer_occupancy_grid_node',
            name='cartographer_occupancy_grid_node',
            output='screen',
            parameters=[
                {'use_sim_time': LaunchConfiguration('use_sim_time')},
                {'resolution':0.05},
                {'publish_period_sec':0.5}]),
    ])
