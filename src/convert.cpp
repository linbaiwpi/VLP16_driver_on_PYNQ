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

#include <iostream>
#include <fstream>
#include "convert.h"
#include "pointcloudXYZIR.h"
#include <time.h>

namespace velodyne_pointcloud{

    Convert::Convert(){
        //std::shared_ptr<velodyne_rawdata::RawData> data_temp (new velodyne_rawdata::RawData);
        data_.reset(new velodyne_rawdata::RawData);
        int set_up = data_->setup();
        data_->setParameters(0.9,200,0,0);
    }

    Convert::~Convert(){
        data_.reset();
    }


    void Convert::processScan(const std::vector<velodyne_driver::VelodynePacket> &scanMsg, float *ip)
    {
        // allocate a point cloud with same time and frame ID as raw data
        PointcloudXYZIR outMsg;
        // outMsg's header is a pcl::PCLHeader, convert it before stamp assignment
        time(&outMsg.t_stamp);
        // outMsg.pc->header.frame_id = scanMsg->header.frame_id;
        // outMsg.pc->height = 1;

        outMsg.pc.reserve(scanMsg.size() * data_->scansPerPacket());

//        std::ofstream fd_t;
//        fd_t.open("test.txt");

        // process each packet provided by the driver
        for (size_t i = 0; i < scanMsg.size(); ++i)
        {
            data_->unpack(scanMsg[i], outMsg);
        }
          
        //for (size_t i = 0; i < 10; ++i)
        for (size_t i = 0; i < outMsg.pc.size(); ++i)
        {
            std::cout<<"the x is "<<outMsg.pc[i].x;
            if(i==outMsg.pc.size()-1)
               std::cout<<""<<std::endl;
            std::cout<<"the y is "<<outMsg.pc[i].y;
            if(i==outMsg.pc.size()-1)
               std::cout<<""<<std::endl;
            std::cout<<"the z is "<<outMsg.pc[i].z;
            if(i==outMsg.pc.size()-1)
               std::cout<<""<<std::endl;
            std::cout<<"the intensity is "<<outMsg.pc[i].intensity;
            if(i==outMsg.pc.size()-1)
               std::cout<<""<<std::endl;
            std::cout<<"the azimuth is "<<unsigned(outMsg.pc[i].azimuth);
            if(i==outMsg.pc.size()-1)
               std::cout<<""<<std::endl;

            ip[i*4+0] = outMsg.pc[i].x;
            ip[i*4+1] = outMsg.pc[i].y;
            ip[i*4+2] = outMsg.pc[i].z;
            ip[i*4+3] = outMsg.pc[i].intensity;

//          fd_t << outMsg.pc[i].x << "," << outMsg.pc[i].y << "," << outMsg.pc[i].z << "," << outMsg.pc[i].intensity <<  "," << unsigned(outMsg.pc[i].azimuth) << ";" << std::endl;
        }
//        fd_t.close();

        std::cout<< "Publishing " << scanMsg.size() * data_->scansPerPacket() << " Velodyne points, time: " << asctime(gmtime(&outMsg.t_stamp))<<std::endl;
    }

}
