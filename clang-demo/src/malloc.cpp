#include <stdint.h>

extern unsigned int __heap_base;

unsigned int *bump_pointer = &__heap_base;

void *malloc(unsigned int n)
{
  unsigned int *r = bump_pointer;
  bump_pointer += n;
  return (void *)r;
}

void free(void *p)
{
  // lol
}