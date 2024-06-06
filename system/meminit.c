/* meminit.c - meminit */

#include <xinu.h>


void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	meminit(void)
{
	struct	memblk *memptr;	/* Memory block pointer	*/

	/* Initialize the minheap and maxheap variables */

	minheap = (void *)&end;
	/* 1024 bytes is reserved for supervise mode handling */
	maxheap = (void *)MAXADDR - HANDLERSTACK;

	/* Initialize the memory list as one big block */

	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;
	memlist.mlength = memptr->mlength =
		(uint32)maxheap - (uint32)minheap;
}



/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall	freemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;

	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			  || ((uint32) blkaddr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	block = (struct memblk *)blkaddr;

	prev = &memlist;			/* Walk along free list	*/
	next = memlist.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlist) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlist) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}
	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}

/*------------------------------------------------------------------------
 *  getstk  -  Allocate stack memory, returning highest word address
 *------------------------------------------------------------------------
 */
char  	*getstk(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr;	/* Walk through memory list	*/
	struct	memblk	*fits, *fitsprev; /* Record block that fits	*/

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	fits = NULL;
	fitsprev = NULL;  /* Just to avoid a compiler warning */

	while (curr != NULL) {			/* Scan entire list	*/
		if (curr->mlength >= nbytes) {	/* Record block address	*/
			fits = curr;		/*   when request fits	*/
			fitsprev = prev;
		}
		prev = curr;
		curr = curr->mnext;
	}

	if (fits == NULL) {			/* No block was found	*/
		restore(mask);
		return (char *)SYSERR;
	}
	if (nbytes == fits->mlength) {		/* Block is exact match	*/
		fitsprev->mnext = fits->mnext;
	} else {				/* Remove top section	*/
		fits->mlength -= nbytes;
		fits = (struct memblk *)((uint32)fits + fits->mlength);
	}
	memlist.mlength -= nbytes;
	restore(mask);
	return (char *)((uint32) fits + nbytes - sizeof(uint32));
}

uint32 heap_free(){
  struct  memblk  *memptr;  /* Ptr to memory block    */
  uint32  free_mem;   /* Total amount of free memory  */
  free_mem = 0;
  for (memptr = memlist.mnext; memptr != NULL; memptr = memptr->mnext) {
       free_mem += memptr->mlength;
  }
  return free_mem;
}



struct kmalloc_chunk *head = 0;

void cc_malloc_init(void *start, int length)
{
  head = (struct kmalloc_chunk *) start;
  head->state = KMALLOC_STATE_FREE;
  head->length = length;
  head->next = 0;
  head->prev = 0;

}

static void ksplit(struct kmalloc_chunk *c, int length)
{
 // uint32 mask= disable();

  struct kmalloc_chunk *n = (struct kmalloc_chunk *) ((char *) c + length);

  n->state = KMALLOC_STATE_FREE;
  n->length = c->length - length;
  n->prev = c;
  n->next = c->next;

  if(c->next)
    c->next->prev = n;

  c->next = n;
  c->length = length;
  //restore(mask);
}

 

void *cc_malloc(int length)
{
  // round up length to a multiple of KUNIT
  int extra = length % KUNIT;
  //uint32 mask= disable();

  if(extra)
    length += (KUNIT - extra);

  // then add one more unit to accommodate the chunk header
  length += KUNIT;

  struct kmalloc_chunk *c = head;

  while(1) {
    if(!c) {
      kprintf("kmalloc: out of memory!\n");
     // restore(mask);
      return NULL;
    }
    if(c->state == KMALLOC_STATE_FREE && c->length >= length)
      break;
    c = c->next;
  }

  // split the chunk if the remainder is greater than two units
  if(length - c->length > 2 * KUNIT) {
    ksplit(c, length);
  }

  c->state = KMALLOC_STATE_USED;

 // restore(mask);
  // return a pointer to the memory following the chunk header
  return (c + 1);
}

 
static void kmerge(struct kmalloc_chunk *c)
{

 // uint32 mask= disable();
  if(!c){
    // restore(mask);
     return;
  }

  if(c->state != KMALLOC_STATE_FREE){
  //    restore(mask);
    return;
  }

  if(c->next && c->next->state == KMALLOC_STATE_FREE) {
    c->length += c->next->length;
    if(c->next->next) {
      c->next->next->prev = c;
    }
    c->next = c->next->next;
  }
 // restore(mask);
}

 
void cc_free(void *ptr)
{

  //uint32 mask= disable();
  struct kmalloc_chunk *c = (struct kmalloc_chunk *) ptr;
  c--;

  if(c->state != KMALLOC_STATE_USED) {
    kprintf("invalid kfree(%x)\n", ptr);
    //restore(mask);
    return;
  }

  c->state = KMALLOC_STATE_FREE;

  kmerge(c);
  kmerge(c->prev);
  //restore(mask);
}

