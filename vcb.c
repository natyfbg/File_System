/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: vcb.c
*
* Description: Basic File System - Volume Control Block functions
*
**************************************************************/

#include <stdlib.h>

#include "vcb.h"
#include "freespace.h"
#include "fsLow.h"

VCB *vcb;
global *g;

int vcbInit(int blockSize, int blockCount) {

  // Malloc a Block of memory as VCB pointer and LBAread block 0
  vcb = malloc(blockSize);
  LBAread(vcb, 1, 0);

	// Malloc memory for global struct to use its data for the file system
  g = malloc(sizeof(global));

	// Set starting path from root directory
	g->curPath[0] = '/';

  // If signature does not match, format the volume
  if (vcb->signature != VCBSIGNATURE) {
	
	vcb->signature = VCBSIGNATURE;
	vcb->blockSize = blockSize;
	vcb->blockCount = blockCount;

	freeSpaceInit();				// Init free space map
	dirInit(STARTDE, NULL);	// Init root directory
	LBAwrite(vcb, 1, 0);		// Write VCB to disk at block[0]
	}

  return 0;
}