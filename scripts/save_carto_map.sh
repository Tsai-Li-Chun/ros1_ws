rosservice call /finish_trajectory "trajectory_id: 0" &
sleep 3
rosservice call /write_state "filename: /home/deltaagv/ros1_ws/map/new_map.pbstream" &
sleep 3
rosrun map_server map_saver_cartographer -f /home/deltaagv/ros1_ws/map/231127_delta_8f
