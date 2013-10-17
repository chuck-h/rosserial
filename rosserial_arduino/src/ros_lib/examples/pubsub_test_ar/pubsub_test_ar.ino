/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <rosserial_arduino/Nh_diag.h>


ros::NodeHandle  nh;


bool ledOn;
void messageCb( const std_msgs::Empty& toggle_msg){
  ledOn = !ledOn;   // blink the led
  digitalWrite(13, ledOn ? HIGH : LOW);
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 1000;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

rosserial_arduino::Nh_diag diag_msg;
ros::Publisher nh_diag("nh_diag", &diag_msg);

char hello[13] = "hello world!";

int32_t loop_count;

void setup()
{
  ledOn = false;
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(nh_diag);
  nh.subscribe(sub);
  next_report_time = millis();
}

void loop()
{
  if ((int32_t)(millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    str_msg.data = hello;
    chatter.publish( &str_msg );
    for (int i=0; i<8; ++i) {
      diag_msg.nh_diag[i] = nh.nh_diagnostics[i];
      nh.nh_diagnostics[i] = 0;
    }
    nh_diag.publish( &diag_msg );
  }
  nh.spinOnce();
}
