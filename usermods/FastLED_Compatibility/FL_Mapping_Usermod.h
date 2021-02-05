#pragma once

#include "wled.h"

#ifdef ENABLE_FL_WRAPPER
  int NUM_LEDS;
  CRGB * leds;
  void * flStructPtr;
  bool fl_usingXY;
#endif

#ifdef ENABLE_FL_MAPPING


segmentmap _segmentmaps[MAX_NUM_SEGMENTS];

uint16_t kMatrixWidth;
uint16_t kMatrixHeight;

// XY is always set to progressive mode (all rows have incrementing index from left to right), mapping is done later
// This function can be used in an effect, and can be used by FastLED's blur2d function
uint16_t XY( uint8_t x, uint8_t y) { return (y * kMatrixWidth) + x; }

const char *tempfilename = "/map_temp";

const uint16_t mapScaleMax = 255;
const uint16_t mapScaleMin = 0;
// TODO: switch to static jsonDoc, and optimize capacity
const int jsonDocCapacity = 200;

//#define JSON_MAP_DEBUG

bool getFXYFromArray(fXyPair &pair, JsonArray &array) {
  if((!array[0].is<long>() && !array[0].is<float>()) || 
    (!array[1].is<long>() && !array[1].is<float>())) {
    // unexpected type we can't handle
    #ifdef JSON_MAP_DEBUG
      Serial.println("unexpected type");
    #endif
    return false;
  }

  pair.x = array[0].as<float>();
  pair.y = array[1].as<float>();

  return true;
}

bool getXYFromArray(xyPair &pair, JsonArray &array) {
  if(!array[0].is<int>() || !array[1].is<int>()) {
    // unexpected type we can't handle
    #ifdef JSON_MAP_DEBUG
      Serial.println("unexpected type");
    #endif
    return false;
  }

  pair.x = array[0].as<int>();
  pair.y = array[1].as<int>();

  return true;
}

bool transposeJsonArray(File &file, File &outputfile, float translateX, float translateY, float scaleX, float scaleY) {
  DynamicJsonDocument doc(jsonDocCapacity);
  DynamicJsonDocument outputdoc(jsonDocCapacity);

  bool firstEntry = true;

  // deserialize coordinate arrays within "coordinates" array one element at a time to save RAM
  do {
    // Deserialize the next array element
    auto err = deserializeJson(doc, file);
    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return 0;
    }

    JsonArray array2 = doc.as<JsonArray>();
  
    fXyPair pair;
    float x, y;

    bool result = getFXYFromArray(pair, array2);

    if(!result) {
      Serial.println("can't getFXYFromArray");
      return 0;
    }

    x = pair.x;
    y = pair.y;

#if 0
    Serial.print("[");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.println("]");
#endif

    outputdoc[0] = (uint16_t)((x+translateX)*scaleX);
    outputdoc[1] = (uint16_t)((y+translateY)*scaleY);

    // we need a comma except for the first entry
    if(!firstEntry)
      outputfile.print(",");
    else
      firstEntry = false;

    serializeJson(outputdoc, outputfile);
  } while (file.findUntil(",", "]"));  

  return true;
}

