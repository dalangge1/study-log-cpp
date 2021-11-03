#include <wasm_simd128.h>

extern "C" {

void mat4_multiply(float const a[16], float const b[16], float out[16]) {
  float b11 = b[0], b12 = b[4], b13 = b[8], b14 = b[12];
  float b21 = b[1], b22 = b[5], b23 = b[9], b24 = b[13];
  float b31 = b[2], b32 = b[6], b33 = b[10], b34 = b[14];
  float b41 = b[3], b42 = b[7], b43 = b[11], b44 = b[15];

  float a1 = a[0], a2 = a[4], a3 = a[8], a4 = a[12];

  out[0] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
  out[4] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
  out[8] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
  out[12] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

  a1 = a[1], a2 = a[5], a3 = a[9], a4 = a[13];

  out[1] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
  out[5] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
  out[9] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
  out[13] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

  a1 = a[2], a2 = a[6], a3 = a[10], a4 = a[14];

  out[2] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
  out[6] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
  out[10] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
  out[14] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

  a1 = a[3], a2 = a[7], a3 = a[11], a4 = a[15];

  out[3] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
  out[7] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
  out[11] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
  out[15] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;
}

#ifdef __SIMD__
float tmpF32[4];

void mat4_multiply_simd(float const ae[16], float const be[16], float te[16]) {
  //   v128_t r0 = wasm_f32x4_replace_lane(
  //       wasm_f32x4_replace_lane(
  //           wasm_f32x4_replace_lane(wasm_f32x4_splat(be[0]), 1, be[4]), 2,
  //           be[8]),
  //       3, be[12]);
  //   v128_t r1 = wasm_f32x4_replace_lane(
  //       wasm_f32x4_replace_lane(
  //           wasm_f32x4_replace_lane(wasm_f32x4_splat(be[1]), 1, be[5]), 2,
  //           be[9]),
  //       3, be[13]);
  //   v128_t r2 = wasm_f32x4_replace_lane(
  //       wasm_f32x4_replace_lane(
  //           wasm_f32x4_replace_lane(wasm_f32x4_splat(be[2]), 1, be[6]), 2,
  //           be[10]),
  //       3, be[14]);
  //   v128_t r3 = wasm_f32x4_replace_lane(
  //       wasm_f32x4_replace_lane(
  //           wasm_f32x4_replace_lane(wasm_f32x4_splat(be[3]), 1, be[7]), 2,
  //           be[11]),
  //       3, be[15]);
  v128_t r0 = wasm_f32x4_make(be[0], be[4], be[8], be[12]);
  v128_t r1 = wasm_f32x4_make(be[1], be[5], be[9], be[13]);
  v128_t r2 = wasm_f32x4_make(be[2], be[6], be[10], be[14]);
  v128_t r3 = wasm_f32x4_make(be[3], be[7], be[11], be[15]);

  v128_t o0 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(ae[0]), r0),
                         wasm_f32x4_mul(wasm_f32x4_splat(ae[4]), r1)),
          wasm_f32x4_mul(wasm_f32x4_splat(ae[8]), r2)),
      wasm_f32x4_mul(wasm_f32x4_splat(ae[12]), r3));
  v128_t o1 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(ae[1]), r0),
                         wasm_f32x4_mul(wasm_f32x4_splat(ae[5]), r1)),
          wasm_f32x4_mul(wasm_f32x4_splat(ae[9]), r2)),
      wasm_f32x4_mul(wasm_f32x4_splat(ae[13]), r3));
  v128_t o2 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(ae[2]), r0),
                         wasm_f32x4_mul(wasm_f32x4_splat(ae[6]), r1)),
          wasm_f32x4_mul(wasm_f32x4_splat(ae[10]), r2)),
      wasm_f32x4_mul(wasm_f32x4_splat(ae[14]), r3));
  v128_t o3 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(ae[3]), r0),
                         wasm_f32x4_mul(wasm_f32x4_splat(ae[7]), r1)),
          wasm_f32x4_mul(wasm_f32x4_splat(ae[11]), r2)),
      wasm_f32x4_mul(wasm_f32x4_splat(ae[15]), r3));

  // 使用shuffle 额不对是变慢了。。。
  //   v128_t v1 = wasm_i32x4_shuffle(
  //       wasm_i32x4_shuffle(wasm_i32x4_shuffle(o0, o1, 0, 4, 0, 0), o2, 0, 1,
  //       4,
  //                          0),
  //       o3, 0, 1, 2, 4);
  //   v128_t v2 = wasm_i32x4_shuffle(
  //       wasm_i32x4_shuffle(wasm_i32x4_shuffle(o0, o1, 1, 5, 0, 0), o2, 0, 1,
  //       5,
  //                          0),
  //       o3, 0, 1, 2, 5);
  //   v128_t v3 = wasm_i32x4_shuffle(
  //       wasm_i32x4_shuffle(wasm_i32x4_shuffle(o0, o1, 2, 6, 0, 0), o2, 0, 1,
  //       6,
  //                          0),
  //       o3, 0, 1, 2, 6);
  //   v128_t v4 = wasm_i32x4_shuffle(
  //       wasm_i32x4_shuffle(wasm_i32x4_shuffle(o0, o1, 3, 7, 0, 0), o2, 0, 1,
  //       7,
  //                          0),
  //       o3, 0, 1, 2, 7);

  //   wasm_v128_store(te, v1);
  //   wasm_v128_store(te + 4, v2);
  //   wasm_v128_store(te + 8, v3);
  //   wasm_v128_store(te + 12, v4);

  wasm_v128_store(&tmpF32, o0);
  te[0] = tmpF32[0];
  te[4] = tmpF32[1];
  te[8] = tmpF32[2];
  te[12] = tmpF32[3];

  wasm_v128_store(&tmpF32, o1);
  te[1] = tmpF32[0];
  te[5] = tmpF32[1];
  te[9] = tmpF32[2];
  te[13] = tmpF32[3];

  wasm_v128_store(&tmpF32, o2);
  te[2] = tmpF32[0];
  te[6] = tmpF32[1];
  te[10] = tmpF32[2];
  te[14] = tmpF32[3];

  wasm_v128_store(&tmpF32, o3);
  te[3] = tmpF32[0];
  te[7] = tmpF32[1];
  te[11] = tmpF32[2];
  te[15] = tmpF32[3];

  // store 比 extrace 快5ms

  // te[0] = wasm_f32x4_extract_lane(o0, 0);
  // te[4] = wasm_f32x4_extract_lane(o0, 1);
  // te[8] = wasm_f32x4_extract_lane(o0, 2);
  // te[12] = wasm_f32x4_extract_lane(o0, 3);

  // te[1] = wasm_f32x4_extract_lane(o1, 0);
  // te[5] = wasm_f32x4_extract_lane(o1, 1);
  // te[9] = wasm_f32x4_extract_lane(o1, 2);
  // te[13] = wasm_f32x4_extract_lane(o1, 3);

  // te[2] = wasm_f32x4_extract_lane(o2, 0);
  // te[6] = wasm_f32x4_extract_lane(o2, 1);
  // te[10] = wasm_f32x4_extract_lane(o2, 2);
  // te[14] = wasm_f32x4_extract_lane(o2, 3);

  // te[3] = wasm_f32x4_extract_lane(o3, 0);
  // te[7] = wasm_f32x4_extract_lane(o3, 1);
  // te[11] = wasm_f32x4_extract_lane(o3, 2);
  // te[15] = wasm_f32x4_extract_lane(o3, 3);
}

