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

#ifndef VELODYNE_DRIVER_CALIBRATION_H
#define VELODYNE_DRIVER_CALIBRATION_H

#include <map>
#include <vector>
#include <string>

namespace velodyne_pointcloud
{

/** \brief correction values for a single laser
 *
 * Correction values for a single laser (as provided by db.xml from
 * Velodyne).  Includes parameters for Velodyne HDL-64E S2.1.
 *
 */

/** \brief Correction information for a single laser. */
    struct LaserCorrection
    {
        /** parameters in db.xml */
        float rot_correction;
        float vert_correction;
        float dist_correction;
        bool two_pt_correction_available;
        float dist_correction_x;
        float dist_correction_y;
        float vert_offset_correction;
        float horiz_offset_correction;
        int max_intensity;
        int min_intensity;
        float focal_distance;
        float focal_slope;

        /** cached values calculated when the calibration file is read */
        float cos_rot_correction;              ///< cosine of rot_correction
        float sin_rot_correction;              ///< sine of rot_correction
        float cos_vert_correction;             ///< cosine of vert_correction
        float sin_vert_correction;             ///< sine of vert_correction
        int laser_ring;                        ///< ring number for this laser
    };

/** \brief Calibration information for the entire device. */
    class Calibration
    {
    public:
        float distance_resolution_m;
        //std::map<int, LaserCorrection> laser_corrections_map;
        std::vector<LaserCorrection> laser_corrections;
        int num_lasers;
        bool initialized;
        bool ros_info;

    public:
        explicit Calibration(bool info = true)
                : distance_resolution_m(0.002f),
                  num_lasers(0),
                  ros_info(info) {}
        void setupParams(Calibration& calibration);
        ~Calibration(){}

    };

}

#endif //VELODYNE_DRIVER_CALIBRATION_H
