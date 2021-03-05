#include "usermod_osc.h"

OSC_Data old_osc_data;




// void OSC_Usermod::poll_osc_receive_dispatch(void)
// {
   
//     OSCMessage message;
//     int size = Udp.parsePacket();

//     if (size > 0)
//     {

//         while (size--)
//         {
//             message.fill(Udp.read());
//         }
//         if (!message.hasError())
//         {
            
//             char buffer[100] = {0};
//             message.getAddress(buffer, 0, 100);
//             Serial.printf("osc_address=\"%s\"\n\r",buffer);

//             // Hue Multifaders
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_HUE_SPEED,  cb_command_set_hue_speed);
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_HUE_OFFSET, cb_command_set_hue_offset);

//             // Strobe Multifaders
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_SPEED,     cb_command_set_strobe_speed);
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_DUTY,      cb_command_set_strobe_duty);
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_FADE,      cb_command_set_strobe_fade);
//             message.dispatch(D_OSC_ADDRESS_MULTIFADER_STROBE_OFFSET,    cb_command_set_strobe_offset);
//         }
//         else
//         {
//             error = message.getError();
//             Serial.print("error: ");
//             Serial.println(error);
//         }
//     }

// }


