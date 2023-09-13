/**************************************************************
* Class:  CSC-415-02 Spring 2023
* Names: James Liu, Gebriel Gidey, Estefanos Kebebew, Nathnael Gebru
* Student IDs: 921381068, 922049034, 922065271, 922092480
* GitHub Name: Gebriell
* Group Name: Team Shinobi
* Project: Basic File System
*
* File: mfs.c
*
* Description: Basic File System - Directory function shell commands
*
**************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mfs.h"
#include "parsePath.h"
#include "util.h"
#include "freespace.h"
#include "vcb.h"
#include "fsLow.h"

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode) {

  // Call parsePath to get info and check for path validity
  char *path;
  strcpy(path, pathname);
  ppinfo *parsePathInfo;
  int valid = parsePath(path, parsePathInfo);

  // Path invalid
  if (valid == -1) {
    return -1;
  }

  strcpy(g->curPath, path);

  // If dir does not exist
  if (parsePathInfo->dirIndex == -1 || parsePathInfo->dirParent[parsePathInfo->dirIndex].isDirectory == 0) {

    // Good to go, time to create directory
    directoryEntry *dir = dirInit(STARTDE, parsePathInfo->dirParent);
    dirCopyDE(parsePathInfo->dirParent, dir, parsePathInfo->dirIndex);

    // Write to disk
    LBAwrite(g->loadedDir, g->loadedDir[0].location.blockCount, g->loadedDir[0].location.blockNumber);
    LBAwrite(parsePathInfo->dirParent, parsePathInfo->dirParent[0].location.blockCount, parsePathInfo->dirParent[0].location.blockNumber);

    return 0;
  }

  return -1;
}

int fs_rmdir(const char *pathname) {

  // Call parsePath to get info and check for path validity
  char *path;
  strcpy(path, pathname);
  ppinfo *parsePathInfo;
  int valid = parsePath(path, parsePathInfo);

  // Path invalid
  if (valid == -1) {
    return -1;
  }

  strcpy(g->curPath, path);

  // If dir does exist
  if (parsePathInfo->dirIndex != -1 && parsePathInfo->dirParent[parsePathInfo->dirIndex].isDirectory == 1) {

    // Good to go, time to delete dir
    dirLoad(&parsePathInfo->dirParent[parsePathInfo->dirIndex]);

    // Make sure dir is empty
    for (int i = 2; i < g->actualDECount; i++) {

      if (g->loadedDir[i].name != "/0") {

        dirLoad(parsePathInfo->dirParent);
        return -1;
      }
    }

    // Dir empty, time to delete
    dirDeleteDE(parsePathInfo->dirParent, parsePathInfo->dirIndex);
    freeSpaceRelease(g->loadedDir[0].location.blockCount, g->loadedDir[0].location.blockNumber);

    // Write to disk
    dirLoad(parsePathInfo->dirParent);
    LBAwrite(g->loadedDir, g->loadedDir[0].location.blockCount, g->loadedDir[0].location.blockNumber);
    
    return 0;
  }

  return -1;
}

// Directory iteration functions
fdDir * fs_opendir(const char *pathname) {

  fdDir *fd;

  // Call parsePath to get info and check for path validity
  char path[MAXPATHSIZE];
  strcpy(path, pathname);
  ppinfo *parsePathInfo;

  int valid = parsePath(path, parsePathInfo);

  // Path is valid and exists
  if (valid == 0 && parsePathInfo->dirIndex != -1) {

    // Is a directory
    if (g->loadedDir[parsePathInfo->dirIndex].isDirectory == 1) {

      // Load into memory
      dirLoad(&parsePathInfo->dirParent[parsePathInfo->dirIndex]);

      // Fill in info to fdDir
      fd->d_reclen = g->loadedDir[0].size;
      fd->dirEntryPosition = 0;
      fd->directoryStartLocation = g->loadedDir[0].location.blockNumber;
    }
  }

  return fd;
}

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {

  struct fs_diriteminfo *fInfo;

  // While dir index has not reached the end
  if (dirp->dirEntryPosition != g->actualDECount) {

    strcpy(fInfo->d_name, g->loadedDir[dirp->dirEntryPosition].name);
    fInfo->d_reclen = g->loadedDir[dirp->dirEntryPosition].size;
    fInfo->fileType = g->loadedDir[dirp->dirEntryPosition].isDirectory;
  }

  dirp->dirEntryPosition++;

  return fInfo;
}

int fs_closedir(fdDir *dirp) {

  free(dirp);
  return 0;
}

// Misc directory functions
char * fs_getcwd(char *pathname, size_t size) {

  strcpy(pathname, g->curPath);

  return g->curPath;
}

//linux chdir
int fs_setcwd(char *pathname) {

  // Call parsePath to get info and check for path validity
  char *path;
  strcpy(path, pathname);
  ppinfo *parsePathInfo;
  int valid = parsePath(path, parsePathInfo);

  // Path invalid
  if (valid == -1) {
    return -1;
  }

  // If dir is found
  if (parsePathInfo->dirIndex != -1 && parsePathInfo->dirParent[parsePathInfo->dirIndex].isDirectory == 1) {

    // Good to go, time to set dir
    dirLoad(&parsePathInfo->dirParent[parsePathInfo->dirIndex]);
    strcpy(g->curPath, path);
  }

  return 0;  
}

//return 1 if file, 0 otherwise
int fs_isFile(char * filename) {

  // Look for DE that matches name and is not dir
  for (int i = 0; i < g->actualDECount; i++) {

    if (g->loadedDir[i].name == filename && g->loadedDir[i].isDirectory == 0) {
      return 1;
    }
  }

  return 0;
}

//return 1 if directory, 0 otherwise
int fs_isDir(char * pathname) {

  // Call parsePath to get info and check for path validity
  char *path;
  strcpy(path, pathname);
  ppinfo *parsePathInfo;
  int valid = parsePath(path, parsePathInfo);

  // Path invalid
  if (valid == -1) {
    return 0;
  }

  // Dir found
  if (parsePathInfo->dirIndex != -1 && parsePathInfo->dirParent[parsePathInfo->dirIndex].isDirectory == 1) {
    return 1;
  }

  return 0;
}

//removes a file
int fs_delete(char* filename) {

  // Find the DE with filename
  for (int i = 0; i < g->actualDECount; i++) {
    
    // If DE found and is a file, delete it
    if (g->loadedDir[i].name == filename && g->loadedDir[i].isDirectory == 0) {
      g->loadedDir[i].name == "\0";
      LBAwrite(g->loadedDir, g->loadedDir[0].location.blockCount, g->loadedDir[0].location.blockNumber);
      return 0;
    }
  }

  return -1;
}

int fs_stat(const char *path, struct fs_stat *buf) {

  char *pathname;
  strcpy(pathname, path);
  ppinfo *parsePathInfo;
  int valid = parsePath(pathname, parsePathInfo);

  // Path invalid, exit
  if (valid == -1) {
    return -1;
  }

  // Fill in fs_stat
  buf->st_size = parsePathInfo->dirParent[parsePathInfo->dirIndex].size;
  buf->st_blksize = vcb->blockSize;
  buf->st_blocks = getBlocksPerByte(buf->st_size, buf->st_blksize);
  buf->st_accesstime = parsePathInfo->dirParent[parsePathInfo->dirIndex].timeAccess;
  buf->st_modtime = parsePathInfo->dirParent[parsePathInfo->dirIndex].timeMod;
  buf->st_createtime = parsePathInfo->dirParent[parsePathInfo->dirIndex].timeCreate;

  return 0;
}