bool cleanupMap(File &file, const char *outfilename) {
  float fMaxX = -std::numeric_limits<float>::max();
  float fMaxY = -std::numeric_limits<float>::max();
  float fMinX = std::numeric_limits<float>::max();
  float fMinY = std::numeric_limits<float>::max();

  int numElements = 0;

  // keep track if an update is needed, and we always update if writing to a new file
  bool fileUpdateNeeded = strcmp(file.name(), outfilename);

  DynamicJsonDocument doc(jsonDocCapacity);

  // TODO: optimize memory usage
  StaticJsonDocument<200> filter;
  filter["minVirtualScreenWidth"] = true;
  filter["minVirtualScreenHeight"] = true;
  filter["idealScaleX"] = true;
  filter["idealScaleY"] = true;
  filter["numElements"] = true;

  auto err = deserializeJson(doc, file, DeserializationOption::Filter(filter));
  if (err) {
    #ifdef JSON_MAP_DEBUG
      Serial.print(F("filter deserializeJson() failed with code "));
      Serial.println(err.c_str());
    #endif
    file.close();
    return 0;
  }

  int existingNumElements = doc["numElements"].as<int>();
  int minVirtualScreenWidth = doc["minVirtualScreenWidth"].as<int>();
  int minVirtualScreenHeight = doc["minVirtualScreenHeight"].as<int>();

  #ifdef JSON_MAP_DEBUG
    Serial.print("minVirtualScreenWidth: ");
    Serial.println(minVirtualScreenWidth);
    Serial.print("minVirtualScreenHeight: ");
    Serial.println(minVirtualScreenHeight);
  #endif

  float idealScaleX = doc["idealScaleX"].as<float>();
  float idealScaleY = doc["idealScaleY"].as<float>();
  #ifdef JSON_MAP_DEBUG
    Serial.print("idealScaleX: ");
    Serial.println(idealScaleX);
    Serial.print("idealScaleY: ");
    Serial.println(idealScaleY);
  #endif

  // if minVirtualScreenWidth/minVirtualScreenHeight is specified, idealScaleX/idealScaleY should be too
  if((minVirtualScreenWidth || minVirtualScreenHeight) && (!idealScaleX || !idealScaleY))
    fileUpdateNeeded = true;

  bool arrayfound = false;

  // go back to start of file, look for start of "coordinates" array
  file.seek(0);
  arrayfound = file.find("\"coordinates\":[");

  if(!arrayfound) {
    // go back to start of file, look for any array
    file.seek(0);
    arrayfound = file.find("[");
  }

  if(!arrayfound) {
    #ifdef JSON_MAP_DEBUG
      Serial.println("can't find array in JSON");
    #endif
    file.close();
    return 0;
  }

  do {
    // Deserialize the next array element
    auto err = deserializeJson(doc, file);
    if (err) {
      #ifdef JSON_MAP_DEBUG
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
      #endif
      file.close();
      return 0;
    }

    JsonArray array2 = doc.as<JsonArray>();
  
    fXyPair pair;
    float x, y;

    bool result = getFXYFromArray(pair, array2);

    if(!result){
      #ifdef JSON_MAP_DEBUG
        Serial.println("can't getFXYFromArray");
      #endif
      file.close();
      return 0;
    }

    x = pair.x;
    y = pair.y;

    if(x < fMinX)
      fMinX = x;
    if(x > fMaxX)
      fMaxX = x;\
    if(y < fMinY)
      fMinY = y;
    if(y > fMaxY)
      fMaxY = y;

    #ifdef JSON_MAP_DEBUG
      Serial.print("[");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.println("]");
    #endif

    numElements++;
  } while (file.findUntil(",", "]"));

  if(existingNumElements != numElements)
    fileUpdateNeeded = true;

  #ifdef JSON_MAP_DEBUG
    Serial.print("Found ");
    Serial.print(numElements);
    Serial.print(" elements ");
    Serial.print(" from range [");
    Serial.print(fMinX);
    Serial.print(",");
    Serial.print(fMinY);
    Serial.print("] to [");
    Serial.print(fMaxX);
    Serial.print(",");
    Serial.print(fMaxY);
    Serial.println("]");
  #endif

  float scaleX = (fMaxX - fMinX) ? (float)mapScaleMax/(fMaxX - fMinX) : 1;
  float translateX = (float)mapScaleMin - fMinX;
  float scaleY = (fMaxY - fMinY) ? (float)mapScaleMax/(fMaxY - fMinY) : 1;
  float translateY = (float)mapScaleMin - fMinY;

  #ifdef JSON_MAP_DEBUG
    Serial.print("Need to scale X by: ");
    Serial.print(scaleX);
    Serial.print(", and translate X by: ");
    Serial.println(translateX);

    Serial.print("Need to scale Y by: ");
    Serial.print(scaleY);
    Serial.print(", and translate Y by: ");
    Serial.println(translateY);
  #endif

  // if the coordinates aren't in the right format, update the file
  if(scaleX != 1.0 || scaleY != 1.0 || translateX || translateY)
    fileUpdateNeeded = true;

  if(!minVirtualScreenWidth && !minVirtualScreenHeight && idealScaleX && idealScaleY) {
    #ifdef JSON_MAP_DEBUG
      Serial.println("calculating minVirtualScreenWidth/Height");
    #endif

    // calculate these values based on the max seen in the data, as mapScaleMax could be different from when the file was generated
    minVirtualScreenWidth = round(idealScaleX*(fMaxX - fMinX));
    minVirtualScreenHeight = round(idealScaleY*(fMaxY - fMinY));

    #ifdef JSON_MAP_DEBUG
      Serial.print("minVirtualScreenWidth: ");
      Serial.println(minVirtualScreenWidth);
      Serial.print("minVirtualScreenHeight: ");
      Serial.println(minVirtualScreenHeight);
    #endif

    fileUpdateNeeded = true;
  }

  if(fileUpdateNeeded) {
    #ifdef JSON_MAP_DEBUG
      Serial.println("file update needed");
    #endif

    // in case temp file already exists, remove
    LITTLEFS.remove(tempfilename);

    // open a second file, we'll translate one file to the other
    File outputfile = LITTLEFS.open(tempfilename, "w");

    if(!tempfilename){
        #ifdef JSON_MAP_DEBUG
          Serial.println("- failed to open file for writing");
        #endif
        file.close();
        return 0;
    }

    DynamicJsonDocument outputdoc(jsonDocCapacity);

    outputdoc["numElements"] = numElements;

    if(minVirtualScreenWidth && minVirtualScreenHeight) {
      idealScaleX = (float)minVirtualScreenWidth/(float)mapScaleMax;
      idealScaleY = (float)minVirtualScreenHeight/(float)mapScaleMax;

      #ifdef JSON_MAP_DEBUG
        Serial.print("idealScaleX = ");
        Serial.println(idealScaleX, 8);
        Serial.print("idealScaleY = ");
        Serial.println(idealScaleY, 8);
      #endif

      outputdoc["idealScaleX"] = idealScaleX;
      outputdoc["idealScaleY"] = idealScaleY;
      outputdoc["minVirtualScreenWidth"] = minVirtualScreenWidth;
      outputdoc["minVirtualScreenHeight"] = minVirtualScreenHeight;
    }

    serializeJson(outputdoc, outputfile);
    outputdoc.clear();

    // backup one character in the file to replace premature '}' with comma
    outputfile.seek(outputfile.position() - 1);
    outputfile.print(",");

    outputfile.print("\"coordinates\":[");

    // start over now that we know the min/max and number of elements from the array

    // go back to start of file, look for start of "coordinates" array
    file.seek(0);
    arrayfound = file.find("\"coordinates\":[");

    if(!arrayfound) {
      // go back to start of file, look for any array
      file.seek(0);
      arrayfound = file.find("[");
    }

    if(!arrayfound) {
      #ifdef JSON_MAP_DEBUG
        Serial.println("can't find array in JSON");
      #endif

      file.close();
      outputfile.close();
      return 0;
    }

    if(!transposeJsonArray(file, outputfile, translateX, translateY, scaleX, scaleY)) {
      #ifdef JSON_MAP_DEBUG
        Serial.println("transposeJsonArray failed");
      #endif

      file.close();
      outputfile.close();
      return 0;
    }

    outputfile.print("]");

    if(idealScaleX && idealScaleY) {
      outputfile.print(",");
      outputfile.print("\"idealCoordinates\":[");

      // go back to start of file, look for start of "coordinates" array
      file.seek(0);
      arrayfound = file.find("\"coordinates\":[");

      if(!arrayfound) {
        // go back to start of file, look for any array
        file.seek(0);
        arrayfound = file.find("[");
      }

      if(!arrayfound) {
        #ifdef JSON_MAP_DEBUG
          Serial.println("can't find array in JSON");
        #endif

        file.close();
        outputfile.close();
        return 0;
      }

      if(!transposeJsonArray(file, outputfile, translateX, translateY, idealScaleX/scaleX, idealScaleY/scaleY)) {
        #ifdef JSON_MAP_DEBUG
          Serial.println("transposeJsonArray failed");
        #endif

        file.close();
        outputfile.close();
        return 0;
      }
      outputfile.print("]");
    }

    outputfile.print("}");
    outputfile.close();
    file.close();

    // in case output file already exists, remove
    LITTLEFS.remove(outfilename);

    // rename tempfile to outfilename
    LITTLEFS.rename(tempfilename, outfilename);

    // remove temp file
    LITTLEFS.remove(tempfilename);
  } else {
    #ifdef JSON_MAP_DEBUG
      Serial.println("no file update needed");
    #endif
  }

  return true;
}

