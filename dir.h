/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: dir.h
*
* Description: Interface of Directory and DE functions
*
**************************************************************/

#ifndef _DIR_H
#define _DIR_H

#include <time.h>

#include "extent.h"

#define FILENAMESIZE 255
#define STARTDE 50

typedef struct directoryEntry {
  char name[FILENAMESIZE];      // Name of the file, 255 char max
  extent location;              // Keep track of location on disk
  int size;                     // Size of the file in bytes
  int isDirectory;              // 0 for non-dir, and 1 for dir
  time_t timeCreate;            // Time when file was created
  time_t timeMod;               // Time when file was modified
  time_t timeAccess;            // Time when file was accessed
} directoryEntry;

directoryEntry *dirInit(int baseNumDE, directoryEntry *parent);

int dirLoad(directoryEntry *dir); // Load dir into memory
int dirLocate(char *name);        // Lookup entry index in directory
int dirCopyDE(directoryEntry *dest, directoryEntry *source, int index); // Copy DE into Dir
int dirCopyDir(directoryEntry *dest, directoryEntry *source); // Copy dir to dir
int dirDeleteDE(directoryEntry *dir, int index);  // Delete DE within a directory

#endif