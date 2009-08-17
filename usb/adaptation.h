typedef u32 dma_addr_t;

struct list_head {
	struct list_head *next, *prev;
};

#define __iomem
