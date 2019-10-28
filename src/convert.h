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

#ifndef VELODYNE_DRIVER_CONVERT_H
#define VELODYNE_DRIVER_CONVERT_H

#include "rawdata.h"
#include <vector>
#include <memory>

namespace velodyne_pointcloud{

    class Convert {
    public:
        Convert();
        ~Convert();
        void processScan(const std::vector<velodyne_driver::VelodynePacket> &scan_packets, float *ip);
        std::shared_ptr<velodyne_rawdata::RawData>  data_;

    private:

        typedef struct
        {
            int npackets;                    // number of packets to combine
        } Config;
        Config config_;
    };
}

#endif //VELODYNE_DRIVER_CONVERT_H
