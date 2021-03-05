#include "usermod_osc.h"




/***
 * 
 * 
 * 
      if(osc_data.hue++>255){osc_data.hue = 0;}



       OSCMessage msg("/transition_toggle/HueValue");
        msg.add(osc_data.hue);
        Udp.beginPacket(outIp, outPort);
        msg.send(Udp);
        Udp.endPacket();
        msg.empty();


**/

 
  /**
   * Receive osc traffic
   * */
//   void
void OSC_Usermod::listen_osc_receive_1()
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
  void OSC_Usermod::parse_oscmessage_commands_1(OSCMessage* msg){

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



    



    // msg.dispatch("/led", led);

  }

  
  /**
   * Test parser, including vectoring
   **/
  void OSC_Usermod::parse_oscmessage_commands_2(OSCMessage* msg){

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
  void OSC_Usermod::splash_udp_packet_serial()
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