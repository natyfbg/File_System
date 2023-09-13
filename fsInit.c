/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "vcb.h"
#include "freespace.h"

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize) {
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	
	// Calls freeSpaceInit and dirInit inside vcbInit
	vcbInit(blockSize, numberOfBlocks);

	return 0;
}

void exitFileSystem () {
	free(g->freeSpaceMap);
	free(g->loadedDir);
	free(g->rootDir);
	free(g);
	free(vcb);
	printf ("System exiting\n");
}