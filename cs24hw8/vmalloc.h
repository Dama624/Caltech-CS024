/*============================================================================
 * Declarations for the simple allocator that sits on top of the virtual
 * memory system.
 */

#ifndef VMALLOC_H
#define VMALLOC_H

void vmem_alloc_init();
void * vmem_alloc(unsigned int size);

#endif /* VMALLOC_H */

