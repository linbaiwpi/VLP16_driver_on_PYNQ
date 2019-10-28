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

#ifndef VELODYNE_DRIVER_POINT_TYPES_H
#define VELODYNE_DRIVER_POINT_TYPES_H

/*#define PCL_ADD_POINT4D
 union {
   float data[4];
      struct {
         float x;
         float y;
         float z;
      };
  } EIGEN_ALIGN16;
*/

#include <unistd.h>
namespace velodyne_pointcloud
{
/** Euclidean Velodyne coordinate, including intensity and ring number. */
    struct PointXYZIR
    {
        //PCL_ADD_POINT4D;                    // quad-word XYZ
        float data[4];
        float x;
        float y;
        float z;
        float intensity;                 ///< laser intensity reading
        u_int16_t azimuth;
        u_int16_t ring;                      ///< laser ring number
        //EIGEN_MAKE_ALIGNED_OPERATOR_NEW     // ensure proper alignment
    } ;
}

#endif //VELODYNE_DRIVER_POINT_TYPES_H
