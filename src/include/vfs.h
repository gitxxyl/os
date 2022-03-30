#pragma once

#include <stdint.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08

typedef struct dirent {
    char name[256];
    uint64_t inode;
} dirent_t;

typedef struct fs_node fs_node_t;

typedef uint64_t (*vfs_read_t)(fs_node_t* node, void *buf, uint64_t size, uint64_t offset);
typedef uint64_t (*vfs_write_t)(fs_node_t* node, void *buf, uint64_t size, uint64_t offset);
typedef uint64_t (*vfs_open_t)(fs_node_t*);
typedef uint64_t (*vfs_close_t)(fs_node_t*);
typedef dirent_t* (*vfs_readdir_t)(fs_node_t*, uint64_t);
typedef fs_node_t* (*vfs_finddir_t)(fs_node_t*, char*);

typedef struct fs_node {
    char name[256];
    uint64_t mask;
    uint64_t uid;
    uint64_t gid;
    uint64_t flags;
    uint64_t inode;
    uint64_t length;
    uint64_t impl;
    vfs_read_t  read;
    vfs_write_t write;
    vfs_open_t open;
    vfs_close_t close;
    vfs_readdir_t readdir;
    vfs_finddir_t finddir;
    struct fs_node *ptr;
} fs_node_t;

extern fs_node_t* root;

uint64_t read_fs(fs_node_t* node, uint8_t* buffer, uint64_t size, uint64_t offset);
uint64_t write_fs(fs_node_t* node, uint8_t* buffer, uint64_t size, uint64_t offset);
uint64_t open_fs(fs_node_t* node, unsigned int flags);
uint64_t close_fs(fs_node_t* node);
dirent_t* readdir_fs(fs_node_t* node, uint64_t index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);
