/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, 2013, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Eitan Marder-Eppstein
 *         David V. Lu!!
 *********************************************************************/
#ifndef COSTMAP_2D_COSTMAP_LAYER_H_
#define COSTMAP_2D_COSTMAP_LAYER_H_
#include <ros/ros.h>
#include <costmap_2d/layer.h>
#include <costmap_2d/layered_costmap.h>

namespace costmap_2d
{

class CostmapLayer : public Layer, public Costmap2D
{
public:
  CostmapLayer() : has_extra_bounds_(false),
    extra_min_x_(1e6), extra_max_x_(-1e6),
    extra_min_y_(1e6), extra_max_y_(-1e6) {}

  bool isDiscretized()
  {
    return true;
  }

  /* 使用主地圖的尺寸來設置該層子地圖的尺寸 */
  virtual void matchSize();

  virtual void clearArea(int start_x, int start_y, int end_x, int end_y, bool invert_area=false);

  /**
   * @brief 將傳入的bound邊界與資料成員的值比較，如果傳入的bound範圍更大，則更新資料成員的值
   * @param mx0 Minimum x value of the bounding box
   * @param my0 Minimum y value of the bounding box
   * @param mx1 Maximum x value of the bounding box
   * @param my1 Maximum y value of the bounding box
   */
  void addExtraBounds(double mx0, double my0, double mx1, double my1);

protected:
  /* 使用當前子地圖資料(包括未知cell)覆蓋主地圖對應區域 */
  void updateWithTrueOverwrite(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

  /* 使用當前子地圖資料(不包括未知cell)覆蓋主地圖對應區域 */
  void updateWithOverwrite(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

  /* 使用當前子地圖資料(不包括未知cell)覆蓋主地圖對應區域 */
  /* * */
  /* 若子地圖cost值比主地圖大或主地圖該cell為未知時，用子地圖資料覆蓋，否則保留主地圖原資料 */
  void updateWithMax(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

  /* 使用當前子地圖資料(不包括未知cell)覆蓋主地圖對應區域 */
  /* * */
  /* 主地圖cell未知時,用子地圖資料覆蓋；反之主地圖原資料基礎上+子地圖資料(會進行限制避免cost值溢出) */
  void updateWithAddition(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

  /**
   * Updates the bounding box specified in the parameters to include the location (x,y)
   * @param x x-coordinate to include
   * @param y y-coordinate to include
   * @param min_x bounding box
   * @param min_y bounding box
   * @param max_x bounding box
   * @param max_y bounding box
   */
  void touch(double x, double y, double* min_x, double* min_y, double* max_x, double* max_y);

  /**
   * @brief 比較傳入的bound與更新後的資料，將更大的範圍通過傳入的指標填充，並恢復資料成員初始值，認為將新增的bound使用過了
   * @brief -- Should be called at the beginning of the updateBounds method
   * @param min_x bounding box (input and output)
   * @param min_y bounding box (input and output)
   * @param max_x bounding box (input and output)
   * @param max_y bounding box (input and output)
   */
  void useExtraBounds(double* min_x, double* min_y, double* max_x, double* max_y);

  bool has_extra_bounds_;

private:
  double extra_min_x_, extra_max_x_, extra_min_y_, extra_max_y_;
};

}  // namespace costmap_2d
#endif  // COSTMAP_2D_COSTMAP_LAYER_H_
