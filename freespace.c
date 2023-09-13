/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: freespace.c
*
* Description: Basic File System - Free space functions
*
**************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "freespace.h"
#include "vcb.h"
#include "fsLow.h"
#include "util.h"

int freeSpaceInit() {

	// Find bytes needed to malloc and blocksNeeded
	int bytesNeeded = STARTEXTENT * sizeof(extent);
	int blocksNeeded = getBlocksPerByte(bytesNeeded, vcb->blockSize);
	int bytesUsed = blocksNeeded * vcb->blockSize;

	// Store free space map starting at Block 1
	vcb->freeSpaceMapLoc.blockNumber = 1;
	vcb->freeSpaceMapLoc.blockCount = blocksNeeded;

	g->freeSpaceMap = malloc(bytesUsed);
	g->freeSpaceExtentCount = blocksNeeded / sizeof(extent);

	// Set default values to free space map
	for (int i = 1; i < g->freeSpaceExtentCount; i++) {
		g->freeSpaceMap[i].blockNumber = 0;
		g->freeSpaceMap[i].blockCount = 0;
	}

	// Mark VCB and free space map as used [0 to totalBlocks - blocksNeeded - 1]
	g->freeSpaceMap[0].blockNumber = blocksNeeded + 1;
	g->freeSpaceMap[0].blockCount = vcb->blockCount - blocksNeeded - 1;

	// Write free space map to disk starting from Block 1
	LBAwrite(g->freeSpaceMap, blocksNeeded, 1);

	return 0;
}

extent freeSpaceAlloc(int blockCount) {

	extent location;
	for (int i = 0; i < g->freeSpaceExtentCount; i++) {

		if (g->freeSpaceMap[i].blockCount >= blockCount) {

			// check if the allocated blocks would overlap with other extents, 0 for false, 1 for true
			int end = g->freeSpaceMap[i].blockNumber + g->freeSpaceMap[i].blockCount;
			int overlap = 0;

			for (int j = 0; j < g->freeSpaceExtentCount; j++) {

      	if (j != i && g->freeSpaceMap[j].blockNumber < end && 
					g->freeSpaceMap[j].blockNumber + g->freeSpaceMap[j].blockCount 
					> g->freeSpaceMap[i].blockNumber) {
        
					overlap = 1;
        	break;
      	}
    	}

    	if (!overlap) {
		
				// allocate blocks from this extent
				location.blockNumber = g->freeSpaceMap[i].blockNumber;
				location.blockCount = blockCount;
				g->freeSpaceMap[i].blockNumber += blockCount;
				g->freeSpaceMap[i].blockCount -= blockCount;

				// Write to disk updated free space map
				LBAwrite(g->freeSpaceMap, vcb->freeSpaceMapLoc.blockCount, vcb->freeSpaceMapLoc.blockNumber);

				return location;
    	}
		}
	}
  
  // no free space found
  location.blockNumber = 0;
  location.blockCount = 0;

  return location;
}

int freeSpaceRelease(int blockNumber, int blockCount) {

	// Find the index we want to work with in other helper funcs
	int curIndex;
	for (int i = 0; i < g->freeSpaceExtentCount - 1; i++) {

		if (blockNumber > g->freeSpaceMap[i].blockNumber + g->freeSpaceMap[i].blockCount) {

			curIndex = i + 1;
			break;
		}
	}

	checkPre(curIndex, blockNumber, blockCount);
	checkPost(curIndex, blockNumber, blockCount);
	int mergeable = checkMerge(curIndex, blockNumber, blockCount);

	// No overlap, insert
	if (mergeable == -1) {
		freeShiftDown(curIndex);
		g->freeSpaceMap[curIndex].blockNumber = blockNumber;
		g->freeSpaceMap[curIndex].blockCount = blockCount;
	}
	
	return 0;
}

int freeShiftDown(int index) {

	// Start from the back and towards the front until reaching index
	for (int i = g->freeSpaceExtentCount - 1; i > index; i--) {

		g->freeSpaceMap[i].blockNumber = g->freeSpaceMap[i - 1].blockNumber;
		g->freeSpaceMap[i].blockCount = g->freeSpaceMap[i - 1].blockCount;

		// Set the newly created gap to extent 0, 0
		if (i == index + 1) {
			g->freeSpaceMap[i].blockNumber = 0;
			g->freeSpaceMap[i].blockNumber = 0;
		}
	}

	return 0;
}

int freeShiftUp(int index) {

	// Start from the front and towards the back until reaching index
	for (int i = index; i < g->freeSpaceExtentCount - 1; i++) {

		g->freeSpaceMap[i].blockNumber = g->freeSpaceMap[i + 1].blockNumber;
		g->freeSpaceMap[i].blockCount = g->freeSpaceMap[i + 1].blockCount;

		// Set the last extent 0, 0
		if (i == g->freeSpaceExtentCount - 2) {
			g->freeSpaceMap[g->freeSpaceExtentCount - 1].blockNumber = 0;
			g->freeSpaceMap[g->freeSpaceExtentCount - 1].blockNumber = 0;
		}
	}

	return 0;
}

// Return 0 for pre, -1 for non pre
int checkPre(int index, int blockNumber, int blockCount) {
	// Can add on the front of blockNumber
	if (blockNumber + blockCount == g->freeSpaceMap[index].blockNumber) {

		g->freeSpaceMap[index].blockNumber -= blockCount;
		g->freeSpaceMap[index].blockCount += blockCount;

		return 0;
	}

	return -1;
}

// Return 0 for post, -1 for non post
int checkPost(int index, int blockNumber, int blockCount) {
	// Can add on to end of blockCount
	if (blockNumber == g->freeSpaceMap[index].blockNumber + g->freeSpaceMap[index].blockCount) {

		g->freeSpaceMap[index].blockCount += blockCount;

		return 0;
	}

	return -1;
}

// Return 0 for merge-able, -1 for not merge-able
int checkMerge(int index, int blockNumber, int blockCount) {

	// Check for merge previous
	if (g->freeSpaceMap[index].blockNumber 
			< g->freeSpaceMap[index - 1].blockNumber + g->freeSpaceMap[index - 1].blockCount) {

		g->freeSpaceMap[index - 1].blockCount += g->freeSpaceMap[index].blockCount - g->freeSpaceMap[index].blockNumber;
		freeShiftUp(index);

		return 0;
	}

	// Check for merge next
	else if (g->freeSpaceMap[index + 1].blockNumber
	< g->freeSpaceMap[index].blockNumber + g->freeSpaceMap[index].blockCount) {

		g->freeSpaceMap[index].blockCount += g->freeSpaceMap[index + 1].blockCount - g->freeSpaceMap[index + 1].blockNumber;
		freeShiftUp(index + 1);

		return 0;
	}

	return -1;
}