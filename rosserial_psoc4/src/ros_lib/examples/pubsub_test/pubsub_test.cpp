/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#define NH_DIAGNOSTICS

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <rosserial_psoc4/Nh_diag.h>

#include "BlueLed_psoc4.h"
#include "isnprintf.h"

ros::NodeHandle  nh;

uint32_t blink_count;
bool ledOn;
void messageCb( const std_msgs::Empty& toggle_msg){
  ledOn = !ledOn;   // blink the led
  setBlueLed(ledOn);
  blink_count++;
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 1000;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

rosserial_psoc4::Nh_diag diag_msg;
ros::Publisher nh_diag("nh_diag", &diag_msg);

char hello[13] = "hello world!";

uint16_t diagnostic_record[ros::NodeHandle::NUM_DIAGNOSTICS];

void setup()
{
  blueLed_setup();
  ledOn = false;
  blink_count=0;
  setBlueLed(ledOn);
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(nh_diag);
  nh.subscribe(sub);
  next_report_time = SysTimer::millis();
  delay(500);
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    str_msg.data = hello;
    chatter.publish( &str_msg );
    isnprintf(hello+6, 5, "%d", blink_count);
    blink_count = 0;
    for (int i=0; i<ros::NodeHandle::NUM_DIAGNOSTICS; ++i) {
      diagnostic_record[i] = nh.nh_diagnostics[i];
      nh.nh_diagnostics[i] -= diagnostic_record[i];
    }
    diag_msg.data_length = ros::NodeHandle::NUM_DIAGNOSTICS;
    diag_msg.data = diagnostic_record;
    nh_diag.publish( &diag_msg);
  }
  nh.spinOnce();
}
