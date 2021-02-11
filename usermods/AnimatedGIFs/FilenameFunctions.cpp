/*
 * Animated GIFs Display Code for SmartMatrix and 32x32 RGB LED Panels
 *
 * This file contains code to enumerate and select animated GIF files by name
 *
 * Written by: Craig A. Lindley
 * Refactored by: Louis Beaudoin
 */

#include "FilenameFunctions.h"
#include <FS.h>

#define DEBUG_DISPLAY_NON_GIF_FILENAMES 0

File file;

bool fileSeekCallback(unsigned long position) {
  return file.seek(position);
}

unsigned long filePositionCallback(void) {
  return file.position();
}

int fileReadCallback(void) {
  return file.read();
}

int fileReadBlockCallback(void * buffer, int numberOfBytes) {
  return file.read((uint8_t*)buffer, numberOfBytes);
}

int fileSizeCallback(void) {
  return file.size();
}

bool isAnimationFile(const char filename []) {
  String filenameString(filename);

  #if defined(ESP32)
    // ESP32 filename includes the full path, so need to remove the path before looking at the filename
    int pathindex = filenameString.lastIndexOf("/");
    if(pathindex >= 0)
        filenameString.remove(0, pathindex + 1);
  #endif

  if ((filenameString[0] == '_') || (filenameString[0] == '~') || (filenameString[0] == '.')) {
      return false;
  }

  filenameString.toUpperCase();
  if (filenameString.endsWith(".GIF") != 1)
      return false;

  return true;
}

// Enumerate and possibly display the animated GIF filenames in GIFS directory, optional startingIndex just used for Serial.print
int enumerateGIFFiles(FS &fs, const char *directoryName, bool displayFilenames, const char *fsName, int startingIndex) {
  int numberOfFiles = 0;

  File directory = fs.open(directoryName);
  File file;

  if (!directory) {
    return -1;
  }

  while (file = directory.openNextFile()) {
    if (isAnimationFile(file.name())) {
      if (displayFilenames) {
        Serial.print(numberOfFiles + startingIndex);
        Serial.print(" - ");
        if(fsName) {
          Serial.print(fsName);
          Serial.print(":");
        }
        Serial.print(file.name());
        Serial.print("    size:");
        Serial.println(file.size());
      }
      numberOfFiles++;
    } else if (displayFilenames && DEBUG_DISPLAY_NON_GIF_FILENAMES) {
      if(fsName) {
        Serial.print(fsName);
        Serial.print(": ");
      }
      Serial.println(file.name());
    }

    file.close();
  }

  directory.close();

  return numberOfFiles;
}

// Get the full path/filename of the GIF file with specified index
void getGIFFilenameByIndex(FS &fs, const char *directoryName, int index, char *pnBuffer) {
  File directory = fs.open(directoryName);
  if (!directory)
    return;

  while ((index >= 0)) {
    file = directory.openNextFile();
    if (!file) break;

    if (isAnimationFile(file.name())) {
      index--;

      // Copy the directory name into the pathname buffer			
      strcpy(pnBuffer, directoryName);

      //ESP32 SD Library includes the full path name in the filename, so no need to add the directory name
      #if defined(ESP32)
        pnBuffer[0] = 0;
      #else
        int len = strlen(pnBuffer);
        if (len == 0 || pnBuffer[len - 1] != '/') strcat(pnBuffer, "/");
      #endif

      // Append the filename to the pathname
      strcat(pnBuffer, file.name());
    }

    file.close();
  }

  file.close();
  directory.close();
}

int openGifFilenameByIndex(FS &fs, const char *directoryName, int index, bool displayFilename, const char *fsName) {
  char pathname[255];

  getGIFFilenameByIndex(fs, directoryName, index, pathname);
  
  if(displayFilename) {
    Serial.print("Pathname: ");
    if(strlen(fsName)) {
      Serial.print(fsName);
      Serial.print(":");
    }
    Serial.println(pathname);
  }

  if(file)
    file.close();

  // Attempt to open the file for reading
  file = fs.open(pathname);
  if (!file) {
    if(displayFilename)
      Serial.println("Error opening GIF file");
    return -1;
  }

  return 0;
}
