/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>

ros::NodeHandle  nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

std_msgs::Int32 int_msg;
ros::Publisher intparam("intparam", &int_msg);

char hello[13] = "hello world!";

// parameter server exercise
char hellomessage[20] = { 0 }; // last entry always remains 0 as string terminator
char* textparams[1];

int32_t parm_ints[1];

bool params_fetched = false;

void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(intparam);
}

void loop()
{
  if (!params_fetched) {
    params_fetched = true;
    if (! nh.getParam("~text", textparams, 1)) { 
      strncpy(hellomessage, hello, sizeof(hellomessage)-1);
      params_fetched = false;
    } else {
      strncpy(hellomessage, textparams[0], sizeof(hellomessage)-1);
    }
    if(!nh.getParam("~pint", parm_ints, 1) ) {
      parm_ints[0] = 42;
      params_fetched = false;
    }
  }  
  str_msg.data = hellomessage;
  chatter.publish( &str_msg );
  int_msg.data = parm_ints[0];
  intparam.publish( &int_msg );
  nh.spinOnce();
  delay(1000);
}
