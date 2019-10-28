/*
 * This file is originally copied from ros-driver for velodyne lidar.
 * We (Worcester Polytechinic Institute) modified it so that no ROS os is needed.
 *
 * Use of the original code must obey BSD License 2.0
 * 
 * Authors are from Austin Robot Technology
 * 
 * This code is modified by:
 * Lin Bai, Xin Xu
 * Worester Polytechnic Instittue
 * {lbai2, xxin10}@wpi.edu
 * 2019-10-28
 */

#ifndef VELODYNE_DRIVER_POINTCLOUDXYZIR_H
#define VELODYNE_DRIVER_POINTCLOUDXYZIR_H

#include "rawdata.h"

namespace velodyne_pointcloud
{
    class PointcloudXYZIR : public velodyne_rawdata::DataContainerBase
    {
    public:

        velodyne_rawdata::VPointCloud pc;
        time_t t_stamp;

        //PointcloudXYZIR(){}

        virtual void addPoint(
                const float& x,
                const float& y,
                const float& z,
                const u_int16_t& ring,
                const u_int16_t& azimuth,
                const float& distance,
                const float& intensity);
    };
}  // namespace velodyne_pointclo

#endif //VELODYNE_DRIVER_POINTCLOUDXYZIR_H
