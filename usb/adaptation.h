typedef u32 dma_addr_t;

typedef u8 __u8;
typedef u16 __u16;
typedef u32 __u32;

typedef u16 __le16;
typedef u32 __le32;

struct list_head {
	struct list_head *next, *prev;
};

typedef struct {
	volatile int counter;
} atomic_t;

#define BITS_PER_LONG 64

#define __iomem
#define __force

#define le16_to_cpu __le16_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define __cpu_to_le16(x) ((__le16)__swab16((x)))
#define __le16_to_cpu(x) __swab16((__u16)(__le16)(x))

/**
 * __swab16 - return a byteswapped 16-bit value
 * @x: value to byteswap
 */
#define __swab16(x)				\
	(__builtin_constant_p((__u16)(x)) ?	\
	___constant_swab16(x) :			\
	___constant_swab16(x))

#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define cpu_to_le16p __cpu_to_le16
#define le16_to_cpup __le16_to_cpu
#define __cpu_to_le16p __cpu_to_le16
#define __le16_to_cpup __le16_to_cpu



#define cpu_to_le32 __cpu_to_le32
#define le32_to_cpu __le32_to_cpu
#define __cpu_to_le32(x) ((__le32)__swab32((x)))
#define __le32_to_cpu(x) __swab32((__u32)(__le32)(x))
/**
 * __swab32 - return a byteswapped 32-bit value
 * @x: value to byteswap
 */
#define __swab32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swab32(x) :			\
	___constant_swab32(x))

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define cpu_to_le32p __cpu_to_le32
#define le32_to_cpup __le32_to_cpu
#define __cpu_to_le32p __cpu_to_le32
#define __le32_to_cpup __le32_to_cpu


