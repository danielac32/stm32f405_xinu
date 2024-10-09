#if 0
#include <xinu.h>
#include <w25qxxx.h>




#define psram_read(addr, buf, len) SPI_Flash_Read(buf,addr,len)
#define psram_write(addr, buf, len) SPI_Flash_Write(buf,addr,len)



#define CACHESIZE   4096
struct cacheline {
    uint8_t data[64];
};

static uint64_t accessed, hit;
static uint32_t tags[CACHESIZE/64/2][2];
static struct cacheline cachelines[CACHESIZE/64/2][2];

/*
 * bit[0]: valid
 * bit[1]: dirty
 * bit[2]: for LRU
 * bit[3:10]: reserved
 * bit[11:31]: tag
 */
#define VALID       (1 << 0)
#define DIRTY       (1 << 1)
#define LRU     (1 << 2)
#define LRU_SFT     2
#define TAG_MSK     0xfffff800

/*
 * bit[0: 5]: offset
 * bit[6: 10]: index
 * bit[11: 31]: tag
 */
static inline int get_index(uint32_t addr)
{
    return (addr >> 6) & 0x1f;
}

void cache_write(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("write cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                   psram_write(*tp & ~0x3f, p, 64);
                    //SPI_Flash_Write(line->data,flush_base,CACHE_LINE_SIZE);
                }
                psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(p + (ofs & 0x3f), buf, size);
    *tp |= DIRTY;
}

void cache_read(uint32_t ofs, void *buf, uint32_t size)
{
    if (((ofs | (64 - 1)) != ((ofs + size - 1) | (64 - 1))))
        printf("read cross boundary, ofs:%x size:%x\n", ofs, size);

    int ti, i, index = get_index(ofs);
    uint32_t *tp;
    uint8_t *p;

    ++accessed;

    for (i = 0; i < 2; i++) {
        tp = &tags[index][i];
        p = cachelines[index][i].data;
        if (*tp & VALID) {
            if ((*tp & TAG_MSK) == (ofs & TAG_MSK)) {
                ++hit;
                ti = i;
                break;
            } else {
                if (i != 1)
                    continue;

                ti = 1 - ((*tp & LRU) >> LRU_SFT);
                tp = &tags[index][ti];
                p = cachelines[index][ti].data;

                if (*tp & DIRTY) {
                    psram_write(*tp & ~0x3f, p, 64);
                }
                psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(buf, p + (ofs & 0x3f), size);
}

void cache_get_stat(uint64_t *phit, uint64_t *paccessed)
{
    *phit = hit;
    *paccessed = accessed;
}
#endif