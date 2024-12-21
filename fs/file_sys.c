#include "filesystem.h"


unsigned char fs_memory[BLOCK_SIZE * 1024]; // 512KB filesystem

// Pointers to the filesystem sections
superblock_t *sb;
inode_t *inodes;
unsigned char *data_blocks;
unsigned char *free_inode_bitmap;
unsigned char *free_data_bitmap;


void init_filesystem() {
    sb = (superblock_t *)fs_memory;
    sb->magic = FS_MAGIC;
    sb->total_blocks = 1024;
    sb->inode_blocks = 16;
    sb->data_blocks = 1008;
    sb->free_inode_bitmap = sizeof(superblock_t);
    sb->free_data_bitmap = sb->free_inode_bitmap + sb->inode_blocks;
    
    inodes = (inode_t *)(fs_memory + sb->free_inode_bitmap + sb->inode_blocks);
    data_blocks = fs_memory + BLOCK_SIZE * (sb->free_data_bitmap + sb->data_blocks);
    free_inode_bitmap = fs_memory + sb->free_inode_bitmap;
    free_data_bitmap = fs_memory + sb->free_data_bitmap;

    memset(free_inode_bitmap, 0, sb->inode_blocks);
    memset(free_data_bitmap, 0, sb->data_blocks);
}

int create_file(const char *filename) {
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        if (!free_inode_bitmap[i]) {
            free_inode_bitmap[i] = 1;
            inode_t *new_inode = &inodes[i];
            memset(new_inode, 0, sizeof(inode_t));
            dir_entry_t *new_entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
            for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
                new_entry->filename[j] = filename[j];
                if (filename[j] == '\0') break;
            }
            new_entry->inode_index = i;
            return 0;
        }
    }
    return -1; // No free inode available
}

int write_file(const char *filename, const char *data, unsigned int length) {
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
        int match = 1;
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
            if (entry->filename[j] != filename[j]) {
                match = 0;
                break;
            }
            if (filename[j] == '\0') break;
        }
        if (match) {
            inode_t *file_inode = &inodes[entry->inode_index];
            if (length > BLOCK_SIZE * 12) {
                return -1; // File too large
            }
            for (unsigned int j = 0; j < (length + BLOCK_SIZE - 1) / BLOCK_SIZE; j++) {
                if (!free_data_bitmap[j]) {
                    free_data_bitmap[j] = 1;
                    file_inode->data_block_indices[j] = j;
                    memcpy(data_blocks + j * BLOCK_SIZE, data + j * BLOCK_SIZE, BLOCK_SIZE);
                }
            }
            file_inode->size = length;
            return 0;
        }
    }
    return -1; // File not found
}

int read_file(const char *filename, char *buffer, unsigned int length) {
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE);
        int match = 1;
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
            if (entry->filename[j] != filename[j]) {
                match = 0;
                break;
            }
            if (filename[j] == '\0') break;
        }
        if (match) {
            inode_t *file_inode = &inodes[entry->inode_index];
            if (length > file_inode->size) {
                length = file_inode->size;
            }
            for (unsigned int j = 0; j < (length + BLOCK_SIZE - 1) / BLOCK_SIZE; j++) {
                memcpy(buffer + j * BLOCK_SIZE, data_blocks + file_inode->data_block_indices[j] * BLOCK_SIZE, BLOCK_SIZE);
            }
            return length;
        }
    }
    return -1; // File not found
}
