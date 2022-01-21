#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

#define TIMES 100
#define SIZE 1024

std::uint64_t rdtsc() {
  std::uint32_t lo, hi;

  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (std::uint64_t)hi << 32 | lo;
}

int myfunction() {
  int i;
  char *p = NULL;
  for (i = 0; i < TIMES; i++) {
    p = (char *)malloc(SIZE * sizeof(char));
    if (p) {
      free(p);
    } else {
      printf("malloc failed when i = %d\n", i);
    }
  }
  return 0;
}
int test_rdtsc() {
  std::uint64_t begin;
  std::uint64_t end;

  begin = rdtsc();
  myfunction();
  end = rdtsc();
  printf("myfunction cost %llu CPU cycles\n", end - begin);
  return 0;
}

int main() {
  test_rdtsc();
  return 0;
}