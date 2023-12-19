-- Copyright 2016 The Cartographer Authors
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--      http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

include "map_builder.lua"
include "trajectory_builder.lua"

options = {
  map_builder = MAP_BUILDER,
  trajectory_builder = TRAJECTORY_BUILDER,
  map_frame = "map",
  tracking_frame = "imu_ch110_link",
  published_frame = "odom_filtered",
  odom_frame = "odom_filtered",
  provide_odom_frame = false,
  publish_frame_projected_to_2d = false,
  use_pose_extrapolator = true,
  use_odometry = false,
  use_nav_sat = false,
  use_landmarks = false,
  num_laser_scans = 2,
  num_multi_echo_laser_scans = 0,
  num_subdivisions_per_laser_scan = 1,
  num_point_clouds = 0,
  lookup_transform_timeout_sec = 0.2,
  submap_publish_period_sec = 0.3,
  pose_publish_period_sec = 5e-3,
  trajectory_publish_period_sec = 30e-3,
  rangefinder_sampling_ratio = 1.,
  odometry_sampling_ratio = 0.75, --1.
  fixed_frame_pose_sampling_ratio = 1.,
  imu_sampling_ratio = 1.,
  landmarks_sampling_ratio = 1.,
}

MAP_BUILDER.use_trajectory_builder_2d = true
TRAJECTORY_BUILDER_2D.use_imu_data = true
TRAJECTORY_BUILDER_2D.num_accumulated_range_data = 5 --20 --10 --80 
TRAJECTORY_BUILDER_2D.submaps.num_range_data = 100 --90
TRAJECTORY_BUILDER_2D.submaps.grid_options_2d.resolution = 0.02 -- 0.02 0.05
TRAJECTORY_BUILDER_2D.use_online_correlative_scan_matching = false --true
TRAJECTORY_BUILDER_2D.min_range = 0.5
TRAJECTORY_BUILDER_2D.max_range = 8.0  --6.0
TRAJECTORY_BUILDER_2D.missing_data_ray_length = 4.

TRAJECTORY_BUILDER_2D.motion_filter.max_time_seconds= 5.
TRAJECTORY_BUILDER_2D.motion_filter.max_distance_meters = 0.1
TRAJECTORY_BUILDER_2D.motion_filter.max_angle_radians = math.rad(3.0) --3

TRAJECTORY_BUILDER_2D.ceres_scan_matcher.occupied_space_weight = 1.
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.translation_weight = 5 --1.
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.rotation_weight = 8 --1.
--new added============================
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.ceres_solver_options.max_num_iterations = 2e3 --15
TRAJECTORY_BUILDER_2D.voxel_filter_size = 0.025 --0.05
TRAJECTORY_BUILDER_2D.adaptive_voxel_filter.max_length = 1.0
TRAJECTORY_BUILDER_2D.adaptive_voxel_filter.min_num_points = 50

TRAJECTORY_BUILDER_2D.loop_closure_adaptive_voxel_filter.min_num_points = 30
TRAJECTORY_BUILDER_2D.loop_closure_adaptive_voxel_filter.max_range = 10.
TRAJECTORY_BUILDER_2D.loop_closure_adaptive_voxel_filter.max_length = 1.8

TRAJECTORY_BUILDER_2D.real_time_correlative_scan_matcher.linear_search_window = 0.1
TRAJECTORY_BUILDER_2D.real_time_correlative_scan_matcher.translation_delta_cost_weight = 10.
TRAJECTORY_BUILDER_2D.real_time_correlative_scan_matcher.rotation_delta_cost_weight = 1e-1
--TRAJECTORY_BUILDER_2D.motion_filter.max_angle_radians = math.rad(0.2)

--==================================================================
POSE_GRAPH.optimize_every_n_nodes = 35
POSE_GRAPH.global_sampling_ratio = 0.001
POSE_GRAPH.constraint_builder.sampling_ratio = 0.21
POSE_GRAPH.constraint_builder.min_score = 0.65 --0.85 --0.75
--new added============================
POSE_GRAPH.constraint_builder.global_localization_min_score = 0.65
POSE_GRAPH.constraint_builder.max_constraint_distance = 25

POSE_GRAPH.constraint_builder.loop_closure_translation_weight = 1.1e4 --1e7
POSE_GRAPH.constraint_builder.loop_closure_rotation_weight = 1e5 --1e8

POSE_GRAPH.constraint_builder.ceres_scan_matcher.translation_weight = 1e5 --100--10.
POSE_GRAPH.constraint_builder.ceres_scan_matcher.rotation_weight = 1e5 --10--1.

POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher_3d.linear_xy_search_window = 3
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher_3d.linear_z_search_window = 0.6
POSE_GRAPH.constraint_builder.fast_correlative_scan_matcher_3d.angular_search_window = math.rad(15.)

POSE_GRAPH.optimization_problem.acceleration_weight = 1e3 --2e2
POSE_GRAPH.optimization_problem.rotation_weight = 3e5 --3e3

POSE_GRAPH.optimization_problem.huber_scale = 1e2
POSE_GRAPH.optimization_problem.ceres_solver_options.use_nonmonotonic_steps = false
POSE_GRAPH.optimization_problem.ceres_solver_options.max_num_iterations = 20
POSE_GRAPH.optimization_problem.ceres_solver_options.num_threads = 8

POSE_GRAPH.max_num_final_iterations = 2e3 --1
--==================================================================
MAP_BUILDER.num_background_threads = 8



return options
