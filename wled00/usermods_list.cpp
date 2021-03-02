#include "wled.h"

#define ENABLE_OSC
/*
 * Register your v2 usermods here!
 *   (for v1 usermods using just usermod.cpp, you can ignore this file)
 */

/*
 * Add/uncomment your usermod filename here (and once more below)
 * || || ||
 * \/ \/ \/
 */
//#include "usermod_v2_example.h"
#ifdef USERMOD_DALLASTEMPERATURE
#include "../usermods/Temperature/usermod_temperature.h"
#endif
//#include "usermod_v2_empty.h"
#ifdef USERMOD_BUZZER
#include "../usermods/buzzer/usermod_v2_buzzer.h"
#endif
#ifdef USERMOD_SENSORSTOMQTT
#include "usermod_v2_SensorsToMqtt.h"
#endif
#if (defined(ENABLE_FL_MAPPING) || defined(ENABLE_FL_WRAPPER))
  #include "../usermods/FastLED_Compatibility/FL_Mapping_Usermod.h"
#endif
#ifdef USERMOD_ANIMATEDGIFS
  #include <SD.h> // needed here to make it onto PlatformIO Dependency Graph, otherwise would only need it inside the Usermod
  #include "../usermods/AnimatedGIFs/usermod_animatedgifs.h"
#endif
#ifdef ENABLE_OSC
  #include "../usermods/osc/usermod_osc.h"
#endif


void registerUsermods()
{
/*
   * Add your usermod class name here
   * || || ||
   * \/ \/ \/
   */
//usermods.add(new MyExampleUsermod());
#ifdef USERMOD_DALLASTEMPERATURE
  usermods.add(new UsermodTemperature());
#endif
//usermods.add(new UsermodRenameMe());
#ifdef USERMOD_BUZZER
  usermods.add(new BuzzerUsermod());
#endif
#ifdef USERMOD_SENSORSTOMQTT
  usermods.add(new UserMod_SensorsToMQTT());
#endif
#ifdef ENABLE_FL_MAPPING
  usermods.add(new Usermod_FL_Mapping());
#endif
#ifdef USERMOD_ANIMATEDGIFS
  usermods.add(new AnimatedGifsUsermod());
#endif
#ifdef ENABLE_OSC
  usermods.add(new OSC_Usermod());
#endif
}
 