#include <xinu.h>
//#include <w25qxxx.h>
#include <emulator.h>
#include <23lc.h>


//#define psram_read(addr, buf, len) SPI_Flash_Read(buf,addr,len)
//#define psram_write(addr, buf, len) SPI_Flash_Write(buf,addr,len)


#if 0
#define ADDR_BITS 24
#define OFFSET_BITS 4
#define CACHE_LINE_SIZE 16
#define INDEX_BITS 5

#define OFFSET(addr) (addr & 0b1111)
#define INDEX(addr) ((addr >> OFFSET_BITS) & 0b11111)
#define TAG(addr) (addr >> 9)
#define BASE(addr) (addr & (~(uint32_t)(0b1111)))

#define LINE_TAG(line) (line->tag)

#define IS_VALID(line) (line->status & 0b01)
#define IS_DIRTY(line) (line->status & 0b10)
#define IS_LRU(line) (line->status & 0b100)

#define SET_VALID(line) line->status = 1
#define SET_DIRTY(line) line->status |= 0b10;
#define SET_LRU(line) line->status |= 0b100;
#define CLEAR_LRU(line) line->status &= ~0b100;

struct Cacheline
{
    uint16_t tag;
    uint8_t data[CACHE_LINE_SIZE];
    uint8_t status;
};
typedef struct Cacheline cacheline_t;

cacheline_t cache[64][2];

uint64_t hits, misses;



void cache_read(uint32_t addr, void *ptr, uint8_t size)
{
    uint8_t index = INDEX(addr);
    uint16_t tag = TAG(addr);
    uint8_t offset = OFFSET(addr);

    cacheline_t *line;

    if (tag == LINE_TAG((&cache[index][0])) && IS_VALID((&cache[index][0])))
    {
        line = &(cache[index][0]);
        CLEAR_LRU((&cache[index][0]));
        SET_LRU((&cache[index][1]));
        hits++;
    }
    else if (tag == LINE_TAG((&cache[index][1])) && IS_VALID((&cache[index][1])))
    {
        line = &(cache[index][1]);
        CLEAR_LRU((&cache[index][1]));
        SET_LRU((&cache[index][0]));
        hits++;
    }

    else // miss
    {
        misses++;

        if (IS_LRU((&cache[index][0])))
        {
            line = &(cache[index][0]);
            CLEAR_LRU((&cache[index][0]));
            SET_LRU((&cache[index][1]));
        }

        else
        {
            line = &(cache[index][1]);
            CLEAR_LRU((&cache[index][1]));
            SET_LRU((&cache[index][0]));
        }

        if (IS_VALID(line) && IS_DIRTY(line)) // if line is valid and dirty, flush it to RAM
        {
            // flush line to RAM
            uint32_t flush_base = (index << OFFSET_BITS) | ((uint32_t)(LINE_TAG(line)) << 9);
            SPI_Flash_Write(line->data,flush_base,CACHE_LINE_SIZE);
            //psram_write(flush_base, line->data, CACHE_LINE_SIZE);
        }

        // get line from RAM
        uint32_t base = BASE(addr);
        SPI_Flash_Read(line->data,base,CACHE_LINE_SIZE);
        //psram_read(base, line->data, CACHE_LINE_SIZE);
    
        line->tag = tag; // set the tag of the line
        SET_VALID(line); // mark the line as valid
    }

/*
    if (offset + size > CACHE_LINE_SIZE)
    {
        // printf("cross boundary read!\n");
        size = CACHE_LINE_SIZE - offset;
    }
*/

    for (int i = 0; i < size; i++)
        ((uint8_t *)(ptr))[i] = line->data[offset + i];
}

void cache_write(uint32_t addr, void *ptr, uint8_t size)
{
    uint8_t index = INDEX(addr);
    uint32_t tag = TAG(addr);
    uint8_t offset = OFFSET(addr);

    cacheline_t *line;

    if (tag == LINE_TAG((&cache[index][0])) && IS_VALID((&cache[index][0])))
    {
        line = &(cache[index][0]);
        CLEAR_LRU((&cache[index][0]));
        SET_LRU((&cache[index][1]));
        hits++;
    }
    else if (tag == LINE_TAG((&cache[index][1])) && IS_VALID((&cache[index][1])))
    {
        line = &(cache[index][1]);
        CLEAR_LRU((&cache[index][1]));
        SET_LRU((&cache[index][0]));
        hits++;
    }

    else // miss
    {
        misses++;

        if (IS_LRU((&cache[index][0])))
        {
            line = &(cache[index][0]);
            CLEAR_LRU((&cache[index][0]));
            SET_LRU((&cache[index][1]));
        }

        else
        {
            line = &(cache[index][1]);
            CLEAR_LRU((&cache[index][1]));
            SET_LRU((&cache[index][0]));
        }

        if (IS_VALID(line) && IS_DIRTY(line)) // if line is valid and dirty, flush it to RAM
        {
            // flush line to RAM
            uint32_t flush_base = (index << OFFSET_BITS) | ((uint32_t)(LINE_TAG(line)) << 9);
            //psram_write(flush_base, line->data, CACHE_LINE_SIZE);
            SPI_Flash_Write(line->data,flush_base,CACHE_LINE_SIZE);
        }

        // get line from RAM
        uint32_t base = BASE(addr);
        //psram_read(base, line->data, CACHE_LINE_SIZE);
        SPI_Flash_Read(line->data,base,CACHE_LINE_SIZE);

        line->tag = tag; // set the tag of the line
        SET_VALID(line); // mark the line as valid
    }
/*
    if (offset + size > CACHE_LINE_SIZE)
    {
        // printf("cross boundary write!\n");
        size = CACHE_LINE_SIZE - offset;
    }
*/
    for (int i = 0; i < size; i++)
        line->data[offset + i] = ((uint8_t *)(ptr))[i];
    SET_DIRTY(line); // mark the line as dirty
}

void cache_reset()
{
    for(int i=0; i < 32; i++)
    {
        cache[i][0].status = 0;
        cache[i][1].status = 0;
    }
}

#else 


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
    __disable_irq();     

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
                   //psram_write(*tp & ~0x3f, p, 64);
                   sramwrite(*tp & ~0x3f, p, 64);
                    //SPI_Flash_Write(line->data,flush_base,CACHE_LINE_SIZE);
                }
                sramread(ofs & ~0x3f, p, 64);//psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            sramread(ofs & ~0x3f, p, 64);//psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(p + (ofs & 0x3f), buf, size);
    *tp |= DIRTY;
    __enable_irq();          
}

void cache_read(uint32_t ofs, void *buf, uint32_t size)
{
    __disable_irq();  
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
                    sramwrite(*tp & ~0x3f, p, 64);//psram_write(*tp & ~0x3f, p, 64);
                }
                sramread(ofs & ~0x3f, p, 64);//psram_read(ofs & ~0x3f, p, 64);
                *tp = ofs & ~0x3f;
                *tp |= VALID;
            }
        } else {
            if (i != 1)
                continue;

            ti = i;
            sramread(ofs & ~0x3f, p, 64);//psram_read(ofs & ~0x3f, p, 64);
            *tp = ofs & ~0x3f;
            *tp |= VALID;
        }
    }

    tags[index][1] &= ~(LRU);
    tags[index][1] |= (ti << LRU_SFT);
    memcpy(buf, p + (ofs & 0x3f), size);
    __enable_irq();          
}

void cache_get_stat(uint64_t *phit, uint64_t *paccessed)
{
    *phit = hit;
    *paccessed = accessed;
}


#endif