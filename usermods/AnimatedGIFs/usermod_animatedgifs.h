#pragma once

#include "wled.h"

#include <GifDecoder.h>

#include "animatedgifs_common.h"

// these example GIFs are from the AnimatedGIF library, see that library for the source, and instructions on how to make your own GIF into a C header file
#include "homer_tiny.h"
#include "badgers.h"

// these gifs were made by resizing the original GIFs (source unknown) using ezgif.com and converted to C headers using https://github.com/bitbank2/image_to_c
#include "infinity1.h"
//#include "infinity2.h"
#include "rainbow1.h"
#include "rainbow2.h"

// add gif name and sizeof(gif) to the below lists
const uint8_t * gifsList[] = { rainbow2, rainbow1, infinity1 };
const int gifsSizeList[] = { sizeof(rainbow2), sizeof(rainbow1), sizeof(infinity1) };
//const uint8_t * gifsList[] = { infinity1 };
//const int gifsSizeList[] = { sizeof(infinity1) };

// defaults in case config hasn't been written to cfg.json yet
#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   100
#define AGIF_MATRIX_WIDTH 64
#define AGIF_MATRIX_HEIGHT 64
#define PLAYBACK_MODE playModeFirst

int gifMatrixWidth;
int gifMatrixHeight;
int displayTimeSeconds;
int numFullCycles;
int playMode;
bool playNextGif = true;     // we haven't loaded a GIF yet on first pass through, make sure we do that
int gifIndex = 0;

// decoder has width=320 (max AnimatedGIF library can support), height=lzwMax=0 (doesn't matter), use malloc for buffer
GifDecoder<320, 0, 12, true> decoder;

CRGB * gifMatrixBuffer;

#define USE_SMARTMATRIX         0

void screenClearCallback(void) {
  fill_solid(gifMatrixBuffer, gifMatrixWidth*gifMatrixHeight, CRGB::Black);
}

void updateScreenCallback(void) {
  // don't need to do anything here
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  if(x < gifMatrixWidth && y < gifMatrixHeight)
    gifMatrixBuffer[(y*gifMatrixWidth) + x] = CRGB(red, green, blue);
}

void * agifsGetVirtualScreen(uint16_t width, uint16_t height) {
  kMatrixWidth = gifMatrixWidth;
  kMatrixHeight = gifMatrixHeight;

  return (void*)gifMatrixBuffer;
}

void agifsSetNextGifIndex(int index, bool switchImmediately) {
  gifIndex = index;
  if(switchImmediately)
    playNextGif = true;
}

class AnimatedGifsUsermod : public Usermod {
  public:
    void setup() {
      gifMatrixBuffer = (CRGB *)malloc(sizeof(CRGB) * (gifMatrixWidth * gifMatrixHeight));

      decoder.setScreenClearCallback(screenClearCallback);
      decoder.setUpdateScreenCallback(updateScreenCallback);
      decoder.setDrawPixelCallback(drawPixelCallback);
    }

    void loop() {
      if(!configExists) {
        Serial.println("Agifs: Need to save config");
        configExists = true;
        serializeConfig();
      }

      // these variables keep track of when it's time to play a new GIF
      static unsigned long displayStartTime_millis;
      
      // these variables keep track of when we're done displaying the last frame and are ready for a new frame
      static uint32_t lastFrameDisplayTime = 0;
      static unsigned int currentFrameDelay = 0;

      unsigned long now = millis();

      // default behavior is to play the gif for displayTimeSeconds or for numFullCycles, whichever comes first
      if((playMode == playModeFirst) && ((now - displayStartTime_millis) > (displayTimeSeconds * 1000) || decoder.getCycleNumber() > numFullCycles)) {
        playNextGif = true;

      }
      // alt behavior is to play the gif until both displayTimeSeconds and numFullCycles have passed
      else if((playMode == playModeBoth) && ((now - displayStartTime_millis) > (displayTimeSeconds * 1000) && decoder.getCycleNumber() > numFullCycles)) {
        playNextGif = true;  
      }

      // We only decode a GIF frame if the previous frame delay is over
      if((millis() - lastFrameDisplayTime) > currentFrameDelay) {
        if(playNextGif)
        {
          playNextGif = false;

          // check for valid gifIndex before using
          gifIndex %= sizeof(gifsList)/sizeof(gifsList[0]);

          // start decoding, skipping to the next GIF if there's an error
          if(decoder.startDecoding((uint8_t *)gifsList[gifIndex], gifsSizeList[gifIndex]) < 0) {
            playNextGif = true;
            return;
          }

          // Calculate time in the future to terminate animation
          displayStartTime_millis = now;

          // get the index for the next pass through
          gifIndex++;
        }

        // decode frame without delaying after decode
        int result = decoder.decodeFrame(false);

        lastFrameDisplayTime = now;
        currentFrameDelay = decoder.getFrameDelay_ms();

        // it's time to start decoding a new GIF if there was an error, and don't wait to decode
        if(result < 0) {
          playNextGif = true;
          currentFrameDelay = 0;
        }
      }
    }

    void addToConfig(JsonObject& root)
    {
      JsonObject top = root.createNestedObject("agif");
      top["vsW"] = gifMatrixWidth;
      top["vsH"] = gifMatrixHeight;
      top["dtSec"] = displayTimeSeconds;
      top["numCyc"] = numFullCycles;
      top["playMode"] = playMode;
    }

    void readFromConfig(JsonObject& root)
    {
      JsonObject top = root["agif"];

      // If not all of the keys are present in the config, set a flag to write them later
      configExists = root.containsKey("agif") &&
                      top.containsKey("vsW") &&
                      top.containsKey("vsH") &&
                      top.containsKey("dtSec") &&
                      top.containsKey("numCyc") &&
                      top.containsKey("playMode");

      gifMatrixWidth = top["vsW"] | AGIF_MATRIX_WIDTH; //The value right of the pipe "|" is the default value in case your setting was not present in cfg.json (e.g. first boot)
      gifMatrixHeight = top["vsH"] | AGIF_MATRIX_HEIGHT;
      displayTimeSeconds = top["dtSec"] | DISPLAY_TIME_SECONDS;
      numFullCycles = top["numCyc"] | NUMBER_FULL_CYCLES;
      playMode = top["playMode"] | PLAYBACK_MODE;
    }

  private:
    //Private class members. You can declare variables and functions only accessible to your usermod here
    unsigned long lastTime = 0;
    bool configExists;
};
