/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt32.h>
#include <rosserial_psoc4/Nh_diag.h>

#include "BlueLed_psoc4.h"
#include "UartStatus_psoc4.h"
#include "isnprintf.h"


ros::NodeHandle  nh;


bool ledOn;
void messageCb( const std_msgs::Empty& toggle_msg){
  ledOn = !ledOn;   // blink the led
  setBlueLed(ledOn);
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 1000;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

rosserial_psoc4::Nh_diag diag_msg;
ros::Publisher nh_diag("nh_diag", &diag_msg);

std_msgs::UInt32 uartstat_msg;
ros::Publisher uartstat("uartstat", &uartstat_msg);

std_msgs::UInt32 bufovf_msg;
ros::Publisher bufovf("bufovf", &bufovf_msg);

char hello[13] = "hello world!";

int32_t loop_count;

void setup()
{
  blueLed_setup();
  ledOn = false;
  setBlueLed(ledOn);
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(nh_diag);
  nh.advertise(uartstat);
  nh.advertise(bufovf);
  nh.subscribe(sub);
  next_report_time = SysTimer::millis();
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    str_msg.data = hello;
    chatter.publish( &str_msg );
    isnprintf(hello+6, 5, "%d", loop_count++);
    for (int i=0; i<8; ++i) {
      diag_msg.nh_diag[i] = nh.nh_diagnostics[i];
      nh.nh_diagnostics[i] = 0;
    }
    nh_diag.publish( &diag_msg );
    uartstat_msg.data = getUartIntStatus();
    uartstat.publish( &uartstat_msg );
    bufovf_msg.data = getUartRxBufOvf();
    bufovf.publish( &bufovf_msg );
  }
  nh.spinOnce();
}
