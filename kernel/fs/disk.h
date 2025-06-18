typedef struct {
  int (*read)(u32 dev, u32 sector, void *buffer);
  int (*write)(u32 dev, u32 sector, const void *buffer);
  int (*flush)(u32 dev); 
} disk;

int readb();
int writeb();
