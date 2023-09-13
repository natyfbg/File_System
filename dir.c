/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: dir.c
*
* Description: Basic File System - Directory and DE functions
*
**************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vcb.h"
#include "freespace.h"
#include "util.h"
#include "fsLow.h"

directoryEntry *dirInit(int baseNumDE, directoryEntry* parent) {

	// Optimize bytes usage in block to avoid wasted bytes
	int bytesNeeded = baseNumDE * sizeof(directoryEntry);
	int blocksNeeded = getBlocksPerByte(bytesNeeded, vcb->blockSize);
	int bytesUsed = blocksNeeded * vcb->blockSize;
	g->actualDECount = bytesUsed / sizeof(directoryEntry);
	bytesNeeded = g->actualDECount * sizeof(directoryEntry);

	// Free later by caller
	directoryEntry *dir = malloc(bytesUsed);

	// Alloc free space
	extent location = freeSpaceAlloc(blocksNeeded);

	// Mark all DE starting from dir[2] as unused
	for (int i = 2; i < g->actualDECount; i++) {
		dir[i].name[0] = '\0';
	}

	// Set first dir to "."
	strcpy(dir[0].name, ".");
	extentCopy(dir[0].location, location);
	dir[0].size = bytesNeeded;
	dir[0].isDirectory = 1;
	dir[0].timeCreate = time(NULL);
	dir[0].timeMod = dir[0].timeCreate;
	dir[0].timeAccess = dir[0].timeCreate;

	// If doing Root Directory, then copy dir[0] except name
	if (parent == NULL) {
		strcpy(dir[1].name, "..");
		extentCopy(dir[1].location, location);
		dir[1].size = bytesNeeded;
		dir[1].isDirectory = 1;
		dir[1].timeCreate = time(NULL);
		dir[1].timeMod = dir[1].timeCreate;
		dir[1].timeAccess = dir[1].timeCreate;

		// Store info about root into vcb
		vcb->rootDir = dir[0];

		// Load into memory for further use
		g->rootDir = malloc(bytesUsed);
		g->loadedDir = malloc(bytesUsed);
		dirCopyDir(g->rootDir, dir);
		dirCopyDir(g->loadedDir, dir);
	}

	// If doing non Root Directory, then copy parent except name
	else {
		strcpy(dir[1].name, "..");
		extentCopy(dir[1].location, parent->location);
		dir[1].size = parent->size;
		dir[1].isDirectory = parent->isDirectory;
		dir[1].timeCreate = parent->timeCreate;
		dir[1].timeMod = parent->timeMod;
		dir[1].timeAccess = parent->timeAccess;
	}

	// Write directory to disk
	LBAwrite(dir, location.blockCount, location.blockNumber);
	
	return dir;
}

int dirLoad(directoryEntry *dir) {

	// Load dir into memory
	free(g->loadedDir);
	g->loadedDir = malloc(dir[0].size);
	LBAread(g->loadedDir, dir[0].location.blockCount, dir[0].location.blockNumber);

	return 0;
}

int dirLocate(char* name) {

	// Return index where DE with name is found, -1 if fail
	for (int i = 0; i < g->actualDECount; i++) {
		if (g->loadedDir[i].name == name) {
			return i;
		}
	}

	return -1;
}

int dirCopyDE(directoryEntry *dest, directoryEntry *source, int index) {

	// Copy values of source dir into dest dir[index]
	strcpy(dest[index].name, source[0].name);
	dest[index].location = source[0].location;
	dest[index].size = source[0].size;
	dest[index].isDirectory = source[0].isDirectory;
	dest[index].timeCreate = source[0].timeCreate;
	dest[index].timeMod = source[0].timeMod;
	dest[index].timeAccess = source[0].timeAccess;

	LBAwrite(dest, dest[0].location.blockCount, dest[0].location.blockNumber);

	return 0;
}

int dirCopyDir(directoryEntry *dest, directoryEntry *source) {

	// Copy values of source dir into dest dir
	for (int i = 0; i < g->actualDECount; i++) {
		strcpy(dest[i].name, source[i].name);
		dest[i].location = source[i].location;
		dest[i].size = source[i].size;
		dest[i].isDirectory = source[i].isDirectory;
		dest[i].timeCreate = source[i].timeCreate;
		dest[i].timeMod = source[i].timeMod;
		dest[i].timeAccess = source[i].timeAccess;
	}

	LBAwrite(dest, dest[0].location.blockCount, dest[0].location.blockNumber);

	return 0;
}

int dirDeleteDE(directoryEntry *dir, int index) {

	// Mark DE in dir as unused
	dir[index].name[0] = '\0';
	LBAwrite(dir, dir[0].location.blockCount, dir[0].location.blockNumber);

	return 0;
}