bool cleanupMap(const char *filename, const char *outfilename) {
  // Open file for reading
  File file = LITTLEFS.open(filename, "r");

  // This may fail if the file is missing
  if (!file) {
    #ifdef JSON_MAP_DEBUG
      Serial.println(F("Failed to open file"));
    #endif
    return false;
  }

  return cleanupMap(file, outfilename);
}

int cleanupMapsWithPrefix(const char *inputprefix, const char *outputprefix) {
  int numberOfFiles = 0;

  File directory = LITTLEFS.open("/");
  File file;

  if (!directory) {
      return -1;
  }

  while (file = directory.openNextFile()) {
    //char outputdir[] = "/outdir/";
    int length = strlen(inputprefix);

    if(strncmp(file.name(), inputprefix, length)) {
      file.close();
      continue;
    }

    char outfilename[50];
    const char * filenameonly = file.name() + length;
    if(!strlen(filenameonly)) {
      file.close();
      directory.close();
      return -1;
    }
    strcpy(outfilename, outputprefix);
    strcat(outfilename, filenameonly);

    #ifdef WLED_DEBUG
      Serial.print("map filename: ");
      Serial.println(outfilename);
    #endif

    // file will be closed within cleanupMap
    bool result = cleanupMap(file, outfilename);
    
    if(result) {
      #ifdef WLED_DEBUG
        Serial.println("cleanupmap: good");
      #endif
      numberOfFiles++;
    } else {
      #ifdef WLED_DEBUG
        Serial.println("cleanupmap: bad");
      #endif
    }
  }

  directory.close();

  return numberOfFiles;
}

