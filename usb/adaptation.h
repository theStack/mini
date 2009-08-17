typedef u32 dma_addr_t;

typedef u8 __u8;
typedef u16 __u16;
typedef u32 __u32;

typedef u16 __le16;

struct list_head {
	struct list_head *next, *prev;
};

typedef struct {
	volatile int counter;
} atomic_t;

#define BITS_PER_LONG 64

#define __iomem

#define le16_to_cpu __le16_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define __cpu_to_le16(x) ((__force __le16)__swab16((x)))
#define __le16_to_cpu(x) __swab16((__force __u16)(__le16)(x))

/**
 * __swab16 - return a byteswapped 16-bit value
 * @x: value to byteswap
 */
#define __swab16(x)				\
	(__builtin_constant_p((__u16)(x)) ?	\
	___constant_swab16(x) :			\
	__fswab16(x))

#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

