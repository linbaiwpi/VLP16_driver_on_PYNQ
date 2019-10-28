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

#ifndef VELODYNE_DRIVER_RAWDATA_H
#define VELODYNE_DRIVER_RAWDATA_H

#include <errno.h>
#include <stdint.h>
#include <string>
#include <math.h>

#include "udp_input.h"
#include "datacontainerbase.h"
#include "calibration.h"
#include "point_types.h"

namespace velodyne_rawdata
{
// Shorthand typedefs for point cloud representations
    typedef velodyne_pointcloud::PointXYZIR VPoint;
    typedef std::vector<VPoint> VPointCloud;
    //typedef pcl::PointCloud<VPoint> VPointCloud;

/**
 * Raw Velodyne packet constants and structures.
 */
    static const int SIZE_BLOCK = 100;
    static const int RAW_SCAN_SIZE = 3;
    static const int SCANS_PER_BLOCK = 32;
    static const int BLOCK_DATA_SIZE = (SCANS_PER_BLOCK * RAW_SCAN_SIZE);

    static const float ROTATION_RESOLUTION      = 0.01f;  // [deg]
    static const uint16_t ROTATION_MAX_UNITS    = 36000u;     // [deg/100]

/** @todo make this work for both big and little-endian machines */
    static const uint16_t UPPER_BANK = 0xeeff;
    static const uint16_t LOWER_BANK = 0xddff;


/** Special Defines for VLP16 support **/
    static const int    VLP16_FIRINGS_PER_BLOCK =   2;
    static const int    VLP16_SCANS_PER_FIRING  =  16;
    static const float  VLP16_BLOCK_TDURATION   = 110.592f;   // [µs]
    static const float  VLP16_DSR_TOFFSET       =   2.304f;   // [µs]
    static const float  VLP16_FIRING_TOFFSET    =  55.296f;   // [µs]


/** \brief Raw Velodyne data block.
 *
 *  Each block contains data from either the upper or lower laser
 *  bank.  The device returns three times as many upper bank blocks.
 *
 *  use stdint.h types, so things work with both 64 and 32-bit machines
 */
    typedef struct raw_block
    {
        uint16_t header;        ///< UPPER_BANK or LOWER_BANK
        uint16_t rotation;      ///< 0-35999, divide by 100 to get degrees
        uint8_t  data[BLOCK_DATA_SIZE];
    } raw_block_t;

/** used for unpacking the first two data bytes in a block
 *
 *  They are packed into the actual data stream misaligned.  I doubt
 *  this works on big endian machines.
 */
    union two_bytes
    {
        uint16_t uint;
        uint8_t  bytes[2];
    };

    static const int PACKET_SIZE = 1206;
    static const int BLOCKS_PER_PACKET = 12;
    static const int PACKET_STATUS_SIZE = 4;
    static const int SCANS_PER_PACKET = (SCANS_PER_BLOCK * BLOCKS_PER_PACKET);

/** \brief Raw Velodyne packet.
 *
 *  revolution is described in the device manual as incrementing
 *    (mod 65536) for each physical turn of the device.  Our device
 *    seems to alternate between two different values every third
 *    packet.  One value increases, the other decreases.
 *
 *  \todo figure out if revolution is only present for one of the
 *  two types of status fields
 *
 *  status has either a temperature encoding or the microcode level
 */
    typedef struct raw_packet
    {
        raw_block_t blocks[BLOCKS_PER_PACKET];
        uint16_t revolution;
        uint8_t status[PACKET_STATUS_SIZE];
    } raw_packet_t;

/** \brief Velodyne data conversion class */
    class RawData
    {
    public:
        RawData();
        ~RawData() {}

        /** \brief Set up for data processing.
         *
         *  Perform initializations needed before data processing can
         *  begin:
         *
         *    - read device-specific angles calibration
         *
         *  @param private_nh private node handle for ROS parameters
         *  @returns 0 if successful;
         *           errno value for failure
         */
        int setup();

        void unpack(const velodyne_driver::VelodynePacket &pkt, DataContainerBase& data);

        void setParameters(double min_range, double max_range, double view_direction,
                           double view_width);

        int scansPerPacket() const;

        velodyne_pointcloud::Calibration calibration_;

    private:
        /** configuration parameters */
        typedef struct
        {
            std::string calibrationFile;     ///< calibration file name
            double max_range;                ///< maximum range to publish
            double min_range;                ///< minimum range to publish
            int min_angle;                   ///< minimum angle to publish
            int max_angle;                   ///< maximum angle to publish

            double tmp_min_angle;
            double tmp_max_angle;
        } Config;
        Config config_;

        /**
         * Calibration file
         */

        float sin_rot_table_[ROTATION_MAX_UNITS];
        float cos_rot_table_[ROTATION_MAX_UNITS];

        /** add private function to handle the VLP16 **/
        void unpack_vlp16(const velodyne_driver::VelodynePacket &pkt, DataContainerBase& data);

        /** in-line test whether a point is in range */
        bool pointInRange(float range)
        {
            return (range >= config_.min_range
                    && range <= config_.max_range);
        }
    };
}

#endif //VELODYNE_DRIVER_RAWDATA_H