void cc_malloc_debug()
{
   //uint32 mask= disable();
   struct kmalloc_chunk *c;
 
  const  char ps_msg1[] = "\nstate\tptr\tprev\tnext\tlength\t";

  //printf("state ptr      prev     next     length  ptr(%x)\n",KMALLOC_START);
  //kprintf("%s max %d kb\n",ps_msg1,KMALLOC_LENGTH/1000);

  for(c = head; c; c = c->next) {
    if(c->state == KMALLOC_STATE_FREE) {
      kprintf("Free");
    } else if(c->state == KMALLOC_STATE_USED) {
      kprintf("Used");
    } else {
      kprintf("kmalloc list corrupted at %x!\n", c);
      return;
    }
    //printf("  %x         %d       %x       %x       %d\n", c,c, c->prev, c->next, c->length);
    kprintf("\t%x ", c);
    kprintf("\t%d ", c);
    kprintf("\t%x ", c->prev);
    kprintf("\t%x ", c->next);
    kprintf("\t%d kb", c->length/1000);
    kprintf("\n");
  }
  //restore(mask);
}



void* do_malloc (uint32 len) {

    //sem_wait(&sem_malloc);
    chunk_t* it = (chunk_t *)&end;
    chunk_t *ch;

    if (!len) {
        return (NULL);
    }
    for (; it; it = it->next) {
        if (!it->free) { /* occupied */
            continue;
        }
        if (len + sizeof(chunk_t) > it->size) {
            continue; /* free but too small  */
        }
        if (len + sizeof(chunk_t) + sizeof(chunk_t) >= it->size) {
            /* free and just perfect in size, reserve it! */
            it->free = 0;
            //it->pid=getpid();
            //sem_signal(&sem_malloc);
            return ((char*)it + sizeof(chunk_t));
        }
        /* free but big, split it! */
        ch = (chunk_t*)((char*)it + len + sizeof(chunk_t));
        ch->next = it->next;
        it->next = ch;

        ch->free = it->free;

        ch->size = it->size - (len + sizeof(chunk_t));
        it->size -= ch->size;

        it->free = 0;
        //it->pid=getpid();
        //sem_signal(&sem_malloc);
        return ((char*)it + sizeof(chunk_t));
    }
    /* No more free memory */
    //sem_signal(&sem_malloc);
    return (NULL);
}


void do_free (void *p) {
    //sem_wait(&sem_malloc);
    chunk_t* it = (chunk_t *)&end;
    if (!p) {
        return;
    }
    memset(p, 0, sizeof(p));
    /* mark chunk as free */
    ((chunk_t*)((char*)p - sizeof(chunk_t)))->free = 1;

    /* merge free chunks */
    for (; it; it = it->next) {
        while (it->free && it->next && it->next->free) {
            /* merge with next free */
            it->size += it->next->size;
            it->next = it->next->next;
        }
    }
    //it->free = 1;
    //it->pid=getpid();
   // sem_signal(&sem_malloc);
    return;
}


void chunklist_init (uint32 heap_size) {
    chunk_t* it = (chunk_t *)KMALLOC_START;
    it->free = 1;
    it->size = heap_size;
    it->next = NULL;
   // it->pid=-1;
}

void info() {
    chunk_t* it = (chunk_t *)KMALLOC_START;
    chunk_t* ch = it;

    kprintf("============================\n");
    for (; ch; ch = ch->next) {
        kprintf("chunk:%d, size:%d, %s\n",(char*)ch - (char*)it,ch->size,ch->free ? "FREE" : "RESERVED");
    }
    kprintf("============================\n\n");
}
