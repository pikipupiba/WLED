# 1 "C:\\Users\\micha\\AppData\\Local\\Temp\\tmpsblp2y6s"
#include <Arduino.h>
# 1 "C:/Users/micha/NextCloud/Personal Documents/Project/GitHub/WLED_Colab/WLED/wled00/wled00.ino"
# 14 "C:/Users/micha/NextCloud/Personal Documents/Project/GitHub/WLED_Colab/WLED/wled00/wled00.ino"
#include "wled.h"
void setup();
void loop();
#line 16 "C:/Users/micha/NextCloud/Personal Documents/Project/GitHub/WLED_Colab/WLED/wled00/wled00.ino"
void setup() {
  WLED::instance().setup();
}

void loop() {
  WLED::instance().loop();
}