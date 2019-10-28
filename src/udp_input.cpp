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


#include <unistd.h>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "udp_input.h"
#include <time.h>

namespace velodyne_driver{

    VelodynePacket velodynePacket;
    static const size_t packet_size = sizeof(velodynePacket.data);

    InputSocket::InputSocket(uint16_t port)
    {
        sockfd_ = -1;

        // connect to Velodyne UDP port
        std::cout<< "Opening UDP socket: port " << port<<std::endl;
        sockfd_ = socket(PF_INET, SOCK_DGRAM, 0);
        if (sockfd_ == -1)
        {
            perror("socket");               // TODO: ROS_ERROR errno
            return;
        }

        sockaddr_in my_addr;                     // my address information
        memset(&my_addr, 0, sizeof(my_addr));    // initialize to zeros
        my_addr.sin_family = AF_INET;            // host byte order
        my_addr.sin_port = htons(port);          // port in network byte order
        my_addr.sin_addr.s_addr = INADDR_ANY;    // automatically fill in my IP

        if (bind(sockfd_, (sockaddr *)&my_addr, sizeof(sockaddr)) == -1)
        {
            perror("bind");                 // TODO: ROS_ERROR errno
            return;
        }

        if (fcntl(sockfd_,F_SETFL, O_NONBLOCK|FASYNC) < 0)
        {
            perror("non-block");
            return;
        }

        std::cout<<"Velodyne socket fd is "<< sockfd_<<std::endl;
    }

    InputSocket::~InputSocket(void)
    {
        (void) close(sockfd_);
    }

    int InputSocket::getPacket(VelodynePacket *pkt, const double time_offset)
    {
        time_t tt = time(NULL);

        struct pollfd fds[1];
        fds[0].fd = sockfd_;
        fds[0].events = POLLIN;
        static const int POLL_TIMEOUT = 5000; // one second (in msec)

        sockaddr_in sender_address;
        socklen_t sender_address_len = sizeof(sender_address);

        while (true)
        {
            // Unfortunately, the Linux kernel recvfrom() implementation
            // uses a non-interruptible sleep() when waiting for data,
            // which would cause this method to hang if the device is not
            // providing data.  We poll() the device first to make sure
            // the recvfrom() will not block.
            //
            // Note, however, that there is a known Linux kernel bug:
            //
            //   Under Linux, select() may report a socket file descriptor
            //   as "ready for reading", while nevertheless a subsequent
            //   read blocks.  This could for example happen when data has
            //   arrived but upon examination has wrong checksum and is
            //   discarded.  There may be other circumstances in which a
            //   file descriptor is spuriously reported as ready.  Thus it
            //   may be safer to use O_NONBLOCK on sockets that should not
            //   block.

            // poll() until input available

            do
            {
                int retval = poll(fds, 1, POLL_TIMEOUT);
                if (retval < 0)             // poll() error?
                {
                    if (errno != EINTR)
                        std::cout<<"poll() error: %s" <<strerror(errno)<<std::endl;
                    return -1;
                }
                if (retval == 0)            // poll() timeout?
                {
                    std::cout<<"Velodyne poll() timeout"<<std::endl;
                    return -1;
                }
                if ((fds[0].revents & POLLERR)
                    || (fds[0].revents & POLLHUP)
                    || (fds[0].revents & POLLNVAL)) // device error?
                {
                    std::cout<<"poll() reports Velodyne error"<<std::endl;
                    return -1;
                }
            } while ((fds[0].revents & POLLIN) == 0);
              

            // Receive packets that should now be available from the
            // socket using a blocking read.
            ssize_t nbytes = recvfrom(sockfd_, &pkt->data[0],
                                      packet_size,  0,
                                      (sockaddr*) &sender_address,
                                      &sender_address_len);
            std::cout<<"number of bytes received"<<nbytes<<std::endl;

            if (nbytes < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    perror("recvfail");
                    std::cout<<"recvfail"<<std::endl;
                    return -1;
                }
            }
            else if ((size_t) nbytes == packet_size)
            {
                break; //done
            }

            std::cout<<"incomplete Velodyne packet read: " << nbytes << " bytes"<<std::endl;
        }

        // Average the times at which we begin and end reading.  Use that to
        // estimate when the scan occurred. Add the time offset.
        time_t tt2 = time(NULL);
        pkt->stamp = (tt + tt2) / 2 + time_offset;

        return 0;
    }


}
