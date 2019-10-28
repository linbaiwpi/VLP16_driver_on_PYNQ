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

#ifndef VELODYNE_DRIVER_UDP_INPUT_H
#define VELODYNE_DRIVER_UDP_INPUT_H

#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

namespace velodyne_driver{

    struct VelodynePacket{
        uint8_t data[1206];
        time_t stamp;
    };


    static uint16_t DATA_PORT_NUMBER = 2368;     // default data port

    /** @brief Live Velodyne input from socket. */
    class InputSocket
    {
    public:
        InputSocket(uint16_t port);
        virtual ~InputSocket();
        virtual int getPacket(VelodynePacket *pkt, const double time_offset);
    private:
        int sockfd_;
    };


    /** @brief Velodyne input from PCAP dump file.
     *
     * Dump files can be grabbed by libpcap, Velodyne's DSR software,
     * ethereal, wireshark, tcpdump, or the \ref vdump_command.
     */
    // velodyne_driver namespace

}

#endif //VELODYNE_DRIVER_UDP_INPUT_H