int cleanupMapsInDirectory(const char *inputdir, const char *outputdir) {
  int numberOfFiles = 0;

  File directory = LITTLEFS.open(inputdir);
  File file;

  if (!directory) {
      return -1;
  }

  while (file = directory.openNextFile()) {
    //char outputdir[] = "/outdir/";

    char outfilename[50];
    char * filenameonly = strrchr(file.name(), '/');
    if(!strlen(filenameonly)) {
      file.close();
      directory.close();
      return -1;
    }
    strcpy(outfilename, outputdir);
    strcat(outfilename, filenameonly + 1);

    Serial.print("string: ");
    Serial.println(outfilename);

    // file will be closed within cleanupMap
    bool result = cleanupMap(file, outfilename);
    
    if(result) {
      #ifdef WLED_DEBUG
        Serial.println("cleanupmap: good");
      #endif
      numberOfFiles++;
    } else {
      #ifdef WLED_DEBUG
        Serial.println("cleanupmap: bad");
      #endif
    }
  }

  directory.close();

  return numberOfFiles;
}

// note: clears entire _segmentmaps before load
bool loadMapListIntoSegmentmaps() {
  int pos = 0;

  // clear _segmentmaps before load, so any segments without a map are zero'd out
  memset(_segmentmaps, 0x00, sizeof(segmentmap) * MAX_NUM_SEGMENTS);

  // open maplist file
  // Open file for reading
  File file = LITTLEFS.open("/mapnames.json", "r");

  // This may fail if the file is missing
  if (!file) {
    #ifdef JSON_MAP_DEBUG
      Serial.println(F("Failed to open file"));
    #endif
    return 0;
  }

  // TODO: figure out good capacity or optimize to get part of array only
  DynamicJsonDocument doc(1000);

  auto err = deserializeJson(doc, file);
  if (err) {
    #ifdef JSON_MAP_DEBUG
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
    #endif
    file.close();
    return 0;
  }

  for(int i=0; i<MAX_NUM_SEGMENTS; i++) {
    const char* mapname = doc[i];

    if(mapname) {
      int length = strlen(mapname);
      if(!length)
        continue;

      char* newString = (char*)malloc(sizeof(char)*(length+1));

      strcpy(newString, mapname);
      _segmentmaps[i].mapname = newString;

      #ifdef WLED_DEBUG
        Serial.print("got string \"");
        Serial.print(mapname);
        Serial.print("\" in array position ");
        Serial.println(i);
      #endif
    }
  }
  return 1;
}

