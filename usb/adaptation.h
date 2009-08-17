typedef u32 dma_addr_t;
typedef u8 __u8;

typedef u16 __attribute__((bitwise)) __le16;

struct list_head {
	struct list_head *next, *prev;
};

typedef struct {
	volatile int counter;
} atomic_t;

#define __iomem
