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

float test_v128_load(float a, float b, float c, float d) {
  float arr[4];
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
  v128_t v1 = wasm_v128_load(arr);
  arr[0] = d;
  arr[1] = c;
  arr[2] = b;
  arr[3] = a;
  v128_t v2 = wasm_v128_load(arr);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  return wasm_f32x4_extract_lane(v3, 0);
}

float test_v128_load_by_shuffle(float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_i32x4_shuffle(v1, v1, 3, 2, 1, 0);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  return wasm_f32x4_extract_lane(v3, 0);
}

float test_v128_load_by_make(float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_f32x4_make(d, c, b, a);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  return wasm_f32x4_extract_lane(v3, 0);
}

void test_v128_arg(v128_t *v) {
  v128_t v1 = wasm_f32x4_make(1, 2, 3, 4);
  v128_t v2 = wasm_f32x4_make(5, 5, 5, 5);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  *v = v3;
}

void test_v128_arg2(v128_t *v, float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_f32x4_make(a, b, c, d);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  *v = v3;
}

void test_v128_arr_arg(v128_t v[4]) {
  float col0[4] = {0, 1, 2, 3};
  v[0] = wasm_v128_load(col0);

  float col1[4] = {4, 5, 6, 7};
  v[1] = wasm_v128_load(col1);

  float col2[4] = {8, 9, 10, 11};
  v[2] = wasm_v128_load(col2);

  float col3[4] = {12, 13, 14, 15};
  v[3] = wasm_v128_load(col3);
}

void test_v128_arr_arg2(v128_t v[4], float a, float b, float c, float d) {
  v128_t v1 = wasm_f32x4_make(a, b, c, d);
  v128_t v2 = wasm_f32x4_make(a, b, c, d);
  v128_t v3 = wasm_f32x4_mul(v1, v2);
  v[0] = v3;
  v[1] = v3;
  v[2] = v3;
  v[3] = v3;
}
#endif

void multiply_arrays_no_simd(int *out, int *in_a, int *in_b, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in_a[i] * in_b[i];
  }
}

void void_fn() {}
}