void mat4_multiply_simd2(float const a[16], float const b[16], float out[16]) {
  v128_t a_col_0 = wasm_v128_load(a);
  v128_t a_col_1 = wasm_v128_load(a + 4);
  v128_t a_col_2 = wasm_v128_load(a + 8);
  v128_t a_col_3 = wasm_v128_load(a + 12);

  v128_t o_col_0 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(b[0]), a_col_0),
                         wasm_f32x4_mul(wasm_f32x4_splat(b[1]), a_col_1)),
          wasm_f32x4_mul(wasm_f32x4_splat(b[2]), a_col_2)),
      wasm_f32x4_mul(wasm_f32x4_splat(b[3]), a_col_3));
  v128_t o_col_1 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(b[4]), a_col_0),
                         wasm_f32x4_mul(wasm_f32x4_splat(b[5]), a_col_1)),
          wasm_f32x4_mul(wasm_f32x4_splat(b[6]), a_col_2)),
      wasm_f32x4_mul(wasm_f32x4_splat(b[7]), a_col_3));
  v128_t o_col_2 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(b[8]), a_col_0),
                         wasm_f32x4_mul(wasm_f32x4_splat(b[9]), a_col_1)),
          wasm_f32x4_mul(wasm_f32x4_splat(b[10]), a_col_2)),
      wasm_f32x4_mul(wasm_f32x4_splat(b[11]), a_col_3));
  v128_t o_col_3 = wasm_f32x4_add(
      wasm_f32x4_add(
          wasm_f32x4_add(wasm_f32x4_mul(wasm_f32x4_splat(b[12]), a_col_0),
                         wasm_f32x4_mul(wasm_f32x4_splat(b[13]), a_col_1)),
          wasm_f32x4_mul(wasm_f32x4_splat(b[14]), a_col_2)),
      wasm_f32x4_mul(wasm_f32x4_splat(b[15]), a_col_3));

  wasm_v128_store(out, o_col_0);
  wasm_v128_store(out + 4, o_col_1);
  wasm_v128_store(out + 8, o_col_2);
  wasm_v128_store(out + 12, o_col_3);
}

