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

#include "VelodyneDriver.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fcntl.h>

namespace velodyne_driver
{

    VelodyneDriver::VelodyneDriver()
    {
        config_.model = "VLP16";
        double packet_rate;                   // packet frequency (Hz)
        std::string model_full_name;
        packet_rate = 754.0;             // 754 Packets/Second for Last or Strongest mode 1508 for dual (VLP-16 User Manual)
        model_full_name = "VLP-16";

        std::string deviceName(std::string("Velodyne ") + model_full_name);
        config_.rpm = 600.0;
        double frequency = (config_.rpm / 60.0);     // expected Hz rate

        // default number of packets for each scan is a single revolution
        // (fractions rounded up)
        config_.npackets = (int)ceil(packet_rate / frequency);
        std::cout <<"publishing " << config_.npackets << " packets per scan"<<std::endl;

        std::string dump_file;

        double cut_angle = -0.01;
        if (cut_angle < 0.0)
        {
            std::cout<<"Cut at specific angle feature deactivated."<< std::endl;
        }
        else if (cut_angle < (2 * std::asin(1)))
        {
            std::cout<<"Cut at specific angle feature activated. "<<std::endl;
            std::cout<<"Cutting velodyne points always at %f " << cut_angle << " rad."<<std::endl;
        }
        else
        {
            std::cout<<"cut_angle parameter is out of range. Allowed range is "<<std::endl;
            std::cout<<"between 0.0 and 2*PI or negative values to deactivate this feature."<<std::endl;
            cut_angle = -0.01;
        }

        // Convert cut_angle from radian to one-hundredth degree,
        // which is used in velodyne packets
        config_.cut_angle = int((cut_angle*360/(2*M_PI))*100);

        int udp_port = DATA_PORT_NUMBER;

        last_azimuth_ = -1;
        
        input_.reset(new InputSocket(udp_port));
        conv_.reset(new velodyne_pointcloud::Convert);
    }

    VelodyneDriver::~VelodyneDriver()
    {
        input_.reset();
        conv_.reset();
    }

/** poll the device
 *
 *  @returns true unless end of file reached
 */
    bool VelodyneDriver::poll(float *ip)
    {
        // Allocate a new shared pointer for zero-copy sharing with other nodelets.
        std::vector<VelodynePacket> scan_packets;
        //InputSocket* input_ = new InputSocket(DATA_PORT_NUMBER);
        //velodyne_pointcloud::Convert conv;

        if( config_.cut_angle >= 0) //Cut at specific angle feature enabled
        {
            scan_packets.reserve(config_.npackets);
            VelodynePacket tmp_packet;
            while(true)
            {
                while(true)
                {
                    int rc = input_->getPacket(&tmp_packet, config_.time_offset);
                    if (rc == 0) break;       // got a full packet?
                    if (rc < 0) return false; // end of file reached?
                }
                scan_packets.push_back(tmp_packet);

                // Extract base rotation of first block in packet
                std::size_t azimuth_data_pos = 100*0+2;
                int azimuth = *( (u_int16_t*) (&tmp_packet.data[azimuth_data_pos]));

                //if first packet in scan, there is no "valid" last_azimuth_
                if (last_azimuth_ == -1) {
                    last_azimuth_ = azimuth;
                    continue;
                }
                if((last_azimuth_ < config_.cut_angle && config_.cut_angle <= azimuth)
                   || ( config_.cut_angle <= azimuth && azimuth < last_azimuth_)
                   || (azimuth < last_azimuth_ && last_azimuth_ < config_.cut_angle))
                {
                    last_azimuth_ = azimuth;
                    break; // Cut angle passed, one full revolution collected
                }
                last_azimuth_ = azimuth;
            }
        }
        else // standard behaviour
        {
            // Since the velodyne delivers data at a very high rate, keep
            // reading and publishing scans as fast as possible.
            scan_packets.resize(config_.npackets);
            
            int fd_t = -1;
            fd_t = open("test.txt",O_RDWR);
            if(fd_t == -1){
               std::cout<<"write failed\n"<<std::endl;
            }
            else{
               std::cout<<"open successfully\n"<<std::endl;
            }
            
            for (int i = 0; i < config_.npackets; ++i)
            {
                while (true)
                {
                    // keep reading until full packet received
                    int rc = input_->getPacket(&scan_packets[i], config_.time_offset);
                    if (rc == 0){                 
                        break;
                    }      // got a full packet?
                    if (rc < 0){
                        /*for(int i=0;i<1;++i){
                          for(int j=0;j<1206;j++){
                            std::cout<<unsigned(scan_packets[i].data[j])<<",";
                          }
                          int ret = -1;
                          ret = write(fd_t,&scan_packets[i].data[0],1206);
                          if(ret<0){
                             std::cout<<"write failed\n"<<std::endl;
                          } 
                        }
                       */
                       //conv->processScan(scan_packets);   
                       return false;
                    }  // end of file reached?
                }
            }

            conv_->processScan(scan_packets, ip);

            /*for(int i=0;i<4;++i){
               for(int j=0;j<1206;j++){
                  std::cout<<scan_packets[i].data[j]<<",";
               }
               int ret = -1;
               ret = write(fd_t,&scan_packets[i].data[0],1206);
               if(ret<0){
                  std::cout<<"write failed\n"<<std::endl;
               } 
            }*/
/*
            std::cout<<"start writing ......\n"<<std::endl;
            for(int i=0;i<4;++i){
                for(int j=0;j<1206;j++){
                    std::cout<<int(scan_packets[i].data[j])<<",";
                    r[i*1206+j] = int(scan_packets[i].data[j]);                
                }
                std::cout<<std::endl;
                //int ret = -1;
                //ret = write(fd_t,&scan_packets[i].data[0],1206);
                //if(ret<0){
                //    std::cout<<"write failed\n"<<std::endl;
                //}
            }
            std::cout<<config_.npackets<<std::endl;
            close(fd_t);
*/
        }
        //ip = &r[0];
        return 0;
    }

} // namespace velodyne_driver