bool loadMapIntoBuffer(const char *filename, xyPair * xyTablePointer, uint16_t numElements, float scaleX, float scaleY) {
  int pos = 0;

  if(!numElements) return true;

  // Open file for reading
  File file = LITTLEFS.open(filename, "r");

  // This may fail if the file is missing
  if (!file) {
    #ifdef JSON_MAP_DEBUG
      Serial.println(F("Failed to open file"));
    #endif
    return false;
  }

  DynamicJsonDocument doc(jsonDocCapacity);

  bool arrayfound = false;

  // go back to start of file, look for start of "coordinates" or "idealCoordinates" array
  file.seek(0);
  arrayfound = file.find("\"coordinates\":[");

  if(!arrayfound) {
    #ifdef JSON_MAP_DEBUG
      Serial.println("can't find array in JSON");
    #endif
    file.close();
    return 0;
  }

  do {
    // Deserialize the next array element
    auto err = deserializeJson(doc, file);
    if (err) {
      #ifdef JSON_MAP_DEBUG
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
      #endif
      file.close();
      return 0;
    }

    JsonArray array2 = doc.as<JsonArray>();
  
    xyPair pair;

    bool result = getXYFromArray(pair, array2);

    if(!result){
      #ifdef JSON_MAP_DEBUG
        Serial.println("can't getXYFromArray");
      #endif
      file.close();
      return 0;
    }

    xyTablePointer[pos].x = round((float)pair.x * scaleX);
    xyTablePointer[pos].y = round((float)pair.y * scaleY);

    #ifdef JSON_MAP_DEBUG
      Serial.print("[");
      Serial.print(pair.x);
      Serial.print(",");
      Serial.print(pair.y);
      Serial.print("]");
      Serial.print("->[");
      Serial.print(xyTablePointer[pos].x);
      Serial.print(",");
      Serial.print(xyTablePointer[pos].y);
      Serial.println("]");
    #endif

    pos++;
  } while ((pos < numElements) && file.findUntil(",", "]"));

  file.close();

  if(pos < numElements) {
    #ifdef JSON_MAP_DEBUG
      Serial.println("table shorter than numElements");
    #endif

    return false;
  }

  return true;
}

bool getMapDetails(const char *filename, uint16_t &numElements, uint16_t &minVirtualScreenWidth, uint16_t &minVirtualScreenHeight) {
  // Open file for reading
  File file = LITTLEFS.open(filename, "r");

  // This may fail if the file is missing
  if (!file) {
    #ifdef JSON_MAP_DEBUG
      Serial.println(F("Failed to open file"));
    #endif
    return false;
  }

  DynamicJsonDocument doc(jsonDocCapacity);

  StaticJsonDocument<64> filter;
  filter["minVirtualScreenWidth"] = true;
  filter["minVirtualScreenHeight"] = true;
  filter["numElements"] = true;

  auto err = deserializeJson(doc, file, DeserializationOption::Filter(filter));
  if (err) {
    #ifdef JSON_MAP_DEBUG
      Serial.print(F("filter deserializeJson() failed with code "));
      Serial.println(err.c_str());
    #endif
    file.close();
    return 0;
  }

  minVirtualScreenWidth = doc["minVirtualScreenWidth"].as<int>();
  minVirtualScreenHeight = doc["minVirtualScreenHeight"].as<int>();
  numElements = doc["numElements"].as<int>();

  file.close();

  return 1;
}

