#include "filesystem.h"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --File system built for AuroraDB--


Important stuff to know:

Superblock structure:
Total blocks: 1024
Inode blocks: 16
Data blocks: 1008


Global variables:
- fs_memory, Main array thats holding the whole filesystem.
- sb, ptr to superblock.
- inodes, ptr to the inode table.
- data_blocks, ptr to the data storage.
- free_inode_bitmap, Tracks inode allocation
- free_data_bitmap, Tracks data allocation.

Memory layout: 
[Superblock][Inode Bitmap][Data Bitmap][Inode Table][Data Blocks]

Additional declarations:

memcpy, memset: memory/memory.c
true, false: macros/macros.c


*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

unsigned char fs_memory[BLOCK_SIZE * 1024]; // 512KB filesystem.

// Pointers to the filesystem sections, see (Global variables).
superblock_t *sb;
inode_t *inodes;
unsigned char *data_blocks;
unsigned char *free_inode_bitmap;
unsigned char *free_data_bitmap;


void fs_init() {

    //Init superblock at the start.
    sb = (superblock_t *)fs_memory; 
    sb->magic = FS_MAGIC;

    //Assign the right values.
    sb->total_blocks = 1024;
    sb->inode_blocks = 16;
    sb->data_blocks = 1008;
    
    //Calculate the bitmap pos.
    sb->free_inode_bitmap = sizeof(superblock_t); //Calculate the pos for the inodes.
    sb->free_data_bitmap = sb->free_inode_bitmap + sb->inode_blocks; // Calculate the pos for the data.
    
    //Set up the pointers to the diffrent parts.
    inodes = (inode_t *)(fs_memory + sb->free_inode_bitmap + sb->inode_blocks); //Create inode_t struct with the memory + the offset of the free_inode_bitmap + the location of the blocks.
    data_blocks = fs_memory + BLOCK_SIZE * (sb->free_data_bitmap + sb->data_blocks); //Create data blocks with the size of the mem with the block size and with the offset and the location.
    free_inode_bitmap = fs_memory + sb->free_inode_bitmap; //Make a pointer to the already defined method.
    free_data_bitmap = fs_memory + sb->free_data_bitmap; //Make a pointer to the already defined method.

    //Make memsets to and set every block to 0 (free).
    memset(free_inode_bitmap, 0, sb->inode_blocks);
    memset(free_data_bitmap, 0, sb->data_blocks);
}

int create(const char *filename) {        
    for (unsigned int i = 0; i < MAX_FILES; i++) {
        
        if (!free_inode_bitmap[i]) {
            free_inode_bitmap[i] = 1; //Set to used.
            
            inode_t *new_inode = &inodes[i]; //Make a new inode.
            memset(new_inode, 0, sizeof(inode_t)); //Make available mem for it .
            
            dir_entry_t *new_entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE); //Make a new directory entry.
            
            for (int j = 0; j < MAX_FILENAME_LENGTH; j++) { //For the length of the file.
                new_entry->filename[j] = filename[j]; //Write the name of the file.
                if (filename[j] == '\0') break; //If at the end, break.
            }
            
            new_entry->inode_index = i; //Set the new filename to the inode index.
            return 0;
        }
    }
    return -1; // If no free inode.
]

int write(const char *filename, const char *data, unsigned int length) {
    for (unsigned int i = 0; i < MAX_FILES; i++) { //For every file.
        
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE); //Make a new directory entry.
        
        int found = true; //Declare found as default true,
        
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
            if (entry->filename[j] != filename[j]) { //If entry dosen't match the filename.
                found = false; //Set found to false;
                break;
            }
            if (filename[j] == '\0') break; //If filename length 0, break.
        }
        
        if (found) { //If they match.
        
            inode_t *file_inode = &inodes[entry->inode_index]; //Make a new inode for the file.

            //Check the filesize limit.
            if (length > BLOCK_SIZE * 12) {
                return -1; // File too large.
            }

            //Allocate + write.
            for (unsigned int j = 0; j < (length + BLOCK_SIZE - 1) / BLOCK_SIZE; j++) { //If j is less than the number of required blocks, continue, else restart (increment j).
                
                if (!free_data_bitmap[j]) { //If there is a free inode pos.
                    
                    free_data_bitmap[j] = 1; //Set that pos to used.
                    
                    file_inode->data_block_indices[j] = j; //Set the file inode to the pos.
                    
                    memcpy(data_blocks + j * BLOCK_SIZE, data + j * BLOCK_SIZE, BLOCK_SIZE); //Memory copy the data to the pos.
                }
            }
            
            file_inode->size = length;  //Set the file inodes size to the length.
            return 0; 
        }
    }
    return -1; // File not found
}

int read(const char *filename, char *buffer, unsigned int length) {
    for (unsigned int i = 0; i < MAX_FILES; i++) { //For all files.
        
        dir_entry_t *entry = (dir_entry_t *)(data_blocks + i * BLOCK_SIZE); //Make a new directory entry as usual :O
        
        int found = true; //Declare found as default true,
        
        for (int j = 0; j < MAX_FILENAME_LENGTH; j++) { //For all files.
            if (entry->filename[j] != filename[j]) { //If the filename and entry match.
                found = false;
                break;
            }
            
            if (filename[j] == '\0') break; //If the length of the filename 0, break.
        }
        
        if (found) { //If match.
            
            inode_t *file_inode = &inodes[entry->inode_index]; //Make a new inode.
            
            //Set the length to be the size of the file.
            length = file_inode->size;
            
            //For all data blocks.
            for (unsigned int j = 0; j < (length + BLOCK_SIZE - 1) / BLOCK_SIZE; j++) {
                memcpy(buffer + j * BLOCK_SIZE, data_blocks + file_inode->data_block_indices[j] * BLOCK_SIZE, BLOCK_SIZE); // Memory copy them to a char array (buffer).
            }
            
            return length; //Return the length of the file-
        }
    }
    return -1; // File not found.
}
