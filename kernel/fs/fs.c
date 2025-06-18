#include "fs.h"

static fs fs_main;

// read a block 
int rblock(u32 dev, u32 block, void *buffer) {
    u32 sector = block * SPB;
    for (int i = 0; i < SPB; i++) {
        disk.read_sector(dev, sector + i, (u8*)buffer + (i * SECTOR_SIZE));
    }
    return 0;
}

// write a block 
int wblock(u32 dev, u32 block, const void* buffer) {
    u32 sector = block * SPB;
    for (int i = 0; i < SPB; i++) {
        disk.write_sector(dev, sector + i, (const u8*)buffer + (i * SECTOR_SIZE));
    }
    return 0;
}


int fs_format(u32 dev, u32 tblocks) {
    block_t b = {0};
    b.magic = FS_MAGIC;
    b.version = 1;
    b.size = BLOCK_SIZE;
    b.tblocks = tblocks;
    b.fblocks = tblocks - 1;
    b.node_start = 1; // first free block

    if (wblock(dev, 0, &b) != 0) return -1;

    free_block_t fb = {0};
    // just write over all blocks.
    for (u32 i = 1; i < tblocks; i++) {
        fb.num = i;
        fb.next = (i + 1 < tblocks) ? i + 1 : 0;
        wblock(dev, i, &fb);
    }

    return 0;
}

// mount fs from dev.
int fs_mount(u32 dev, fs *fs) {
    if (rblock(dev, 0, &fs->block) != 0) return -1;
    if (fs->block.magic != FS_MAGIC) return -1;
    fs->devid = dev;
    fs->ismounted = true;
    return 0;
}

// unmount fs
int fs_umount(fs *fs) {
    if (!fs->ismounted) return -1;
    fs->ismounted = false;
    return 0;
}

// write it back to the disk NOTE: Don't do this every frame, it will just lag out everything.
int fs_sync(fs *fs) {
    return wblock(fs->devid, 0, &fs->block);
}

// alloc one free block, returns block num
u32 block_alloc(fs *fs) {
    if (fs->block.fblocks == 0) return 0;
    free_block_t fb;
    rblock(fs->devid, fs->block.node_start, &fb);
    u32 block_num = fb.num;
    fs->block.node_start = fb.next;
    fs->block.fblocks--;
    fs_sync(fs);
    return block_num;
}

// just janks a block to the free list.
int block_free(fs *fs, u32 block) {
    free_block_t fb;
    fb.num = block;
    fb.next = fs->block.node_start;
    wblock(fs->devid, block, &fb);
    fs->block.node_start = block;
    fs->block.fblocks++;
    fs_sync(fs);
    return 0;
}

// just check if the block is marked free
bool is_free(fs *fs, u32 block) {
    free_block_t fb;
    rblock(fs->devid, block, &fb);
    return fb.num == block;
}

// alloc a new node (data)
u32 node_alloc(fs *fs) {
    u32 block = block_alloc(fs);
    if (!block) return 0;
    node_t node = {0};
    node.used = 1;
    node.num = block;
    wnode(fs, &node);
    return block;
}

// free a node AND it's block
int node_free(fs *fs, u32 num) {
    node_t node = {0};
    wnode(fs, &node);
    return block_free(fs, num);
}

// get node from disk
node_t* node_get(fs *fs, u32 num) {
    static node_t node;
    rblock(fs->devid, num, &node);
    return &node;
}

// write node to disk
int wnode(fs *fs, node_t *node) {
    return wblock(fs->devid, node->num, node);
}

// create a new file node TODO: Paths.
int create(fs *fs, const char* path, u16 mode) {
    u32 node_num = node_alloc(fs);
    if (!node_num) return -1;
    node_t *node = node_get(fs, node_num);
    node->type = NODE_FILE;
    node->size = 0;
    node->ctime = node->etime = 0;
    wnode(fs, node);
    return 0;
}

// open file TODO: Path handling
int open(fs *fs, const char* path, u16 flags, file_t *file) {
    u32 node_num = 1; /// rn this is kinda weird asf, theres no paths.
    node_t *node = node_get(fs, node_num);
    file->node_num = node_num;
    file->pos = 0;
    file->flags = flags;
    file->node = *node;
    return 0;
}

int close(fs *fs, file_t *file) {
    return 0;
}

// read file content 
int read(fs *fs, file_t *file, void *buffer, u64 size) {
    node_t *node = &file->node;
    if (file->pos + size > node->size) size = node->size - file->pos;
    rblock(fs->devid, node->num, buffer);
    file->pos += size;
    return size;
}

// write file content 
int write(fs *fs, file_t *file, const void *buffer, u64 size) {
    node_t *node = &file->node;
    wblock(fs->devid, node->num, buffer);
    file->pos += size;
    if (file->pos > node->size) node->size = file->pos;
    node->etime = 0;
    wnode(fs, node);
    return size;
}

// delete file node 
int fs_delete(fs *fs, const char* path) {
    u32 node_num = 1; // TODO: Paths.
    node_free(fs, node_num);
    return 0;
}

// repos from fhandler
int seek(fs *fs, file_t *file, u64 offset, int whence) {
    switch (whence) {
        case 0: file->pos = offset; break;               // SEEK_SET
        case 1: file->pos += offset; break;              // SEEK_CUR
        case 2: file->pos = file->node.size + offset; break; // SEEK_END
    }
    return 0;
}