bool loadMapIntoSegmentBuffer(segmentmap * map, float scaleX, float scaleY) {
  bool result = loadMapIntoBuffer(map->mapname, map->xyTablePointer, map->numElements, scaleX, scaleY);
  return result;
}

bool loadMapDetailsIntoSegment(segmentmap * map) {
  bool result = getMapDetails(map->mapname, map->numElements, map->minVirtualScreenWidth, map->minVirtualScreenHeight);

  // clear variables in unknown state
  if(!result) {
    map->numElements = 0;
    map->minVirtualScreenWidth = 0;
    map->minVirtualScreenHeight = 0;
  }

  return result;
}

class Usermod_FL_Mapping : public Usermod {
  public:
    void setup() {
      int result = cleanupMapsWithPrefix("/map_", "/map_");

      #ifdef WLED_DEBUG
        Serial.print("Found ");
        Serial.print(result);
        Serial.println(" maps");
      #endif

      if(!loadMapListIntoSegmentmaps()) {
        #ifdef WLED_DEBUG
          Serial.println("Couldn't load maps");
        #endif
        return;
      }

      for(int i=0; i<MAX_NUM_SEGMENTS; i++) {
        result = loadMapDetailsIntoSegment(&_segmentmaps[i]);

        if(!result) {
          #ifdef WLED_DEBUG
            Serial.print("Couldn't load map details for segment ");
            Serial.println(i);
          #endif
          continue;
        }

        #ifdef JSON_MAP_DEBUG
          Serial.print("_segmentmaps[");
          Serial.print(i);
          Serial.println("]:");
          Serial.print("- mapname:  ");
          Serial.println(_segmentmaps[i].mapname);
          Serial.print("- numElements: ");
          Serial.println(_segmentmaps[i].numElements);
          Serial.print("- minVirtualScreenWidth: ");
          Serial.println(_segmentmaps[i].minVirtualScreenWidth);
          Serial.print("- minVirtualScreenHeight: ");
          Serial.println(_segmentmaps[i].minVirtualScreenHeight);

        #endif

        _segmentmaps[i].xyTablePointer = (xyPair*)malloc(_segmentmaps[i].numElements * sizeof(xyPair));

        if(!_segmentmaps[i].xyTablePointer) {
          #ifdef WLED_DEBUG
            Serial.println("malloc failed");
          #endif          
          continue;
        }
           
        result = loadMapIntoSegmentBuffer(&_segmentmaps[i],
                    (float)(_segmentmaps[i].minVirtualScreenWidth-1)/mapScaleMax,
                    (float)(_segmentmaps[i].minVirtualScreenHeight-1)/mapScaleMax);

        if(!result)
          continue;
        
        #ifdef JSON_MAP_DEBUG
          Serial.println("loaded");
          for(int j=0; j < _segmentmaps[i].numElements; j++) {
            Serial.print("[");
            Serial.print(_segmentmaps[i].xyTablePointer[j].x);
            Serial.print(",");
            Serial.print(_segmentmaps[i].xyTablePointer[j].y);
            Serial.print("],");
          }
          Serial.println();
        #endif
      }
    }

    void loop() {
      if (millis() - lastTime > 1000) {
        //Serial.println("I'm alive!");
        lastTime = millis();
      }
    }
  private:
    unsigned long lastTime = 0;
};
#endif // #ifdef ENABLE_FL_MAPPING
