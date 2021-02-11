#ifndef FILENAME_FUNCTIONS_H
#define FILENAME_FUNCTIONS_H

#include <FS.h>

int enumerateGIFFiles(FS &fs, const char *directoryName, bool displayFilenames, const char *fsName=NULL, int startingIndex = 0);
void getGIFFilenameByIndex(FS &fs, const char *directoryName, int index, char *pnBuffer);
int openGifFilenameByIndex(FS &fs, const char *directoryName, int index, bool displayFilename=true, const char *fsName=NULL);

bool fileSeekCallback(unsigned long position);
unsigned long filePositionCallback(void);
int fileReadCallback(void);
int fileReadBlockCallback(void * buffer, int numberOfBytes);
int fileSizeCallback(void);

#endif
