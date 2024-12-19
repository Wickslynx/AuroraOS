#ifdef FILE_SYS
#define FILE_SYS

#define FS_MAGIC 0x19383845
#define FS_BLOCK_SIZE 512
#define FS_INODE_COUNT 128

typedef struct superblock {
  unsigned int magic;
  unsigned int total_blocks;
  unsigned int inode_count;
  unsigned int data_block_count;
} superblock_t;

typedef struct inode {
  unsigned int size;
  unsigned int data_block_indices;
} inode_t;

typedef struct dir_entry {
  char filename[32];
  unsigned int inode_index;
} dir_entry_t;

void fs_init;
void fs_create();
void fs_write();
void fs_read();


#endif 
