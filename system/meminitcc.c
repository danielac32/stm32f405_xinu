/* meminit.c - meminit */

#include <xinu.h>


void	*minheapcc;	/* Start address of heap	*/
void	*maxheapcc;	/* End address of heap		*/


void	meminitcc(void)
{
	struct	memblkcc *memptr;	/* Memory block pointer	*/

	/* Initialize the minheapcc and maxheapcc variables */

	minheapcc = (void *)0x10000000;
	/* 1024 bytes is reserved for supervise mode handling */
	maxheapcc = (void *)MAXADDRCC;

	/* Initialize the memory list as one big block */

	memlistcc.mnext = (struct memblkcc *)minheapcc;
	memptr = memlistcc.mnext;

	memptr->mnext = (struct memblkcc *)NULL;
	memlistcc.mlength = memptr->mlength =
		(uint32)maxheapcc - (uint32)minheapcc;
}

 
syscall	freememcc(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblkcc	*next, *prev, *block;
	uint32	top;

	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheapcc)
			  || ((uint32) blkaddr > (uint32) maxheapcc)) {
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblkcc multiples	*/
	block = (struct memblkcc *)blkaddr;

	prev = &memlistcc;			/* Walk along free list	*/
	next = memlistcc.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlistcc) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlistcc) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlistcc.mlength += nbytes;

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

 
char  	*getmemcc(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblkcc	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblkcc multiples	*/

	prev = &memlistcc;
	curr = memlistcc.mnext;
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlistcc.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblkcc *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlistcc.mlength -= nbytes;
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

 
char  	*getstkcc(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblkcc	*prev, *curr;	/* Walk through memory list	*/
	struct	memblkcc	*fits, *fitsprev; /* Record block that fits	*/

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use mblock multiples	*/

	prev = &memlistcc;
	curr = memlistcc.mnext;
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
		fits = (struct memblkcc *)((uint32)fits + fits->mlength);
	}
	memlistcc.mlength -= nbytes;
	restore(mask);
	return (char *)((uint32) fits + nbytes - sizeof(uint32));
}

uint32 heap_freecc(){
  struct  memblkcc  *memptr;  /* Ptr to memory block    */
  uint32  free_mem;   /* Total amount of free memory  */
  free_mem = 0;
  for (memptr = memlistcc.mnext; memptr != NULL; memptr = memptr->mnext) {
       free_mem += memptr->mlength;
  }
  return free_mem;
}


