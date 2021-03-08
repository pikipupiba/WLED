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
 * Naming convention for app elements (so #defines below can easily have numbers/labels appended to themm. Makes UI harder, but code is easier)
 * 
 * Multifaders: 
 *    - slider "<GroupName_Slider1_Slider2_Slider3"                // convoluted, but since the multifader name will never allow a split from the slider name, we may aswell have (as sub _ named) all the sliders values.. we can change this later,  but just as a basic example
 *    - RHS value "<GroupName_Slider1_Slider2_Slider3-Label_Value" // if the value should be shown as feedback
 *    - LHS value "<GroupName_Slider1_Slider2_Slider3-Label_Text"  // optional we want to edit the name for any reason, but best to have a unique naming convention
 * 
 * Encoders:
 *    - same as multifaders, no label_value
 * 
 * */

/**
 * LEDJ Tab
 * */
#define D_OSC_ADDRESS_HUE "/ledj/enc_hue"

#define D_OSC_ADDRESS_MF_HUE_GROUP   "/ledj/mf_hue_1"
#define D_OSC_ADDRESS_MF_HUE_SPEED               D_OSC_ADDRESS_MF_HUE_GROUP "/1"         //  "/ledj/mf_hue_1/1"
#define D_OSC_ADDRESS_MF_HUE_SPEED_LABEL_TEXT    D_OSC_ADDRESS_MF_HUE_SPEED "-label_text"   //  "/ledj/mf_hue_1/1-label_text"
#define D_OSC_ADDRESS_MF_HUE_SPEED_LABEL_VALUE   "/ledj/label_hue_speed"    //  "/ledj/mf_hue_1/1-label_value"
#define D_OSC_ADDRESS_MF_HUE_OFFSET              D_OSC_ADDRESS_MF_HUE_GROUP "/2"         //  "/ledj/mf_hue_1/2"
#define D_OSC_ADDRESS_MF_HUE_OFFSET_LABEL_TEXT   D_OSC_ADDRESS_MF_HUE_OFFSET "-label_text"  //  "/ledj/mf_hue_1/2-label_text"
#define D_OSC_ADDRESS_MF_HUE_OFFSET_LABEL_VALUE  "/ledj/label_hue_offset"   //  "/ledj/mf_hue_1/2-label_value"


#define D_OSC_ADDRESS_MF_STROBE_GROUP  "/ledj/mf_strobe_1"
#define D_OSC_ADDRESS_MF_STROBE_SPEED              D_OSC_ADDRESS_MF_STROBE_GROUP "/1"        
#define D_OSC_ADDRESS_MF_STROBE_SPEED_LABEL_VALUE  "/ledj/label_strobe_speed" 
#define D_OSC_ADDRESS_MF_STROBE_DUTY               D_OSC_ADDRESS_MF_STROBE_GROUP "/2"      
#define D_OSC_ADDRESS_MF_STROBE_DUTY_LABEL_VALUE   "/ledj/label_strobe_duty"
#define D_OSC_ADDRESS_MF_STROBE_FADE               D_OSC_ADDRESS_MF_STROBE_GROUP "/3"     
#define D_OSC_ADDRESS_MF_STROBE_FADE_LABEL_VALUE   "/ledj/label_strobe_fade" 
#define D_OSC_ADDRESS_MF_STROBE_OFFSET             D_OSC_ADDRESS_MF_STROBE_GROUP "/4"         
#define D_OSC_ADDRESS_MF_STROBE_OFFSET_LABEL_VALUE "/ledj/label_strobe_offset"   



#define D_OSC_ADDRESS_ENCODER_BEAT_OFFSET          "/ledj/BeatOffset"



struct OSC_Data
{
  // std::vector<OSC_Member> data;
  float hue;
  float hue_speed;
  float hue_offset;
  float strobe_speed;
  float strobe_duty;
  float strobe_fade;
  float strobe_offset;

  
  float beat_offset = 0;

  void print(void)
  {
    Serial.printf("Hue: %f\n\r", hue);
    Serial.printf("Hue Speed: %f\n\r", hue_speed);
    Serial.printf("Hue Offset: %f\n\r", hue_offset);
    Serial.printf("Strobe Speed: %f\n\r", strobe_speed);
    Serial.printf("Strobe Duty: %f\n\r", strobe_duty);
    Serial.printf("Strobe Fade: %f\n\r", strobe_fade);
    Serial.printf("Strobe Offset: %f\n\r", strobe_offset);
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
#define OSC_HOST_IP 192, 168, 1, 115
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
  
  template <typename T>
  void send_single_message(char* address, T value){

    // Serial.printf("address=\"%s\"\n\r",address);

    OSCMessage msg(address);
    msg.add(value);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();

  }


  /**
   * Function will internally check if teleperiod is met, or if a flag is set to force update
   * */
  void update_remote_parameters(){
    
    if (abs(millis() -  tSaved_update_remote_parameters) > 100)
    {
      tSaved_update_remote_parameters = millis();
      
      
    // OSCMessage msg("/ledj/label_hue_speed");
    // msg.add(12.35);
    // Udp.beginPacket(outIp, outPort);
    // msg.send(Udp);
    // Udp.endPacket();
    // msg.empty();
      

      send_single_message(D_OSC_ADDRESS_HUE, osc_data.hue);
      send_single_message(D_OSC_ADDRESS_MF_HUE_SPEED_LABEL_VALUE, osc_data.hue_speed);
      send_single_message(D_OSC_ADDRESS_MF_HUE_OFFSET_LABEL_VALUE, osc_data.hue_offset);

      
      send_single_message(D_OSC_ADDRESS_MF_HUE_SPEED, osc_data.hue_speed);

      send_single_message(D_OSC_ADDRESS_MF_STROBE_SPEED_LABEL_VALUE, osc_data.strobe_speed);
      send_single_message(D_OSC_ADDRESS_MF_STROBE_DUTY_LABEL_VALUE, osc_data.strobe_duty);
      send_single_message(D_OSC_ADDRESS_MF_STROBE_FADE_LABEL_VALUE, osc_data.strobe_fade);
      send_single_message(D_OSC_ADDRESS_MF_STROBE_OFFSET_LABEL_VALUE, osc_data.strobe_offset);

    }

  }

  void loop()
  {

    update_remote_parameters();

    if (abs(millis() -  tTest) > 100)
    {
      tTest = millis();

      // test_send_osc_message();

      
      if(osc_data.hue_speed++>255){osc_data.hue_speed = 0;}

      // if(osc_data.beat_offset>1.0f){
      //   osc_data.beat_offset = 0;
      // }else{
      //   osc_data.beat_offset += 0.05;
      // }



      //  OSCMessage msg("/ledj/label_HueSpeed");
      //   msg.add(osc_data.beat_offset);
      //   Udp.beginPacket(outIp, outPort);
      //   msg.send(Udp);
      //   Udp.endPacket();
      //   msg.empty();

      //  OSCMessage msg2("/ledj/HueValue");
      //   msg2.add(osc_data.beat_offset);
      //   Udp.beginPacket(outIp, outPort);
      //   msg2.send(Udp);
      //   Udp.endPacket();
      //   msg2.empty();

      //  OSCMessage msg3("/ledj/BeatOffset");
      //   msg3.add(osc_data.beat_offset);
      //   Udp.beginPacket(outIp, outPort);
      //   msg3.send(Udp);
      //   Udp.endPacket();
      //   msg3.empty();


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

        // Hue Wheel
        message.dispatch(D_OSC_ADDRESS_HUE,  cb_command_set_hue);

        // Hue Multifaders
        message.dispatch(D_OSC_ADDRESS_MF_HUE_SPEED,  cb_command_set_hue_speed);
        message.dispatch(D_OSC_ADDRESS_MF_HUE_OFFSET, cb_command_set_hue_offset);

        // Strobe Multifaders
        message.dispatch(D_OSC_ADDRESS_MF_STROBE_SPEED,     cb_command_set_strobe_speed);
        message.dispatch(D_OSC_ADDRESS_MF_STROBE_DUTY,      cb_command_set_strobe_duty);
        message.dispatch(D_OSC_ADDRESS_MF_STROBE_FADE,      cb_command_set_strobe_fade);
        message.dispatch(D_OSC_ADDRESS_MF_STROBE_OFFSET,    cb_command_set_strobe_offset);

        message.dispatch(D_OSC_ADDRESS_ENCODER_BEAT_OFFSET,    cb_command_set_beat_offset);
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
  uint32_t tSaved_update_remote_parameters = millis();

  static void cb_command_set_hue(OSCMessage &msg) {
    osc_data.hue = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("hue=%f\n\r",osc_data.hue);
    #endif
  }

  static void cb_command_set_hue_speed(OSCMessage &msg) {
    osc_data.hue_speed = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("hue_speed=%f\n\r",osc_data.hue_speed);
    #endif
  }

  static void cb_command_set_hue_offset(OSCMessage &msg) {
    osc_data.hue_offset = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("hue_offset=%f\n\r",osc_data.hue_offset);
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

  static void cb_command_set_beat_offset(OSCMessage &msg) {
    osc_data.beat_offset = msg.getFloat(0);
    #ifdef DEBUG_PRINT_COMMANDS
    Serial.printf("beat_offset=%f\n\r",osc_data.beat_offset);
    #endif
  }


  

};