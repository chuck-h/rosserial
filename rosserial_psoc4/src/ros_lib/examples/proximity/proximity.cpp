/* 
 * rosserial proximity Example
 * 
 */


#include <ros.h>
#include <std_msgs/UInt16.h>
#include "prox_psoc4.h"

ros::NodeHandle nh;

std_msgs::UInt16 prox_msg0, prox_msg1;
ros::Publisher p0("prox0", &prox_msg0);
ros::Publisher p1("prox1", &prox_msg1);


uint32_t next_report_time;
const uint32_t kReportIntervalMs = 250;

void setup()
{ 
  nh.initNode();
  nh.advertise(p0);
  nh.advertise(p1);
  next_report_time = SysTimer::millis();
  prox_setup();
}


void loop()
{
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    prox_msg0.data = prox_read(0);
    p0.publish(&prox_msg0);
    prox_msg1.data = prox_read(1);
    p1.publish(&prox_msg1);
  }
  prox_spin();
  nh.spinOnce();

}

