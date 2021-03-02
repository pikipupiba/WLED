#pragma once

#include "wled.h"

#include "osc_common.h"

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

//  #define ENABLE_DEVFEATURE_SHOW_UDP_PACKET
#define DISABLE_OSC_COMMON

char ssid[] = "Skynet2400"; // your network SSID (name)
char pass[] = "af4d8bc9ab"; // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(192, 168, 1, 204); // remote IP (not needed for receive)
const unsigned int outPort = 9999;       // remote port (not needed for receive)
const unsigned int localPort = 7000;     // local port to listen for UDP packets (here's where we send the packets)

//  #ifndef ENABLE_DEVFEATURE_SHOW_UDP_PACKET
// #include <OSCMessage.h>
// #include <OSCBundle.h>
// #include <OSCData.h>

#include "src/dependencies/OSC/OSCMessage.h"
#include "src/dependencies/OSC/OSCBundle.h"
#include "src/dependencies/OSC/OSCData.h"

OSCErrorCode error;
unsigned int ledState = LOW; // LOW means led is *on*
//  #endif// ENABLE_DEVFEATURE_SHOW_UDP_PACKET

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
#define BUILTIN_LED 13
#endif
#endif

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

    if (WLED_CONNECTED)
    {

      listen_osc_receive();

      // #ifndef DISABLE_OSC_COMMON
      //       OSC_Data new_osc_data;

      //       set_animation_parameters(new_osc_data, osc_data);
      // #endif // DISABLE_OSC_COMMON
    }
  }
  void connected()
  {
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("UDP server on port %d\n", localPort);
    Udp.begin(localPort);
  }

  /**
   * Receive osc traffic
   * */
  void listen_osc_receive()
  {

    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();

    // If there is data
    if (packetSize > 0)
    {
      
      // Read in data to the OSCMessage parser
      OSCMessage msg;
      uint32_t udp_timeout_millis = millis();
      while (packetSize--)
      {
        #ifdef DEBUG_USERMOD_OSC
          DEBUG_PRINTF("packetSize=%d\n\r",packetSize);
        #endif // DEBUG_USERMOD_OSC
        msg.fill(Udp.read());
        if(abs(millis()-udp_timeout_millis)>1000){ return; }
      }

      // If message is valid, parse it
      if (!msg.hasError())
      {
        parse_oscmessage_commands(&msg);
      }
      else
      {
        #ifdef DEBUG_USERMOD_OSC
          DEBUG_PRINTLN("msg.hasError()");
        #endif // DEBUG_USERMOD_OSC
      }
    }
  }


  void parse_oscmessage_commands(OSCMessage* msg){

    Serial.println("msg WORKING2");

    Serial.printf("size=%d\n\r", msg->size());
    // Serial.printf("getDataLength=%d\n\r",msg.getDataLength());
    // Serial.printf("getInt0=%d\n\r",msg.getInt(0));
    // Serial.printf("getInt1=%d\n\r",msg.getInt(1));
    
    OSC_Member member1 = {"hello", 12.34};

    // osc_data.data.push_back(member1);


    for (int index = 0; index < msg->size(); index++)
    {

      // Name

      // Value
      if (msg->isFloat(index))
      {
        Serial.println(msg->getFloat(index), 10);
      }
      else
      {
        Serial.println("NOT a float");
      }
    }

    char buffer[300];
    memset(&buffer, 0, sizeof(buffer));

    msg->getAddress(buffer); //,0,msg.getDataLength(0));

    Serial.println(buffer);

    // Serial.printf("getInt=%d\n\r",msg.getInt());

    // msg.dispatch("/led", led);

  }

  /**
   * For debugging, display all incoming USP Packets on serial0
   * */
  void splash_udp_packet_serial()
  {

    int packetSize = Udp.parsePacket();

    if (packetSize)
    {

      // Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
      //               packetSize,
      //               Udp.remoteIP().toString().c_str(), Udp.remotePort(),
      //               Udp.destinationIP().toString().c_str(), Udp.localPort(),
      //               ESP.getFreeHeap());

      // read the packet into packetBufffer
      int n = Udp.read(packetBuffer, _UDP_TX_PACKET_MAX_SIZE);
      packetBuffer[n] = 0;
      //  Serial.printf("Contents:\"%s\"\n\r",packetBuffer);

      Serial.printf("Contents:\"");
      for (int i = 0; i < n; i++)
      {
        Serial.printf("%d|%c|%d\n\r", i, packetBuffer[i], (uint8_t)packetBuffer[i]);
      }
      Serial.println();

      //  Serial.println(packetBuffer);

      // send a reply, to the IP address and port that sent us the packet we received
      // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      // Udp.write(ReplyBuffer);
      // Udp.endPacket();
    } //END splash_udp_packet_serial

  } // END loop

private:
  //Private class members. You can declare variables and functions only accessible to your usermod here

    void set_osc_data(OSC_Data new_data, OSC_Data& old_data)
    {
      for(auto& new_member : new_data.data)
      {
        bool found = false;
        Serial.println("new_data.data");

        for(auto& old_member : old_data.data)
        {
        Serial.println("auto& old_member : old_data.data");
          if(old_member.name == new_member.name)
          {
        Serial.println("old_member.name == new_member.name");
            old_member.value = new_member.value;
            found = true;
          }
        }

        if(found == false)
        {
          // old_data.data.push_back(new_data.data);
        Serial.println("old_data.data.push_back(new_data.data)");
        // Serial.println(new_data.data);
        }
      }
    };

    void update_osc_data(OSC_Data& data)
    {

    };

};