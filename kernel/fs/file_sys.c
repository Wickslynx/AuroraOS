#include "file_sys.h"
#include "../utils/utils.h"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --File system built for AuroraOS--


Important stuff to know:

Latest update:
Revamped the FS to follow a dynamic approach instead.


Global variables:


Memory layout: 
[Superblock][Inode Bitmap][Data Bitmap][Inode Table][Data Blocks]

Additional declarations:
true, false: utils/macros/macros.c


*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include "file_sys.h"
#include "../utils/utils.h"

#define FS_BASE_ADDRESS 0x200000
#define FS ((unsigned char *)FS_BASE_ADDRESS)

superblock_t *sb;
inode_t *inodes;
unsigned char *data_blocks;
unsigned char *free_inode_bitmap;
unsigned char *free_data_bitmap;

typedef char bool;

void fs_init() {
    sb = (superblock_t *)FS;
    sb->magic = FS_MAGIC;

    sb->total_blocks = 1024;
    sb->inode_blocks = 16;
    sb->data_blocks = 1008;

    sb->free_inode_bitmap = sizeof(superblock_t);
    sb->free_data_bitmap = sb->free_inode_bitmap + sb->inode_blocks;

    inodes = (inode_t *)(FS + sb->free_inode_bitmap + sb->inode_blocks);
    data_blocks = FS + BLOCK_SIZE * (sb->free_data_bitmap + sb->data_blocks);
    free_inode_bitmap = FS + sb->free_inode_bitmap;
    free_data_bitmap = FS + sb->free_data_bitmap;

    OSmemset(free_inode_bitmap, 0, sb->inode_blocks);
    OSmemset(free_data_bitmap, 0, sb->data_blocks);
}

int create(const char *filename) {
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        if (!free_inode_bitmap[i]) {
            free_inode_bitmap[i] = 1;
            inode_t *new_inode = &inodes[i];
            OSmemset(new_inode, 0, sizeof(inode_t));

            dir_entry_t *new_entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
            for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
                new_entry->filename[j] = filename[j];
                if (filename[j] == '\0') break;
            }
            new_entry->inode_index = i;
            return 0;
        }
    }
    return -1;
}

int write(const char *filename, const char *data, unsigned int length) {
    if (!filename || !data || length == 0) return -1;

    for (unsigned int i = 0; i < MAX_FILES; i++) {
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
        bool found = true;
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
            if (entry->filename[j] != filename[j]) {
                found = false;
                break;
            }
            if (filename[j] == '\0') break;
        }

        if (found) {
            inode_t *inode = &inodes[entry->inode_index];
            if (length > BLOCK_SIZE * 12) return -1;

            unsigned int blocks_needed = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;
            unsigned int blocks_written = 0;

            for (unsigned int j = 0; j < MAX_DATA_BLOCKS && blocks_written < blocks_needed; j++) {
                if (!free_data_bitmap[j]) {
                    free_data_bitmap[j] = 1;
                    inode->data_block_indices[blocks_written] = j;

                    unsigned int bytes_to_copy = (blocks_written == blocks_needed - 1)
                        ? (length % BLOCK_SIZE ? length % BLOCK_SIZE : BLOCK_SIZE)
                        : BLOCK_SIZE;

                    OSmemcpy(data_blocks + j * BLOCK_SIZE, data + blocks_written * BLOCK_SIZE, bytes_to_copy);
                    blocks_written++;
                }
            }

            inode->size = length;
            return 0;
        }
    }
    return -1;
}

int read(const char *filename, char *buffer, unsigned int length) {
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
        bool found = true;
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
            if (entry->filename[j] != filename[j]) {
                found = false;
                break;
            }
            if (filename[j] == '\0') break;
        }

        if (found) {
            inode_t *inode = &inodes[entry->inode_index];
            length = inode->size;

            for (unsigned int j = 0; j < (length + BLOCK_SIZE - 1) / BLOCK_SIZE; j++) {
                OSmemcpy(buffer + j * BLOCK_SIZE,
                         data_blocks + inode->data_block_indices[j] * BLOCK_SIZE,
                         BLOCK_SIZE);
            }
            return length;
        }
    }
    return -1;
}

