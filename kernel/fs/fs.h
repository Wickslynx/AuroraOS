#ifndef FS_H
#define FS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "disk.h"
#include <util.h>
#include <mem.h>

#define SECTOR_SIZE 512
#define SPB 1 // 1 sector per block.
#define BLOCK_SIZE (SECTOR_SIZE * SPB)

#define FS_MAGIC 0x41555246 // AURF
#define M_FILENAME 255 // should be enough who tf has a file that has more than 255 letters in the name

#define M_PATH 255



typedef enum {
    NODE_FILE = 1,
    NODE_DIR = 2,
    NODE_SYMLINK = 3
} node_type_t;

typedef struct {
    u32 magic;
    u32 version; // fs version
    u32 size;
    u32 tblocks;
    u32 fblocks; // free
    u32 node_start;
} block_t;

typedef struct {
    u8 used; // 1 for used, 0 for not used.
    u32 num;
    u16 type;
    u32 uid;
    u64 size;
    u32 ctime; // creation time.
    u32 etime; // edit time.
    u8 reserved[64]; // space
} node_t;

typedef struct {
    u32 node;
    u16 len;
    char name[];
} dir_t;

typedef struct {
    u32 num;
    u32 next;
} free_block_t;

typedef struct {
    block_t block;
    u32 devid;
    bool ismounted;
} fs;

typedef struct {
    u32 node_num;
    u64 pos;
    u32 flags;
    node_t node;
} file_t


// core 

int fs_format(u32 dev, u32 tblocks);
int fs_mount(u32 dev, fs *fs);
int fs_umount(fs *fs);
int fs_sync(fs *fs);

// node stuff -> this should be moved to a seperate file..
u32 block_alloc(fs *fs);
int block_free(fs *fs);
bool is_free(fs *fs, u32 block);

u32 node_alloc(fs *fs);
u32 node_free(fs *fs, num);
node_t* node_get(fs *fs, num);
int wnode(fs *fs, node_t *node);

// fops
int create(fs *fs, const char* path, u16 mode);
int open(fs *fs, const char* path, u16 flags, file_t file);
int close(fs *fs, file_t file);
int read(fs *fs, file_t file, void *buffer, u64 size);
int write(fs *fs, file_t file, const void *buffer, u64 size);
int fs_delete(fs *fs, const char path);
int seek(fs *fs, file_t file, u64 offset, int whence);
    
#ifdef __cplusplus
}
#endif

#endif // FS_H
