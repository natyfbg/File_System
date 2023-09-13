My File System Implementation
Welcome to my File System implementation project! In this repository, I have designed and implemented a basic file system in the C programming language. This project consists of three phases: formatting the volume, implementing directory-based functions, and finally, implementing file operations.

Table of Contents
Introduction
Prerequisites
Project Structure
Features
Usage
File System Interfaces
Shell Program
Building and Running
Issues and Challenges
Screenshots
License
1. Introduction
In this project, I have created a file system that allows for basic file and directory management. The file system supports operations like creating directories, adding and removing files, copying and moving files, and more. It also includes a shell program that demonstrates how to interact with the file system.

2. Prerequisites
Before you can build and run this project, make sure you have the following prerequisites installed:

C Compiler: You need a C compiler to compile the source code.
Linux Environment: This project is designed for a Linux environment.
3. Project Structure
The project structure is organized as follows:

graphql
Copy code
my-filesystem/
├── fsLow.c                # Low-level LBA read and write functions
├── fsLow.h                # Header for LBA functions
├── fsShell.c              # Shell program to interact with the file system
├── fsInit.c               # Initialization and exit code for the file system
├── fs.h                   # Header file for the file system interfaces
├── Makefile               # Makefile for building the project
├── volume.dat             # The volume file for the file system
└── ...
4. Features
Directory Management: Create, list, and remove directories.
File Operations: Create, read, write, and delete files.
File System Navigation: Change the current working directory and display the working directory.
File and Directory Information: Obtain information about files and directories, such as size and timestamps.
Shell Program: Interact with the file system using the included shell program.
5. Usage
This file system provides several interfaces for interacting with files and directories. These interfaces are described in detail in the next section.

6. File System Interfaces
To interact with the file system, I have provided the following interfaces:

b_io_fd b_open (char * filename, int flags);
int b_read (b_io_fd fd, char * buffer, int count);
int b_write (b_io_fd fd, char * buffer, int count);
int b_seek (b_io_fd fd, off_t offset, int whence);
int b_close (b_io_fd fd);
int fs_mkdir(const char *pathname, mode_t mode);
int fs_rmdir(const char *pathname);
fdDir * fs_opendir(const char *name);
struct fs_diriteminfo *fs_readdir(fdDir *dirp);
int fs_closedir(fdDir *dirp);
char * fs_getcwd(char *buf, size_t size);
int fs_setcwd(char *buf);
int fs_isFile(char * path);
int fs_isDir(char * path);
int fs_delete(char* filename);
struct fs_stat
off_t st_size;
blksize_t st_blksize;
blkcnt_t st_blocks;
time_t st_accesstime;
time_t st_modtime;
time_t st_createtime;
7. Shell Program
The shell program, fsShell.c, allows you to interact with the file system using various commands such as ls, cp, mv, mkdir, rm, touch, cat, and more. It provides a user-friendly interface for testing and demonstrating the file system's functionality.

8. Building and Running
To build and run the project, follow these steps:

Compile the project using the provided Makefile:

bash
Copy code
make
Run the shell program:

bash
Copy code
./fsShell
Use the commands within the shell program to interact with the file system.

9. Issues and Challenges
During the implementation of this file system, I encountered various challenges, including managing directory structures, file allocation, and handling file operations efficiently. Solving these challenges was an essential part of completing the project successfully.

10. Screenshots
Insert Screenshots Here

11. License
This project is licensed under the MIT License. See the LICENSE file for details.

Feel free to explore and use this file system implementation. If you have any questions, feedback, or encounter any issues, please feel free to reach out. Happy file system exploring!
