#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <xmmintrin.h>

#define ITERATIONS 10000

#define MULT_FLOAT4(X, Y)                                                      \
  ({                                                                           \
    asm volatile("movaps (%0), %%xmm0\n\t"                                     \
                 "mulps (%1), %%xmm0\n\t"                                      \
                 "movaps %%xmm0, (%1)" ::"r"(X),                               \
                 "r"(Y));                                                      \
  })

int main(void) {
  int i, j;
  float a[4] __attribute__((aligned(16))) = {10, 20, 30, 40};
  float c[4] __attribute__((aligned(16))) = {10, 20, 30, 40};
  const auto va = _mm_set_ps(10, 20, 30, 40);
  auto vc = _mm_set_ps(10, 20, 30, 40);

  auto start = std::chrono::high_resolution_clock::now();
  for (j = 0; j < 5000; ++j)
    for (i = 0; i < ITERATIONS; ++i) {
      float b[4] __attribute__((aligned(16))) = {0.1, 0.1, 0.1, 0.1};

      MULT_FLOAT4(a, b);
    }
  auto sse_time = std::chrono::high_resolution_clock::now() - start;

  start = std::chrono::high_resolution_clock::now();
  float b[4] __attribute__((aligned(16))) = {0.1, 0.1, 0.1, 0.1};
  for (j = 0; j < 5000; ++j)
    for (i = 0; i < ITERATIONS; ++i) {
      // float b[4] __attribute__((aligned(16))) = {0.1, 0.1, 0.1, 0.1};
      // auto vb = _mm_set_ps(0.1, 0.1, 0.1, 0.1);
      auto vb = __extension__(__m128){0.1, 0.1, 0.1, 0.1}; // 60

      // vb = (__m128)((__v4sf)va * (__v4sf)vb); // 60
      // *(__m128 *)b = (__m128)((__v4sf)va * (__v4sf)vb); // 62
      __builtin_nontemporal_store((__v4sf)(__m128)((__v4sf)va * (__v4sf)vb), (__v4sf*)b); // 60
      // vb = _mm_mul_ps(va, vb); // 149
      // vc = _mm_mul_ps(va, vb); // 146
      // _mm_store_ps(b, _mm_mul_ps(va, vb)); // 183
      // _mm_stream_ps(c, _mm_mul_ps(va, vb)); // 5261
      // _mm_stream_ps(a, _mm_mul_ps(va, vb)); // 185
      // _mm_stream_ps(b, _mm_mul_ps(va, vb)); // 185
      // asm volatile("" : "+r,m"(b) : : "memory");
      // asm volatile("" : "+r,m"(vb) : : "memory");
    }
  auto sse2_time = std::chrono::high_resolution_clock::now() - start;

  start = std::chrono::high_resolution_clock::now();
  for (j = 0; j < 5000; ++j)
    for (i = 0; i < ITERATIONS; ++i) {
      float b[4] __attribute__((aligned(16))) = {0.1, 0.1, 0.1, 0.1};

      b[0] *= a[0];
      b[1] *= a[1];
      b[2] *= a[2];
      b[3] *= a[3];
    }
  auto std_time = std::chrono::high_resolution_clock::now() - start;

  std::cout << "sse2_time: "
            << sse2_time.count() / std::chrono::microseconds::period::den
            << '\n';
  std::cout << "sse_time : "
            << sse_time.count() / std::chrono::microseconds::period::den
            << '\n';
  std::cout << "std_time : "
            << std_time.count() / std::chrono::microseconds::period::den
            << '\n';
  return 0;
}