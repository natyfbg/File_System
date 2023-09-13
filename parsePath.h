/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: parsePath.h
*
* Description: Interface of Parse path function
*
**************************************************************/

#ifndef _PARSEPATH_H
#define _PARSEPATH_H

#include "util.h"

// Return -1 if path does not exist, 0 if path does exist
int parsePath(char *path, ppinfo *result);

#endif