// code from
// https://github.com/ngzhian/simd-benchmarks/blob/master/matrix_multiply_intrinsics.cpp
// 完全用simd的指令
void bench_simd(double T1x[], double T2x[], double Outx[]) {
  v128_t a0 = wasm_v128_load(T1x + 0);
  v128_t a1 = wasm_v128_load(T1x + 2);
  v128_t a2 = wasm_v128_load(T1x + 4);
  v128_t a3 = wasm_v128_load(T1x + 6);
  v128_t a4 = wasm_v128_load(T1x + 8);
  v128_t a5 = wasm_v128_load(T1x + 10);
  v128_t a6 = wasm_v128_load(T1x + 12);
  v128_t a7 = wasm_v128_load(T1x + 14);

  v128_t b0 = wasm_v128_load(T2x + 0);
  v128_t b1 = wasm_v128_load(T2x + 2);

  wasm_v128_store(
      Outx + 0,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 0)), a0),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 1)),
                             a2),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 0)), a4),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 1)), a6)))));
  wasm_v128_store(
      Outx + 2,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 0)), a1),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 1)),
                             a3),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 0)), a5),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 1)), a7)))));

  v128_t b2 = wasm_v128_load(T2x + 4);
  v128_t b3 = wasm_v128_load(T2x + 6);

  wasm_v128_store(
      Outx + 4,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 0)), a0),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 1)),
                             a2),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 0)), a4),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 1)), a6)))));
  wasm_v128_store(
      Outx + 6,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 0)), a1),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 1)),
                             a3),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 0)), a5),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 1)), a7)))));

  v128_t b4 = wasm_v128_load(T2x + 8);
  v128_t b5 = wasm_v128_load(T2x + 10);

  wasm_v128_store(
      Outx + 8,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 0)), a0),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 1)),
                             a2),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 0)), a4),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 1)), a6)))));
  wasm_v128_store(
      Outx + 10,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 0)), a1),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 1)),
                             a3),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 0)), a5),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 1)), a7)))));

  v128_t b6 = wasm_v128_load(T2x + 12);
  v128_t b7 = wasm_v128_load(T2x + 14);

  wasm_v128_store(
      Outx + 12,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 0)), a0),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 1)),
                             a2),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 0)), a4),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 1)), a6)))));
  wasm_v128_store(
      Outx + 14,
      wasm_f64x2_add(
          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 0)), a1),
          wasm_f64x2_add(
              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 1)),
                             a3),
              wasm_f64x2_add(
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 0)), a5),
                  wasm_f64x2_mul(
                      wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 1)), a7)))));
}

float m1[16] = {2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2};
float m2[16] = {1, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 1};
float m3[16] = {2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2};
double m4[16] = {2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2};
double m5[16] = {1, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 1};
double m6[16] = {2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2};

void test(int type, int loop) {
  int i;
  float *m1Ptr = m1;
  float *m2Ptr = m2;
  float *tmp1Ptr;

  double *m4Ptr = m4;
  double *m5Ptr = m5;
  double *tmp2Ptr;

  if (type == 0)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      mat4_multiply(m1, m2, m3);
    }

  if (type == 1)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      mat4_multiply_simd(m1Ptr, m2Ptr, m3);
    }

  if (type == 2)
    for (i = 0; i < loop; i++) {
      // 若无下面3行, bench_simd是不耗时
      tmp2Ptr = m4Ptr;
      m4Ptr = m5Ptr;
      m5Ptr = tmp2Ptr;
      bench_simd(m4Ptr, m5Ptr, m6);
    }

  if (type == 3)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      mat4_multiply_simd2(m1Ptr, m2Ptr, m3);
    }
}

#endif
}
