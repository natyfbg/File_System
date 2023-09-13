/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: parsePath.c
*
* Description: Basic File System - Parse path function
*
**************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parsePath.h"
#include "vcb.h"
#include "fsLow.h"

// Return -1 if path does not exist, 0 if path does exist
int parsePath(char *path, ppinfo *result) {

  char token[MAXPATHSIZE];
  char last[MAXPATHSIZE];
  strcpy(last, strrchr(path, '/'));
  int DEindex;

  directoryEntry *dir = malloc(g->loadedDir[0].size);
  LBAread(dir, g->loadedDir[0].location.blockCount, g->loadedDir[0].location.blockNumber);

  // Check absolute path
  if (path[0] = '/') {
    free(dir);
    dir = malloc(g->rootDir[0].size);
    LBAread(dir, g->rootDir[0].location.blockCount, g->rootDir[0].location.blockNumber);
  }

  strcpy(token, strtok(path, "/"));

  // Tokenize the string, start parsing
  while (token != NULL) {

    DEindex = dirLocate(token);
    // Name does not exist or DE is not dir, invalid path
    if (DEindex == -1 || dir[DEindex].isDirectory == 0) {
      return -1;
    }

    // Load the directory at the index into memory
    free(dir);
    dir = malloc(dir[DEindex].size);
    LBAread(dir, dir[DEindex].location.blockCount, dir[DEindex].location.blockNumber);

    strcpy(token, strtok(NULL, "/"));

    // If currently on last element
    if (token == NULL) {

      if (last == token) {

        // If last element exists, return index
        for (int i = 0; i < g->actualDECount; i++) {
          if (dir[DEindex].name == token) {
            result->dirIndex = i;
            result->dirParent = dir;
            result->isDir = dir[DEindex].isDirectory;
            break;
          }
        }
        // Return next empty index
        for (int i = 0; i < g->actualDECount; i++) {
          if (dir[DEindex].name[0] == '\0') {
            result->nextEmptyIndex = i;
            break;
          }
        }
      }

      dirLoad(dir);
      strcpy(g->curPath, path);
      free(dir);
    }
  }

  return 0;
}