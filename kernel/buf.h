//buf.h
struct buf {
  int valid; // has data been read from disk?
  int disk; // does disk "own" buf?
  uint dev; //设备号，用于标识磁盘设备
  uint blockno;
  struct sleeplock lock;
  uint refcnt;
  struct buf *prev; // LRU cache list
  struct buf *next;
  uchar data[BSIZE];
  uint lastUsedTime; // 最近使用时间
  int curBucket; // 当前属于哪个哈希桶
};

