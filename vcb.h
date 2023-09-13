/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: vcb.h
*
* Description: Interface for Volume Control Block functions
*
**************************************************************/

#ifndef _VCB_H
#define _VCB_H

#include "extent.h"
#include "dir.h"

#define VCBSIGNATURE 1234
#define MAXPATHSIZE 255

// Keep track of these variables globally for file system
typedef struct global {
  extent *freeSpaceMap;       // Free space map
  directoryEntry *loadedDir;  // Loaded directory in memory
  directoryEntry *rootDir;    // Root directory in memory
  int freeSpaceExtentCount;   // # of total extents in free space map
  int actualDECount;          // True DE # after byte optimization
  char curPath[MAXPATHSIZE];  // Current path or working directory
} global;

typedef struct VCB {
  int signature;          // Name/ID for VCB. Value is "VCBSIGNATURE"
  int blockSize;          // Size of blocks in bytes in volume
  int blockCount;         // Number of blocks in volume
  extent freeSpaceMapLoc; // Free space map location
  directoryEntry rootDir; // Root Directory Entry
} VCB;

// Global VCB variable is declared in vcb.c
extern VCB *vcb;
extern global *g;

int vcbInit(int blockSize, int blockCount);

#endif