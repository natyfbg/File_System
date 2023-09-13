/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: util.c
*
* Description: Basic File System - Utility/Helper functions
*
**************************************************************/

#include <string.h>

#include "util.h"

int getBlocksPerByte(int bytes, int blockSize) {
  return (bytes + blockSize - 1) / blockSize;
}

char *strcatReverse(char *s, char *t) {
  char *result = strcat(t, s);
  return result;
}