/* memory.h - roundmb, truncmb, freestk */
#include <kernel.h>
#define MAXADDR		0x2001f400		/* 64kB SRAM */
#define HANDLERSTACK	1024			/* Size reserved for stack in Handler mode */
#define	PAGE_SIZE	1024 // TODO: unused?

/*----------------------------------------------------------------------
 * roundmb, truncmb - Round or truncate address to memory block size
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32)(p)		\
				- ((uint32)roundmb(len))	\
				+ (uint32)sizeof(uint32)),	\
				(uint32)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/


/* Added by linker */

extern	unsigned int	_text;			/* Start of text segment	*/
extern	unsigned int	_etext;			/* End of text segment		*/
extern	unsigned int	_sidata;
extern	unsigned int	_sdata;			/* Start of data segment	*/
extern	unsigned int	_edata;			/* End of data segment		*/
extern	unsigned int	_sbss;			/* Start of bss segment		*/
extern	unsigned int	_ebss;			/* End of bss segment		*/
extern	unsigned int	end;			/* End of program		*/
uint32 heap_free(void);



//extern	unsigned int	_ccmram_start;			/* Start of text segment	*/
//extern	unsigned int	_ccmram_end;			/* End of text segment		*/
#define KMALLOC_START  (void*)0x10000000
#define KMALLOC_LENGTH (size_t)64000

void cc_malloc_init(void *start, int length);
void cc_malloc_debug();
int malloc_test();
void cc_free(void *ptr);
void *cc_malloc(int length);

#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

extern struct kmalloc_chunk *head;
