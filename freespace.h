/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: freespace.h
*
* Description: Interface of Free space functions
*
**************************************************************/

#ifndef _FREESPACE_H
#define _FREESPACE_H

#include "extent.h"

int freeSpaceInit();
extent freeSpaceAlloc(int blocksNeeded);
int freeSpaceRelease(int blockNumber, int blockCount);

// Helper free space funcs
int freeShiftDown(int index);
int freeShiftUp(int index);
int checkPre(int index, int blockNumber, int blockCount);
int checkPost(int index, int blockNumber, int blockCount);
int checkMerge(int index, int blockNumber, int blockCount);

#endif