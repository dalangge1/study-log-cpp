#include <wasm_simd128.h>

extern "C" {

#ifdef __SIMD__
void multiply_arrays(int *out, int *in_a, int *in_b, int size) {
  for (int i = 0; i < size; i += 4) {
    v128_t a = wasm_v128_load(&in_a[i]);
    v128_t b = wasm_v128_load(&in_b[i]);
    v128_t prod = wasm_i32x4_mul(a, b);
    wasm_v128_store(&out[i], prod);
  }
}

float test_f32x4_shuffle(float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_f32x4_make(d, c, b, a);
  v128_t v3 = wasm_i32x4_shuffle(v1, v2, 0, 2, 4, 6);
  return wasm_f32x4_extract_lane(v3, 0) + wasm_f32x4_extract_lane(v3, 1) +
         wasm_f32x4_extract_lane(v3, 2) + wasm_f32x4_extract_lane(v3, 3);
}

float test_f32x4_make(float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_f32x4_make(d, c, b, a);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  return wasm_f32x4_extract_lane(v3, 0) + wasm_f32x4_extract_lane(v3, 1) +
         wasm_f32x4_extract_lane(v3, 2) + wasm_f32x4_extract_lane(v3, 3);
}
#endif

void multiply_arrays_no_simd(int *out, int *in_a, int *in_b, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in_a[i] * in_b[i];
  }
}

void void_fn() {}
}
