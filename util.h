/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: util.h
*
* Description: Interface for Utility/Helper functions
*
**************************************************************/

#ifndef _UTIL_H
#define _UTIL_H

#include "extent.h"
#include "dir.h"

// Data from parse path
typedef struct ppinfo {
  directoryEntry *dirParent;  // Parent directory during parsePath
  int isDir;                  // 0 if not dir, 1 if is dir
  int dirIndex;               // -1 if DE does not exist, index if does
  int nextEmptyIndex;         // -1 if no empty index, next empty index if does
} ppinfo;

// Returns minimum blocks needed for amount of bytes
int getBlocksPerByte(int bytes, int blockSize);

// Add strings in reverse order
char *strcatReverse(char *s, char *t);

#endif