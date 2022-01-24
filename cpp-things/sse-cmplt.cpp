#include <iostream>
#include <xmmintrin.h>

int main(void) {
  __m128 v0 = _mm_set_ps(2, 1, 0, 0);
  __m128 v1 = _mm_set_ps(0, 0, 0, 0);
  // __m128 v2 = _mm_cmplt_ps(v0, v1);
  __m128 v2 = _mm_cmpeq_ps(v0, v1);
  v2 = _mm_and_ps(v2, _mm_set1_ps(1));
  v2 = _mm_add_ps(v0, v2);
  
  float out[4];
  _mm_store_ps(out, v2);
  std::cout << out[0] << ',' << out[1] << ',' << out[2] << ',' << out[3];
  return 0;
}