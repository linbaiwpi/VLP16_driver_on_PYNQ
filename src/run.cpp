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
#include <unistd.h>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include "VelodyneDriver.h"
#include "run.h"

using namespace velodyne_driver;

int velodyne_run(float *ip) {
    VelodyneDriver dvr;

    //while(dvr.poll()){
    //}
    dvr.poll(ip);
    return 0;
}

void velodyne_display(char* msg){
    printf("%s\n",msg);
}

