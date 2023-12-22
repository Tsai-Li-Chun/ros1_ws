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

include "backpack_2d_new.lua"

TRAJECTORY_BUILDER.pure_localization_trimmer = {
  max_submaps_to_keep = 3,
}
POSE_GRAPH.optimize_every_n_nodes = 5 --20 --2
POSE_GRAPH.constraint_builder.min_score = 0.75 --0.85
POSE_GRAPH.max_num_final_iterations = 200 --2e3 --1

TRAJECTORY_BUILDER_2D.motion_filter.max_angle_radians = math.rad(0.2)
TRAJECTORY_BUILDER_2D.ceres_scan_matcher.ceres_solver_options.max_num_iterations = 30 --2e3 --15
TRAJECTORY_BUILDER_2D.submaps.num_range_data = 35 --60 --90
TRAJECTORY_BUILDER_2D.max_range = 8.0



return options
