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

#include <string>
#include "udp_input.h"
#include "convert.h"
#include <memory>

#ifndef VELODYNE_DRIVER_VELODYNEDRIVER_H
#define VELODYNE_DRIVER_VELODYNEDRIVER_H

namespace velodyne_driver
{
    class VelodyneDriver
    {
    public:

        VelodyneDriver();
        ~VelodyneDriver();
        bool poll(float *);

    private:

        struct
        {
            std::string frame_id;            ///< tf frame ID
            std::string model;               ///< device model name
            int    npackets;                 ///< number of packets to collect
            double rpm;                      ///< device rotation rate (RPMs)
            int cut_angle;                   ///< cutting angle in 1/100°
            double time_offset;              ///< time in seconds added to each velodyne time stamp
        } config_;

        int last_azimuth_;
        std::shared_ptr<InputSocket> input_;
        std::shared_ptr<velodyne_pointcloud::Convert> conv_;
        
    };

}

#endif //VELODYNE_DRIVER_VELODYNEDRIVER_H


