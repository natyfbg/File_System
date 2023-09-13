/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: extent.h
*
* Description: Interface of Extent functions
*
**************************************************************/

#ifndef _EXTENT_H
#define _EXTENT_H

#define STARTEXTENT 6400

typedef struct extent {
	int blockNumber;		// Location of starting block
	int blockCount;			// Number of blocks starting from blockNumber
} extent;

// Copy extent values from source to destination
int extentCopy(extent dest, extent source);

#endif