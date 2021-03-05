#pragma once

#include "wled.h"

// #include "osc_common.h"

#include <Arduino.h>

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
#define BUILTIN_LED 13
#endif
#endif

#define DEBUG_PRINT_COMMANDS

/**
 * LEDJ Tab
 * */
#define D_OSC_ADDRESS_MULTIFADER_HUE_GROUP_NAME   "/ledj/HueSpeed_HueOffset#"
#define D_OSC_ADDRESS_MULTIFADER_HUE_SPEED        D_OSC_ADDRESS_MULTIFADER_HUE_GROUP_NAME "/1"
#define D_OSC_ADDRESS_MULTIFADER_HUE_OFFSET       D_OSC_ADDRESS_MULTIFADER_HUE_GROUP_NAME "/2"

#define D_OSC_ADDRESS_MULTIFADER_STROBE_GROUP_NAME  "/ledj/StrobeSpeed_StrobeDuty_StrobeFade_StrobeOffset#"
#define D_OSC_ADDRESS_MULTIFADER_STROBE_SPEED       D_OSC_ADDRESS_MULTIFADER_STROBE_GROUP_NAME "/1"
#define D_OSC_ADDRESS_MULTIFADER_STROBE_DUTY        D_OSC_ADDRESS_MULTIFADER_STROBE_GROUP_NAME "/2"
#define D_OSC_ADDRESS_MULTIFADER_STROBE_FADE        D_OSC_ADDRESS_MULTIFADER_STROBE_GROUP_NAME "/3"
#define D_OSC_ADDRESS_MULTIFADER_STROBE_OFFSET      D_OSC_ADDRESS_MULTIFADER_STROBE_GROUP_NAME "/4"



struct OSC_Data
{
  // std::vector<OSC_Member> data;
  float hue;
  float hue_speed;
  float strobe_speed;
  float strobe_duty;
  float strobe_fade;
  float strobe_offset;

  void print(void)
  {
    Serial.printf("Hue: %f\n\r", hue);
    Serial.printf("Hue Speed: %f\n\r", hue_speed);
  }
};

OSC_Data osc_data;

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

//  #define ENABLE_DEVFEATURE_SHOW_UDP_PACKET
#define DISABLE_OSC_COMMON

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
#ifndef OSC_HOST_IP
#define OSC_HOST_IP 192, 168, 1, 1
#endif
const IPAddress outIp(OSC_HOST_IP);  // remote IP (not needed for receive)
const unsigned int outPort = 8000;   // remote port (not needed for receive)
const unsigned int localPort = 7000; // local port to listen for UDP packets (here's where we send the packets)

//  #ifndef ENABLE_DEVFEATURE_SHOW_UDP_PACKET
// #include <OSCMessage.h>
// #include <OSCBundle.h>
// #include <OSCData.h>

#include "src/dependencies/OSC/OSCMessage.h"
#include "src/dependencies/OSC/OSCBundle.h"
#include "src/dependencies/OSC/OSCData.h"

OSCErrorCode error;
unsigned int ledState = LOW; // LOW means led is *on*

#define _UDP_TX_PACKET_MAX_SIZE 1000

// buffers for receiving and sending data
char packetBuffer[_UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

class OSC_Usermod : public Usermod
{
public:
  void setup()
  {
    
  }

  void loop()
  {

    if (abs(millis() -  tTest) > 1000)
    {
      tTest = millis();

      // test_send_osc_message();

      
      if(osc_data.hue++>255){osc_data.hue = 0;}



       OSCMessage msg("/ledj/label_HueSpeed");
        msg.add(osc_data.hue_speed);
        Udp.beginPacket(outIp, outPort);
        msg.send(Udp);
        Udp.endPacket();
        msg.empty();

       OSCMessage msg2("/ledj/HueValue");
        msg2.add(osc_data.hue_speed);
        Udp.beginPacket(outIp, outPort);
        msg2.send(Udp);
        Udp.endPacket();
        msg2.empty();

       OSCMessage msg3("/ledj/SaturationValue");
        msg3.add(osc_data.hue_speed);
        Udp.beginPacket(outIp, outPort);
        msg3.send(Udp);
        Udp.endPacket();
        msg3.empty();


    }

    if (WLED_CONNECTED)
    {
      // No idea what this is not working anymore, undefined reference...
      poll_osc_receive_dispatch();
      // listen_osc_receive_1();

    }
      // splash_udp_packet_serial();
    
  }

  void connected()
  {
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("UDP server on port %d\n", localPort);
    Udp.begin(localPort);
  }
    
  void poll_osc_receive_dispatch(void)
  {
    
    OSCMessage message;
    int size = Udp.parsePacket();

    if (size > 0)
    {

      while (size--)
      {
        message.fill(Udp.read());
      }
      if (!message.hasError())
      {
          
        char buffer[100] = {0};
        message.getAddress(buffer, 0, 100);
        Serial.printf("osc_address=\"%s\"\n\r",buffer);

        // Hue Multifaders
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_HUE_SPEED,  cb_command_set_hue_speed);
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_HUE_OFFSET, cb_command_set_hue_offset);

        // Strobe Multifaders
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_SPEED,     cb_command_set_strobe_speed);
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_DUTY,      cb_command_set_strobe_duty);
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_FADE,      cb_command_set_strobe_fade);
        message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_OFFSET,    cb_command_set_strobe_offset);
      }
      else
      {
          error = message.getError();
          Serial.print("error: ");
          Serial.println(error);
      }
    }

  }



private :

  void listen_osc_receive_1(void);
  void parse_oscmessage_commands_1(OSCMessage *msg);
  void parse_oscmessage_commands_2(OSCMessage *msg);
  void splash_udp_packet_serial(void);
  // void poll_osc_receive_dispatch(void);
  void test_send_osc_message(void);

  uint32_t tTest = millis();

  static void cb_command_set_hue_speed(OSCMessage &msg) {
    osc_data.hue = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("hue=%f\n\r",osc_data.hue);
    #endif
  }

  static void cb_command_set_hue_offset(OSCMessage &msg) {
    osc_data.hue_speed = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("hue_speed=%f\n\r",osc_data.hue_speed);
    #endif
  }

  static void cb_command_set_strobe_speed(OSCMessage &msg) {
    osc_data.strobe_speed = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("strobe_speed=%f\n\r",osc_data.strobe_speed);
    #endif
  }

  static void cb_command_set_strobe_duty(OSCMessage &msg) {
    osc_data.strobe_duty = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("strobe_duty=%f\n\r",osc_data.strobe_duty);
    #endif
  }

  static void cb_command_set_strobe_fade(OSCMessage &msg) {
    osc_data.strobe_fade = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("strobe_fade=%f\n\r",osc_data.strobe_fade);
    #endif
  }

  static void cb_command_set_strobe_offset(OSCMessage &msg) {
    osc_data.strobe_offset = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("strobe_offset=%f\n\r",osc_data.strobe_offset);
    #endif
  }


  

};