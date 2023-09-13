/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: extent.c
*
* Description: Basic File System - Extent functions
*
**************************************************************/

#include "extent.h"

int extentCopy(extent dest, extent source) {

  dest.blockNumber = source.blockNumber;
  dest.blockCount = source.blockCount;

  return 0;
}