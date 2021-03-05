#pragma once

#include "wled.h"

// #include "osc_common.h"

#include <Arduino.h>

struct OSC_Data
{
    // std::vector<OSC_Member> data;
    float hue;
    float hue_speed;

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
#define OSC_HOST_IP 192,168,1,1
#endif
const IPAddress outIp(OSC_HOST_IP); // remote IP (not needed for receive)
const unsigned int outPort = 8000;       // remote port (not needed for receive)
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

      // struct Vertex{
      //   String full_message;
      //   // String parts_message;
      //   struct VALUES{
      //     float x, y, z;
      //   };

      // };

  // std::ostream& operator<<(std::ostream& stream, const Vertex& vertex)
  // {

  // }

  void loop()
  {
    
// char str[] ="LEDJ/multifader_strobe/1";




    if(abs(millis()-tTest)>10){
      tTest = millis();

      if(osc_data.hue++>255){osc_data.hue = 0;}



       OSCMessage msg("/transition_toggle/HueValue");
        msg.add(osc_data.hue);
        Udp.beginPacket(outIp, outPort);
        msg.send(Udp);
        Udp.endPacket();
        msg.empty();

    // std::vector<String> route;

    // route.push_back("LEDJ");
    // route.push_back("multifader_strobe");
    // route.push_back("1");

    // float value = 1.0;
    
    // OSC_Member new_member = {route, value};

    // set_osc_data(new_member, osc_data);


      // std::vector<VALUES> value;

      // value.push_back({1,2,3});

      // for (int i=0;i<value.size();i++){

      //     Serial.println(value[i].x);

      // }



//       char buffer1[150]; memset(&buffer1, 0, sizeof(buffer1));
//       char buffer2[150]; memset(&buffer2, 0, sizeof(buffer2));
//       char buffer3[150]; memset(&buffer3, 0, sizeof(buffer3));


    
//     //  char str[] ="- This, a sample string.";
//      char str[] ="LEDJ/multifader_strobe/1";
//      ///LEDJ/multifader_strobe/1
//   char * pch;
//   printf ("Splitting string \"%s\" into tokens:\n",str);
//   pch = strtok (str,"/");

//   struct OUT{

//     struct PARSED_MESSAGE{
//       char parts[50] = {0};
//     }address[3];

//     struct FLOATS{
      

//     }value;



//   }packet;


//   uint8_t index = 0;

//   while (pch != NULL)
//   {
//     snprintf(address[index++].parts,50,pch);

//     // strtok modifies the original string, placing NULL in the token position
//     pch = strtok (NULL, "/");



//     // sprintf(buffer2,"%s",pch);
//     // printf ("%s\n",buffer2);
//   }

// for(int index=0;index<3;index++){
//   Serial.printf("address[%d].parts=\"%s\"\n\r", index, address[index].parts);
// }

//   // Serial.printf("buffer1=\"%s\"\n\r", buffer1);
//   // Serial.printf("buffer2=\"%s\"\n\r", buffer2);
//   // Serial.printf("buffer3=\"%s\"\n\r", buffer3);


}


    if (WLED_CONNECTED)
    {

      listen_osc_receive();
      // splash_udp_packet_serial();

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
        parse_oscmessage_commands_1(&msg); // Test direct
        // parse_oscmessage_commands_2(&msg); // Vectored
      }
      else
      {
        #ifdef DEBUG_USERMOD_OSC
          DEBUG_PRINTLN("msg.hasError()");
        #endif // DEBUG_USERMOD_OSC
      }
    }
  }

  /**
   * Test parser, search for topics then set controls
   **/
  void parse_oscmessage_commands_1(OSCMessage* msg){

    Serial.println("parse_oscmessage_commands_1");

    uint8_t address_fullpath_max_length = 200;
    char address_fullpath_ctr[address_fullpath_max_length];

    Serial.printf("size=%d\n\r", msg->size());
    // Serial.printf("getDataLength=%d\n\r",msg.getDataLength());
    // Serial.printf("getInt0=%d\n\r",msg.getInt(0));
    // Serial.printf("getInt1=%d\n\r",msg.getInt(1));
    
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

    msg->getAddress(buffer, 0, address_fullpath_max_length);

    #ifdef ENABLE_DEBUG_OSC_LOW
    Serial.printf("buffer=%s",buffer);
    #endif

    char slider_name_hue[] ="/LEDJ/multifader_hue/1";
    char slider_name_hue_speed[] ="/LEDJ/mf=hue-spd-off-/1"; // hue_speed
    char slider_name_hue_offset[] ="/LEDJ/multifader=hue?speed-offset/2"; // hue_offset

//     /HueSpeed_HueDuty_HueFade_HueOffset/1 

// strtok("/_")

    // HueSpeed

    // HueDuty

    // pointer, 8 bytes 

    // if(HueSpeed ){


    // }
    // if(Hue)

    // if(strcmp(buffer,slider_name_hue)==0){ 

    //   Serial.printf("MATCHED \"%s\" with \"%s\" \n\r",buffer,slider_name_hue);
    //   osc_data.hue_speed = msg->getFloat(0);
    //   Serial.println(osc_data.hue_speed);

    // }
    // else
    // if(strcmp(buffer,slider_name_hue_speed)==0){ 

    //   Serial.printf("MATCHED \"%s\" with \"%s\" \n\r",buffer,slider_name_hue_speed);
    //   osc_data.hue_speed = msg->getFloat(0);
    //   Serial.println(osc_data.hue_speed);

    // }else
    // {

    //   Serial.printf("NOMATCH of \"%s\"\n\r",buffer);

    // }

    // Make temp copy, as strtok modifies the buffer by replacing tokens with nulls
    char address_copy[100];
    memset(&address_copy, 0, sizeof(address_copy));
    msg->getAddress(address_copy, 0, 100);

    // Break it down, lets find the number of sliders first 
    char* tok_p = strtok (address_copy,"#");
    uint8_t input_num = 0;

    if(tok_p != NULL){

      // Skip name section to move to number, fix this later
      tok_p = strtok (NULL,"#");
      // tok_p = strtok (address_copy,"#");
      // "/1"
      tok_p++;

      Serial.println(tok_p);

      // Serial.println("tok_p != null");


      if(isdigit(*tok_p)){
        // int(*tok_p);
        Serial.println("isdigit(*tok_p)");

        input_num = atoi(tok_p);

        Serial.printf("input_num=%d\n\r",input_num);


      }

      
    }


    msg->getAddress(address_copy, 0, 100);

    tok_p = strtok (address_copy,"/_#");
    for(int i = 0; i < input_num + 1; i++)
    {
      tok_p = strtok (NULL, "/_#");
    }

    Serial.println(tok_p);

    float value = msg->getFloat(0);

    if(strcmp(tok_p, "HueSpeed") == 0) osc_data.hue_speed = value;

    osc_data.print();

    // osc_data.hue_speed = msg->getFloat(0);
    //   Serial.println(osc_data.hue_speed);

    // if 0, no sliders, treat as single command (pass by pointer and its length)

    // else (>0), handle as sliders, get pointer to relevant command and its length





    // while (tok_p != NULL)
    // {
    //   // snprintf(address[index++].parts,50,pch);

    //   Serial.println(tok_p);

    //   // strtok modifies the original string, placing NULL in the token position
    //   tok_p = strtok (NULL, "/");

    //   // sprintf(buffer2,"%s",pch);
    //   // printf ("%s\n",buffer2);
    // }





    
    // struct PARSED_MESSAGE{
    //   char parts[50] = {0};
    // }address[3];
    
    // char * pch;
    // pch = strtok (address_copy,"/");

    // uint8_t index = 0;
    // uint16_t token_position_index[10];

    // while (pch != NULL)
    // {
    //   snprintf(address[index++].parts,50,pch);

    //   // strtok modifies the original string, placing NULL in the token position
    //   pch = strtok (NULL, "/");

    //   // sprintf(buffer2,"%s",pch);
    //   // printf ("%s\n",buffer2);
    // }




  //    char str[] ="- This, a sample string.";
  // char * pch;
  // printf ("Splitting string \"%s\" into tokens:\n",str);
  // pch = strtok (str," ,.-");
  // while (pch != NULL)
  // {
  //   printf ("%s\n",pch);
  //   pch = strtok (NULL, " ,.-");
  // }
  // // return 0;



    
    //  char str[] ="- This, a sample string.";
    //  char str[] ="LEDJ/multifader_strobe/1";
     ///LEDJ/multifader_strobe/1
//   char * pch;
//   // printf ("Splitting string \"%s\" into tokens:\n",buffer);
//   // pch = strtok (buffer,"/");

//   // struct OUT{

//   //   struct PARSED_MESSAGE{
//   //     char parts[50] = {0};
//   //   }address[3];

//   //   struct FLOATS{
      

//   //   }value;



//   // }packet;




//   uint8_t index = 0;

//   while (pch != NULL)
//   {
//     snprintf(packet.address[index++].parts,50,pch);

//     // strtok modifies the original string, placing NULL in the token position
//     pch = strtok (NULL, "/");



//     // sprintf(buffer2,"%s",pch);
//     // printf ("%s\n",buffer2);
//   }

// for(int index=0;index<3;index++){
//   Serial.printf("address[%d].parts=\"%s\"\n\r", index, packet.address[index].parts);
// }





    // Serial.printf("getInt=%d\n\r",msg.getInt());

    // msg.dispatch("/led", led);

  }

  
  /**
   * Test parser, including vectoring
   **/
  void parse_oscmessage_commands_2(OSCMessage* msg){

    Serial.println("msg WORKING2");

    Serial.printf("size=%d\n\r", msg->size());
    // Serial.printf("getDataLength=%d\n\r",msg.getDataLength());
    // Serial.printf("getInt0=%d\n\r",msg.getInt(0));
    // Serial.printf("getInt1=%d\n\r",msg.getInt(1));
    
    // OSC_Member member1 = {"hello", 12.34};

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
    
    uint32_t tTest = millis();

  //Private class members. You can declare variables and functions only accessible to your usermod here

    // void set_osc_data(OSC_Member new_member, OSC_Data& old_data)
    // {
    //   // for(auto& new_member : new_data.data)
    //   // {
    //     bool found = false;
    //     Serial.println("new_data.data");

    //     for(auto& old_member : old_data.data)
    //     {
    //       Serial.println("auto& old_member : old_data.data");
    //       if(old_member.route == new_member.route)
    //       {
    //         Serial.println("old_member.name == new_member.name");
    //         old_member.value = new_member.value;
    //         found = true;
    //       }
    //     }

    //     if(found == false)
    //     {
    //       old_data.data.emplace_back(new_member);
    //       //old_data.data.pus_back({new_data.data.route, new_data.data.value})
    //     // Serial.println("old_data.data.push_back(new_data.data)");
    //     // Serial.println(new_data.data);
    //     }
    //   //}
    // };

    void update_osc_data(OSC_Data& data)
    {

    };

};