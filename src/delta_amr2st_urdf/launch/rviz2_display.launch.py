import launch
from launch.substitutions import Command, LaunchConfiguration
import launch_ros
import os
import xacro

def generate_launch_description():
    #pkg_share = launch_ros.substitutions.FindPackageShare(package='delta_amr2st_urdf').find('delta_amr2st_urdf')
    #default_model_path = os.path.join(pkg_share, 'urdf/TY2_SW.urdf')
    #default_rviz_config_path = os.path.join(pkg_share, 'rviz/urdf_config.rviz')
    doc = xacro.process_file('/home/ros2/ros1_2_ros2_ws/src/delta_amr2st_gazebo/urdf/TY2_SW.urdf.xacro', mappings={'radius': '0.9'})
    robot_desc = doc.toprettyxml(indent='  ')
    params = {'robot_description': robot_desc}

    robot_state_publisher_node = launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='both',
        parameters=[params]
    )
    #joint_state_publisher_node = launch_ros.actions.Node(
    #    package='joint_state_publisher',
    #    executable='joint_state_publisher',
    #    name='joint_state_publisher',
    #    parameters=[params],
    #    condition=launch.conditions.UnlessCondition(LaunchConfiguration('gui'))
    #)
    #joint_state_publisher_gui_node = launch_ros.actions.Node(
    #    package='joint_state_publisher_gui',
    #    executable='joint_state_publisher_gui',
    #    name='joint_state_publisher_gui',
    #    condition=launch.conditions.IfCondition(LaunchConfiguration('gui'))
    #)
    #rviz_node = launch_ros.actions.Node(
    #    package='rviz2',
    #    executable='rviz2',
    #    name='rviz2',
    #    output='screen',
    #    arguments=['-d', LaunchConfiguration('rvizconfig')],
    #)

    return launch.LaunchDescription([
        #launch.actions.DeclareLaunchArgument(name='gui', default_value='True',
        #                                    description='Flag to enable joint_state_publisher_gui'),
        #launch.actions.DeclareLaunchArgument(name='model', default_value=default_model_path,
        #                                    description='Absolute path to robot urdf file'),
        #launch.actions.DeclareLaunchArgument(name='rvizconfig', default_value=default_rviz_config_path,
        #                                    description='Absolute path to rviz config file'),
        #joint_state_publisher_node,
        #joint_state_publisher_gui_node,
        robot_state_publisher_node,
        #rviz_node
    ])