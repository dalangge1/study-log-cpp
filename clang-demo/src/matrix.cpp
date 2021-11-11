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

float mat4_determinant(float const te[16]) {
  float n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  float n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  float n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  float n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  // 复用部分乘法结果
  // // m0
  // const n14n23 = n14 * n23;
  // const n12n24 = n12 * n24;
  // const n13n22 = n13 * n22;
  // const n11n23 = n11 * n23;
  // // m1
  // const n13n24 = n13 * n24;
  // const n14n22 = n14 * n22;
  // const n12n23 = n12 * n23;
  // const n13n21 = n13 * n21;
  // // m2
  // const n14n21 = n14 * n21;
  // const n12n21 = n12 * n21;
  // // m3
  // const n11n24 = n11 * n24;
  // const n11n22 = n11 * n22;
  // s0
  float n14n23n13n24 = n14 * n23 - n13 * n24;
  float n12n24n14n22 = n12 * n24 - n14 * n22;
  float n13n22n12n23 = n13 * n22 - n12 * n23;
  float n11n23n13n21 = n11 * n23 - n13 * n21;
  // s1
  float n14n21n11n24 = n14 * n21 - n11 * n24;
  float n12n21n11n22 = n12 * n21 - n11 * n22;
  // m4
  float n14n23n13n24n32 = n14n23n13n24 * n32;
  float n12n24n14n22n33 = n12n24n14n22 * n33;
  float n13n22n12n23n34 = n13n22n12n23 * n34;
  float n11n23n13n21n34 = n11n23n13n21 * n34;
  // m5
  float n14n23n13n24n31 = n14n23n13n24 * n31;
  float n12n24n14n22n31 = n12n24n14n22 * n31;
  float n13n22n12n23n31 = n13n22n12n23 * n31;
  float n11n23n13n21n32 = n11n23n13n21 * n32;
  // m6
  float n14n21n11n24n33 = n14n21n11n24 * n33;
  float n12n21n11n22n34 = n12n21n11n22 * n34;
  // m7
  float n14n21n11n24n32 = n14n21n11n24 * n32;
  float n12n21n11n22n33 = n12n21n11n22 * n33;

  return (n41 * (n14n23n13n24n32 + n12n24n14n22n33 + n13n22n12n23n34) +
          n42 * (n11n23n13n21n34 + n14n21n11n24n33 - n14n23n13n24n31) +
          n43 * (-n14n21n11n24n32 + n12n21n11n22n34 - n12n24n14n22n31) +
          n44 * (-n13n22n12n23n31 - n11n23n13n21n32 - n12n21n11n22n33));
}

// 看了glm版本,理顺了最后面的部分, 不过glm最后是点乘
float mat4_determinant2(float const te[16]) {
  float n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  float n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  float n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  float n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  // s1
  float n14n23n13n24 = n14 * n23 - n13 * n24;
  float n14n21n11n24 = n14 * n21 - n11 * n24;
  float n12n21n11n22 = n12 * n21 - n11 * n22;
  float n13n22n12n23 = n12 * n23 - n13 * n22;
  // s1
  float n11n23n13n21 = n13 * n21 - n11 * n23;
  float n12n24n14n22 = n12 * n24 - n14 * n22;

  float n14n23n13n24n32 = n14n23n13n24 * n32;
  float n14n21n11n24n32 = n14n21n11n24 * n32;
  float n12n21n11n22n34 = n12n21n11n22 * n34;
  float n13n22n12n23n34 = n13n22n12n23 * n34;

  float n14n23n13n24n31 = n14n23n13n24 * n31;
  float n14n21n11n24n33 = n14n21n11n24 * n33;
  float n12n21n11n22n33 = n12n21n11n22 * n33;
  float n13n22n12n23n31 = n13n22n12n23 * n31;

  float n11n23n13n21n34 = n11n23n13n21 * n34;
  float n11n23n13n21n32 = n11n23n13n21 * n32;
  float n12n24n14n22n31 = n12n24n14n22 * n31;
  float n12n24n14n22n33 = n12n24n14n22 * n33;

  return (n41 * (n14n23n13n24n32 + n12n24n14n22n33 - n13n22n12n23n34) +
          n42 * (-n14n23n13n24n31 + n14n21n11n24n33 - n11n23n13n21n34) +
          n43 * (-n14n21n11n24n32 + n12n21n11n22n34 - n12n24n14n22n31) +
          n44 * (n11n23n13n21n32 + n13n22n12n23n31 - n12n21n11n22n33));
}

// 这个版本非常利于simd的设置f32x4数据, 不过需要额外乘1个系数f32x4 改变符号
// clang-format off
float mat4_invert_det(float const te[16]) {
  float n11 = te[0], n21 = te[1], n31 = te[2], n41 = te[3], n12 = te[4],
        n22 = te[5], n32 = te[6], n42 = te[7], n13 = te[8], n23 = te[9],
        n33 = te[10], n43 = te[11], n14 = te[12], n24 = te[13], n34 = te[14],
        n44 = te[15],

        t11 =   (n34 * n23 - n33 * n24) * n42 - (n34 * n22 - n32 * n24) * n43 + (n33 * n22 - n32 * n23) * n44,
        t12 =   (n14 * n33 - n13 * n34) * n42 - (n14 * n32 - n12 * n34) * n43 + (n13 * n32 - n12 * n33) * n44,
        t13 = -((n14 * n23 - n13 * n24) * n42 - (n14 * n22 - n12 * n24) * n43 + (n13 * n22 - n12 * n23) * n44),
        t14 =   (n14 * n23 - n13 * n24) * n32 - (n14 * n22 - n12 * n24) * n33 + (n13 * n22 - n12 * n23) * n34;

  return n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
}
// clang-format on

float mat4_det_glmatrix(float const te[16]) {
  float a00 = te[0], a10 = te[1], a20 = te[2], a30 = te[3], a01 = te[4],
        a11 = te[5], a21 = te[6], a31 = te[7], a02 = te[8], a12 = te[9],
        a22 = te[10], a32 = te[11], a03 = te[12], a13 = te[13], a23 = te[14],
        a33 = te[15];

  float b00 = a00 * a11 - a01 * a10;
  float b01 = a00 * a12 - a02 * a10;
  float b02 = a00 * a13 - a03 * a10;
  float b03 = a01 * a12 - a02 * a11;
  float b04 = a01 * a13 - a03 * a11;
  float b05 = a02 * a13 - a03 * a12;
  float b06 = a20 * a31 - a21 * a30;
  float b07 = a20 * a32 - a22 * a30;
  float b08 = a20 * a33 - a23 * a30;
  float b09 = a21 * a32 - a22 * a31;
  float b10 = a21 * a33 - a23 * a31;
  float b11 = a22 * a33 - a23 * a32;

  // Calculate the determinant
  return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
}

float mat4_determinant_babylon(float const m[16]) {
  float m00 = m[0], m01 = m[1], m02 = m[2], m03 = m[3];
  float m10 = m[4], m11 = m[5], m12 = m[6], m13 = m[7];
  float m20 = m[8], m21 = m[9], m22 = m[10], m23 = m[11];
  float m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];
  // https://en.wikipedia.org/wiki/Laplace_expansion
  // to compute the deterrminant of a 4x4 Matrix we compute the cofactors of any
  // row or column, then we multiply each Cofactor by its corresponding matrix
  // value and sum them all to get the determinant Cofactor(i, j) = sign(i,j) *
  // det(Minor(i, j)) where
  //  - sign(i,j) = (i+j) % 2 === 0 ? 1 : -1
  //  - Minor(i, j) is the 3x3 matrix we get by removing row i and column j from
  //  current Matrix
  //
  // Here we do that for the 1st row.

  float det_22_33 = m22 * m33 - m32 * m23;
  float det_21_33 = m21 * m33 - m31 * m23;
  float det_21_32 = m21 * m32 - m31 * m22;
  float det_20_33 = m20 * m33 - m30 * m23;
  float det_20_32 = m20 * m32 - m22 * m30;
  float det_20_31 = m20 * m31 - m30 * m21;
  float cofact_00 = +(m11 * det_22_33 - m12 * det_21_33 + m13 * det_21_32);
  float cofact_01 = -(m10 * det_22_33 - m12 * det_20_33 + m13 * det_20_32);
  float cofact_02 = +(m10 * det_21_33 - m11 * det_20_33 + m13 * det_20_31);
  float cofact_03 = -(m10 * det_21_32 - m11 * det_20_32 + m12 * det_20_31);
  return (m00 * cofact_00 + m01 * cofact_01 + m02 * cofact_02 +
          m03 * cofact_03);
}

#ifdef __SIMD__
float tmpF32[16];

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

v128_t mat4_v128_1[4];
v128_t mat4_v128_2[4];
v128_t mat4_v128_3[4];

v128_t *set_mat4_v128(int i, float const a[16]) {
  if (i == 1) {
    mat4_v128_1[0] = wasm_v128_load(a);
    mat4_v128_1[1] = wasm_v128_load(a + 4);
    mat4_v128_1[2] = wasm_v128_load(a + 8);
    mat4_v128_1[3] = wasm_v128_load(a + 12);
    return mat4_v128_1;
  }
  if (i == 2) {
    mat4_v128_2[0] = wasm_v128_load(a);
    mat4_v128_2[1] = wasm_v128_load(a + 4);
    mat4_v128_2[2] = wasm_v128_load(a + 8);
    mat4_v128_2[3] = wasm_v128_load(a + 12);
    return mat4_v128_2;
  }
  if (i == 3) {
    mat4_v128_3[0] = wasm_v128_load(a);
    mat4_v128_3[1] = wasm_v128_load(a + 4);
    mat4_v128_3[2] = wasm_v128_load(a + 8);
    mat4_v128_3[3] = wasm_v128_load(a + 12);
    return mat4_v128_3;
  }
}

void mat4_multiply_simd_glm(v128_t const a[4], v128_t const b[4],
                            v128_t out[4]) {
  {
    v128_t e0 = wasm_i32x4_shuffle(b[0], b[0], 0, 0, 0, 0);
    v128_t e1 = wasm_i32x4_shuffle(b[0], b[0], 1, 1, 1, 1);
    v128_t e2 = wasm_i32x4_shuffle(b[0], b[0], 2, 2, 2, 2);
    v128_t e3 = wasm_i32x4_shuffle(b[0], b[0], 3, 3, 3, 3);

    v128_t m0 = wasm_f32x4_mul(a[0], e0);
    v128_t m1 = wasm_f32x4_mul(a[1], e1);
    v128_t m2 = wasm_f32x4_mul(a[2], e2);
    v128_t m3 = wasm_f32x4_mul(a[3], e3);

    v128_t a0 = wasm_f32x4_add(m0, m1);
    v128_t a1 = wasm_f32x4_add(m2, m3);
    v128_t a2 = wasm_f32x4_add(a0, a1);

    out[0] = a2;
  }

  {
    v128_t e0 = wasm_i32x4_shuffle(b[1], b[1], 0, 0, 0, 0);
    v128_t e1 = wasm_i32x4_shuffle(b[1], b[1], 1, 1, 1, 1);
    v128_t e2 = wasm_i32x4_shuffle(b[1], b[1], 2, 2, 2, 2);
    v128_t e3 = wasm_i32x4_shuffle(b[1], b[1], 3, 3, 3, 3);

    v128_t m0 = wasm_f32x4_mul(a[0], e0);
    v128_t m1 = wasm_f32x4_mul(a[1], e1);
    v128_t m2 = wasm_f32x4_mul(a[2], e2);
    v128_t m3 = wasm_f32x4_mul(a[3], e3);

    v128_t a0 = wasm_f32x4_add(m0, m1);
    v128_t a1 = wasm_f32x4_add(m2, m3);
    v128_t a2 = wasm_f32x4_add(a0, a1);

    out[1] = a2;
  }

  {
    v128_t e0 = wasm_i32x4_shuffle(b[2], b[2], 0, 0, 0, 0);
    v128_t e1 = wasm_i32x4_shuffle(b[2], b[2], 1, 1, 1, 1);
    v128_t e2 = wasm_i32x4_shuffle(b[2], b[2], 2, 2, 2, 2);
    v128_t e3 = wasm_i32x4_shuffle(b[2], b[2], 3, 3, 3, 3);

    v128_t m0 = wasm_f32x4_mul(a[0], e0);
    v128_t m1 = wasm_f32x4_mul(a[1], e1);
    v128_t m2 = wasm_f32x4_mul(a[2], e2);
    v128_t m3 = wasm_f32x4_mul(a[3], e3);

    v128_t a0 = wasm_f32x4_add(m0, m1);
    v128_t a1 = wasm_f32x4_add(m2, m3);
    v128_t a2 = wasm_f32x4_add(a0, a1);

    out[2] = a2;
  }

  {
    v128_t e0 = wasm_i32x4_shuffle(b[3], b[3], 0, 0, 0, 0);
    v128_t e1 = wasm_i32x4_shuffle(b[3], b[3], 1, 1, 1, 1);
    v128_t e2 = wasm_i32x4_shuffle(b[3], b[3], 2, 2, 2, 2);
    v128_t e3 = wasm_i32x4_shuffle(b[3], b[3], 3, 3, 3, 3);

    v128_t m0 = wasm_f32x4_mul(a[0], e0);
    v128_t m1 = wasm_f32x4_mul(a[1], e1);
    v128_t m2 = wasm_f32x4_mul(a[2], e2);
    v128_t m3 = wasm_f32x4_mul(a[3], e3);

    v128_t a0 = wasm_f32x4_add(m0, m1);
    v128_t a1 = wasm_f32x4_add(m2, m3);
    v128_t a2 = wasm_f32x4_add(a0, a1);

    out[3] = a2;
  }
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

float mat4_determinant_simd(float const te[16]) {
  v128_t n_1 = wasm_v128_load(te);
  v128_t n_2 = wasm_v128_load(te + 4);
  v128_t n_3 = wasm_v128_load(te + 8);
  v128_t n_4 = wasm_v128_load(te + 12);

  v128_t tmp0 = wasm_i32x4_shuffle(n_1, n_2, 0, 4, 1, 5);
  v128_t tmp2 = wasm_i32x4_shuffle(n_1, n_2, 2, 6, 3, 7);
  v128_t tmp1 = wasm_i32x4_shuffle(n_3, n_4, 0, 4, 1, 5);
  v128_t tmp3 = wasm_i32x4_shuffle(n_3, n_4, 2, 6, 3, 7);

  v128_t n1_ = wasm_i32x4_shuffle(tmp0, tmp1, 0, 1, 4, 5);
  v128_t n2_ = wasm_i32x4_shuffle(tmp0, tmp1, 2, 3, 6, 7);
  v128_t n3_ = wasm_i32x4_shuffle(tmp2, tmp3, 0, 1, 4, 5);
  v128_t n4_ = wasm_i32x4_shuffle(tmp2, tmp3, 2, 3, 6, 7);

  v128_t m0_l = wasm_i32x4_shuffle(n1_, n1_, 3, 1, 2, 0);
  v128_t m0_r = wasm_i32x4_shuffle(n2_, n2_, 3, 4, 2, 3);

  v128_t m1_l = wasm_i32x4_shuffle(n1_, n1_, 2, 3, 1, 2);
  v128_t m1_r = wasm_i32x4_shuffle(n2_, n2_, 3, 1, 2, 0);

  v128_t m2_l = wasm_i32x4_shuffle(n1_, n1_, 3, 1, 0, 0);
  v128_t m2_r = wasm_i32x4_shuffle(n2_, n2_, 0, 0, 3, 1);

  v128_t m0 = wasm_f32x4_mul(m0_l, m0_r);
  v128_t m1 = wasm_f32x4_mul(m1_l, m1_r);
  v128_t m2 = wasm_f32x4_mul(m2_l, m2_r);

  v128_t m3 = wasm_i32x4_shuffle(m2, m2, 2, 3, 0, 0);

  v128_t s0 = wasm_f32x4_sub(m0, m1);
  v128_t s1 = wasm_f32x4_sub(m3, m2);

  v128_t m4_r = wasm_i32x4_shuffle(n3_, n3_, 1, 2, 3, 3);
  v128_t m5_r = wasm_i32x4_shuffle(n3_, n3_, 0, 0, 0, 1);
  v128_t m6_r = wasm_i32x4_shuffle(n3_, n3_, 2, 3, 1, 2);
  v128_t m6_l = wasm_i32x4_shuffle(s1, s1, 0, 1, 0, 1);

  v128_t m4 = wasm_f32x4_mul(s0, m4_r);
  v128_t m5 = wasm_f32x4_mul(s0, m5_r);
  v128_t m6 = wasm_f32x4_mul(m6_l, m6_r);

  // 更慢了
  // v128_t m7_1 = wasm_i32x4_shuffle(m4, m6, 0, 3, 6, 0);
  // v128_t m7_2 = wasm_i32x4_shuffle(m7_1, m5, 0, 1, 2, 6);
  // v128_t m7_3 = wasm_f32x4_const(1, 1, -1, -1);
  // v128_t m7 = wasm_f32x4_mul(m7_3, m7_2);

  // v128_t m8_1 = wasm_i32x4_shuffle(m4, m6, 1, 4, 5, 0);
  // v128_t m8_2 = wasm_i32x4_shuffle(m8_1, m5, 0, 1, 2, 7);
  // v128_t m8_3 = wasm_f32x4_const(1, 1, 1, -1);
  // v128_t m8 = wasm_f32x4_mul(m8_3, m8_2);

  // v128_t m9_1 = wasm_i32x4_shuffle(m4, m6, 2, 7, 0, 0);
  // v128_t m9_2 = wasm_i32x4_shuffle(m9_1, m5, 0, 4, 5, 1);
  // v128_t m9_3 = wasm_f32x4_const(1, -1, -1, -1);
  // v128_t m9 = wasm_f32x4_mul(m9_3, m9_2);

  // v128_t m10 = wasm_f32x4_add(wasm_f32x4_add(m7, m8), m9);
  // v128_t m11 = wasm_f32x4_mul(n4_, m10);

  //  wasm_v128_store(tmpF32, m11);

  // return tmpF32[0] + tmpF32[1] + tmpF32[2] + tmpF32[3];

  // 部分计算还是非simd
  wasm_v128_store(tmpF32, m4);
  wasm_v128_store(tmpF32 + 4, m5);
  wasm_v128_store(tmpF32 + 8, m6);

  v128_t m8_r = wasm_f32x4_make(
      tmpF32[0] + tmpF32[1] + tmpF32[2], tmpF32[3] + tmpF32[8] - tmpF32[4],
      -tmpF32[10] + tmpF32[9] - tmpF32[5], -tmpF32[6] - tmpF32[7] - tmpF32[11]);

  v128_t m8 = wasm_f32x4_mul(n4_, m8_r);

  // 怎么方便一个向量求和呢
  wasm_v128_store(tmpF32, m8);

  return tmpF32[0] + tmpF32[1] + tmpF32[2] + tmpF32[3];

  // return (n41 * (tmpF32[0] + tmpF32[1] + tmpF32[2]) +
  //         n42 * (tmpF32[3] + tmpF32[8] - tmpF32[4]) +
  //         n43 * (-tmpF32[10] + tmpF32[9] - tmpF32[5]) +
  //         n44 * (-tmpF32[6] - tmpF32[7] - tmpF32[11]));

  // float n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  // float n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  // float n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  // float n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  // 复用部分乘法结果
  // // m0
  // const n14n23 = n14 * n23;
  // const n12n24 = n12 * n24;
  // const n13n22 = n13 * n22;
  // const n11n23 = n11 * n23;
  // // m1
  // const n13n24 = n13 * n24;
  // const n14n22 = n14 * n22;
  // const n12n23 = n12 * n23;
  // const n13n21 = n13 * n21;
  // // m2
  // const n14n21 = n14 * n21;
  // const n12n21 = n12 * n21;
  // const n11n24 = n11 * n24;
  // const n11n22 = n11 * n22;
  // // s0
  // float n14n23n13n24 = n14 * n23 - n13 * n24;
  // float n12n24n14n22 = n12 * n24 - n14 * n22;
  // float n13n22n12n23 = n13 * n22 - n12 * n23;
  // float n11n23n13n21 = n11 * n23 - n13 * n21;
  // // s1
  // float n14n21n11n24 = n14 * n21 - n11 * n24;
  // float n12n21n11n22 = n12 * n21 - n11 * n22;
  // // m4 0
  // float n14n23n13n24n32 = n14n23n13n24 * n32;
  // float n12n24n14n22n33 = n12n24n14n22 * n33;
  // float n13n22n12n23n34 = n13n22n12n23 * n34;
  // float n11n23n13n21n34 = n11n23n13n21 * n34;
  // // m5 4
  // float n14n23n13n24n31 = n14n23n13n24 * n31;
  // float n12n24n14n22n31 = n12n24n14n22 * n31;
  // float n13n22n12n23n31 = n13n22n12n23 * n31;
  // float n11n23n13n21n32 = n11n23n13n21 * n32;
  // // m6 8
  // float n14n21n11n24n33 = n14n21n11n24 * n33;
  // float n12n21n11n22n34 = n12n21n11n22 * n34;
  // float n14n21n11n24n32 = n14n21n11n24 * n32;
  // float n12n21n11n22n33 = n12n21n11n22 * n33;

  // return (n41 * (n14n23n13n24n32 + n12n24n14n22n33 + n13n22n12n23n34) +
  //         n42 * (n11n23n13n21n34 + n14n21n11n24n33 - n14n23n13n24n31) +
  //         n43 * (-n14n21n11n24n32 + n12n21n11n22n34 - n12n24n14n22n31) +
  //         n44 * (-n13n22n12n23n31 - n11n23n13n21n32 - n12n21n11n22n33));
}

// assemblyscript时候的写法
float mat4_determinant_simd2(float const te[16]) {
  float n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  float n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  float n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  float n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  v128_t l, r, m0, m1, m2, m3, m4, m5, m6, m7, s0, s1;

  l = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(
          wasm_f32x4_replace_lane(wasm_f32x4_splat(n14), 1, n12), 2, n13),
      3, n11);
  r = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(wasm_f32x4_splat(n23), 1, n24), 2, n22);
  m0 = wasm_f32x4_mul(l, r);

  l = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(wasm_f32x4_splat(n13), 1, n14), 2, n12);
  r = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(
          wasm_f32x4_replace_lane(wasm_f32x4_splat(n24), 1, n22), 2, n23),
      3, n21);
  m1 = wasm_f32x4_mul(l, r);

  l = wasm_f32x4_replace_lane(wasm_f32x4_splat(n14), 1, n12);
  r = wasm_f32x4_splat(n21);
  m2 = wasm_f32x4_mul(l, r);

  l = wasm_f32x4_splat(n11);
  r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n22), 0, n24);
  m3 = wasm_f32x4_mul(l, r);

  s0 = wasm_f32x4_sub(m0, m1);
  s1 = wasm_f32x4_sub(m2, m3);

  r = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(wasm_f32x4_splat(n34), 0, n32), 1, n33);
  m4 = wasm_f32x4_mul(s0, r);

  r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n31), 3, n32);
  m5 = wasm_f32x4_mul(s0, r);

  r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n33), 1, n34);
  m6 = wasm_f32x4_mul(s1, r);

  r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n32), 1, n33);
  m7 = wasm_f32x4_mul(s1, r);

  l = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(
          wasm_f32x4_replace_lane(wasm_f32x4_splat(n41), 1, n42), 2, n43),
      3, n44);
  r = wasm_f32x4_replace_lane(
      wasm_f32x4_replace_lane(
          wasm_f32x4_replace_lane(
              wasm_f32x4_splat(wasm_f32x4_extract_lane(m4, 0) +
                               wasm_f32x4_extract_lane(m4, 1) +
                               wasm_f32x4_extract_lane(m4, 2)),
              1,
              wasm_f32x4_extract_lane(m4, 3) + wasm_f32x4_extract_lane(m6, 0) -
                  wasm_f32x4_extract_lane(m5, 0)),
          2,
          -wasm_f32x4_extract_lane(m7, 0) + wasm_f32x4_extract_lane(m6, 1) -
              wasm_f32x4_extract_lane(m5, 1)),
      3,
      -wasm_f32x4_extract_lane(m7, 1) - wasm_f32x4_extract_lane(m5, 3) -
          wasm_f32x4_extract_lane(m5, 2));

  l = wasm_f32x4_mul(l, r);
  return wasm_f32x4_extract_lane(l, 0) + wasm_f32x4_extract_lane(l, 1) +
         wasm_f32x4_extract_lane(l, 2) + wasm_f32x4_extract_lane(l, 3);
}

// 看了glm版本,理顺了最后面的部分
v128_t tmp_f32x4_const = wasm_f32x4_const(1, -1, -1, 1);
v128_t tmp_f32x4_make = wasm_f32x4_make(1, -1, -1, 1);
float mat4_determinant_simd3(float const te[16]) {
  // 7ms
  //   return te[0];
  v128_t n_1 = wasm_v128_load(te);
  v128_t n_2 = wasm_v128_load(te + 4);
  v128_t n_3 = wasm_v128_load(te + 8);
  v128_t n_4 = wasm_v128_load(te + 12);

  // 7.8ms
  //   return wasm_f32x4_extract_lane(n_1, 0) + wasm_f32x4_extract_lane(n_3, 0)
  //   +
  //          wasm_f32x4_extract_lane(n_2, 0) + wasm_f32x4_extract_lane(n_4, 0);

  v128_t s1_sl_l = wasm_i32x4_shuffle(n_2, n_4, 4, 4, 0, 0);
  v128_t s1_sl_r = wasm_i32x4_shuffle(n_1, n_3, 5, 1, 1, 5);
  v128_t s1_sr_l = wasm_i32x4_shuffle(n_1, n_3, 4, 0, 0, 4);
  v128_t s1_sr_r = wasm_i32x4_shuffle(n_2, n_4, 5, 5, 1, 1);

  v128_t s1 = wasm_f32x4_sub(wasm_f32x4_mul(s1_sl_l, s1_sl_r),
                             wasm_f32x4_mul(s1_sr_l, s1_sr_r));

  // 8.3ms
  //   return wasm_f32x4_extract_lane(s1, 0);

  v128_t s2_sl_l = wasm_i32x4_shuffle(n_2, n_3, 4, 0, 0, 0);
  v128_t s2_sl_r = wasm_i32x4_shuffle(n_1, n_4, 5, 1, 0, 0);
  v128_t s2_sr_l = wasm_i32x4_shuffle(n_1, n_4, 4, 0, 0, 0);
  v128_t s2_sr_r = wasm_i32x4_shuffle(n_2, n_3, 5, 1, 0, 0);

  v128_t s2 = wasm_f32x4_sub(wasm_f32x4_mul(s2_sl_l, s2_sl_r),
                             wasm_f32x4_mul(s2_sr_l, s2_sr_r));

  // 10.3ms
  //   return wasm_f32x4_extract_lane(s2, 0) + wasm_f32x4_extract_lane(s1, 0);

  v128_t m1 = wasm_f32x4_mul(s1, wasm_i32x4_shuffle(n_2, n_4, 2, 2, 6, 6));
  v128_t m2 = wasm_f32x4_mul(s1, wasm_i32x4_shuffle(n_1, n_3, 2, 2, 6, 6));
  // TODO: 封装成宏
  v128_t m3_r = wasm_i32x4_shuffle(
      wasm_i32x4_shuffle(wasm_i32x4_shuffle(n_4, n_2, 2, 6, 0, 0), n_3, 0, 1, 6,
                         0),
      n_1, 0, 1, 2, 6);
  //   v128_t m3_r = wasm_f32x4_make(te[14], te[6], te[2], te[10]);
  v128_t m3 = wasm_f32x4_mul(wasm_i32x4_shuffle(s2, s2, 0, 0, 1, 1), m3_r);

  // 12ms
  //   return wasm_f32x4_extract_lane(m1, 0) + wasm_f32x4_extract_lane(m3, 0) +
  //          wasm_f32x4_extract_lane(m2, 0);

  // 都慢,需要好好测试下wasm_i32x4_shuffle的性能了...,感觉可能还不如make的方式
  //   wasm_v128_store(tmpF32, m1);
  //   wasm_v128_store(tmpF32 + 4, m2);
  //   wasm_v128_store(tmpF32 + 8, m3);

  //   v128_t m8_r = wasm_f32x4_make(
  //       tmpF32[0] + tmpF32[1] + tmpF32[2], tmpF32[3] + tmpF32[8] - tmpF32[4],
  //       -tmpF32[10] + tmpF32[9] - tmpF32[5], -tmpF32[6] - tmpF32[7] -
  //       tmpF32[11]);
  //   v128_t n4_ = wasm_f32x4_make(te[3], te[7], te[11], te[15]);
  //   v128_t m8 = wasm_f32x4_mul(n4_, m8_r);

  //   // 怎么方便一个向量求和呢
  //   wasm_v128_store(tmpF32, m8);

  //   return tmpF32[0] + tmpF32[1] + tmpF32[2] + tmpF32[3];

  v128_t a1 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 0, 4, 1, 0), m3, 0,
                                 1, 2, 5);
  v128_t a2 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 0, 5, 2, 7), m3, 7,
                                 1, 2, 3);
  v128_t a3 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 3, 0, 0, 6), m3, 0,
                                 4, 6, 3);
  v128_t m4_r = wasm_f32x4_sub(
      wasm_f32x4_add(wasm_f32x4_mul(a1, tmp_f32x4_const), a2), a3);
  v128_t m4_l = wasm_i32x4_shuffle(
      wasm_i32x4_shuffle(wasm_i32x4_shuffle(n_1, n_2, 3, 7, 0, 0), n_3, 0, 1, 7,
                         0),
      n_4, 0, 1, 2, 7);
  v128_t m4 = wasm_f32x4_mul(m4_r, m4_l);
  v128_t m4_1 = wasm_i32x4_shuffle(m4, m4, 0, 0, 0, 0);
  v128_t m4_2 = wasm_i32x4_shuffle(m4, m4, 1, 1, 1, 1);
  v128_t m4_3 = wasm_i32x4_shuffle(m4, m4, 2, 2, 2, 2);
  v128_t m4_4 = wasm_i32x4_shuffle(m4, m4, 3, 3, 3, 3);
  v128_t a4 =
      wasm_f32x4_add(wasm_f32x4_add(wasm_f32x4_add(m4_1, m4_2), m4_3), m4_4);

  return wasm_f32x4_extract_lane(a4, 0);
}

float mat4_determinant_simd4(float const te[16]) {
  float n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  float n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  float n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  float n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  v128_t s1_sl_l = wasm_f32x4_make(n14, n14, n12, n12);
  v128_t s1_sl_r = wasm_f32x4_make(n23, n21, n21, n23);
  v128_t s1_sr_l = wasm_f32x4_make(n13, n11, n11, n13);
  v128_t s1_sr_r = wasm_f32x4_make(n24, n24, n22, n22);

  v128_t s1 = wasm_f32x4_sub(wasm_f32x4_mul(s1_sl_l, s1_sl_r),
                             wasm_f32x4_mul(s1_sr_l, s1_sr_r));

  // 7.8ms
  //   return wasm_f32x4_extract_lane(s1, 0);

  v128_t s2_sl_l = wasm_f32x4_replace_lane(wasm_f32x4_splat(n13), 1, n12);
  v128_t s2_sl_r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n21), 1, n24);
  v128_t s2_sr_l = wasm_f32x4_replace_lane(wasm_f32x4_splat(n11), 1, n14);
  v128_t s2_sr_r = wasm_f32x4_replace_lane(wasm_f32x4_splat(n23), 1, n22);

  v128_t s2 = wasm_f32x4_sub(wasm_f32x4_mul(s2_sl_l, s2_sl_r),
                             wasm_f32x4_mul(s2_sr_l, s2_sr_r));

  // 8.1ms
  //   return wasm_f32x4_extract_lane(s2, 0) + wasm_f32x4_extract_lane(s1, 0);

  v128_t m1 = wasm_f32x4_mul(s1, wasm_f32x4_make(n32, n32, n34, n34));
  v128_t m2 = wasm_f32x4_mul(s1, wasm_f32x4_make(n31, n33, n33, n31));
  v128_t m3 = wasm_f32x4_mul(wasm_i32x4_shuffle(s2, s2, 0, 0, 1, 1),
                             wasm_f32x4_make(n34, n32, n31, n33));

  // 10.3ms
  // return wasm_f32x4_extract_lane(m1, 0) + wasm_f32x4_extract_lane(m3, 0) +
  //        wasm_f32x4_extract_lane(m2, 0);

  //   v128_t a1 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 0, 4, 1, 0),
  //   m3, 0,
  //                                  1, 2, 5);
  //   v128_t a2 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 0, 5, 2, 7),
  //   m3, 7,
  //                                  1, 2, 3);
  //   v128_t a3 = wasm_i32x4_shuffle(wasm_i32x4_shuffle(m1, m2, 3, 0, 0, 6),
  //   m3, 0,
  //                                  4, 6, 3);

  //   // 15ms 这三个耗时巨大...5ms
  // return wasm_f32x4_extract_lane(a1, 0) + wasm_f32x4_extract_lane(a3, 0) +
  //        wasm_f32x4_extract_lane(a2, 0);

  v128_t a1 = wasm_f32x4_make(
      wasm_f32x4_extract_lane(m1, 0), wasm_f32x4_extract_lane(m2, 0),
      wasm_f32x4_extract_lane(m1, 1), wasm_f32x4_extract_lane(m3, 1));
  v128_t a2 = wasm_f32x4_make(
      wasm_f32x4_extract_lane(m3, 3), wasm_f32x4_extract_lane(m2, 1),
      wasm_f32x4_extract_lane(m1, 2), wasm_f32x4_extract_lane(m2, 3));
  v128_t a3 = wasm_f32x4_make(
      wasm_f32x4_extract_lane(m1, 3), wasm_f32x4_extract_lane(m3, 0),
      wasm_f32x4_extract_lane(m3, 2), wasm_f32x4_extract_lane(m2, 2));

  // 10.7ms
  // return wasm_f32x4_extract_lane(a1, 0) + wasm_f32x4_extract_lane(a3, 0) +
  //        wasm_f32x4_extract_lane(a2, 0);

  v128_t m4_r = wasm_f32x4_sub(
      wasm_f32x4_add(wasm_f32x4_mul(a1, tmp_f32x4_const), a2), a3);

  //   v128_t m4_r =
  //       wasm_f32x4_add(wasm_f32x4_add(wasm_f32x4_mul(a1, tmp_f32x4_const),
  //       a2),
  //                      wasm_f32x4_mul(a3, wasm_f32x4_splat(-1)));

  //   v128_t m4_r = wasm_f32x4_mul(a1, a2);

  // 20ms 这耗时也太诡异了
  //   return wasm_f32x4_extract_lane(m4_r, 0) + wasm_f32x4_extract_lane(m4_r,
  //   1) +
  //          wasm_f32x4_extract_lane(m4_r, 2) + wasm_f32x4_extract_lane(m4_r,
  //          3);

  //   return wasm_f32x4_extract_lane(m4_r, 0);

  v128_t m4 = wasm_f32x4_mul(m4_r, wasm_f32x4_make(n41, n42, n43, n44));

  // 23ms
  //   return wasm_f32x4_extract_lane(m4, 0) + wasm_f32x4_extract_lane(m4, 1) +
  //          wasm_f32x4_extract_lane(m4, 2) + wasm_f32x4_extract_lane(m4, 3);

  // 12ms
  //   return wasm_f32x4_extract_lane(m4, 0);

  //   v128_t m4_1 = wasm_f32x4_splat(wasm_f32x4_extract_lane(m4, 0));
  //   v128_t m4_2 = wasm_f32x4_splat(wasm_f32x4_extract_lane(m4, 1));
  //   v128_t m4_3 = wasm_f32x4_splat(wasm_f32x4_extract_lane(m4, 2));
  //   v128_t m4_4 = wasm_f32x4_splat(wasm_f32x4_extract_lane(m4, 3));
  //   v128_t a4 =
  //       wasm_f32x4_add(wasm_f32x4_add(wasm_f32x4_add(m4_1, m4_2), m4_3),
  //       m4_4);

  //   return wasm_f32x4_extract_lane(a4, 3);

  wasm_v128_store(tmpF32, m4);

  // 22.6
  return tmpF32[0] + tmpF32[1] + tmpF32[2] + tmpF32[3];
  // 20ms
  return tmpF32[0] + tmpF32[1];

  // v128_t m4_1 = wasm_i32x4_shuffle(m4, m4, 0, 0, 0, 0);
  // v128_t m4_2 = wasm_i32x4_shuffle(m4, m4, 1, 1, 1, 1);
  // v128_t m4_3 = wasm_i32x4_shuffle(m4, m4, 2, 2, 2, 2);
  // v128_t m4_4 = wasm_i32x4_shuffle(m4, m4, 3, 3, 3, 3);
  // v128_t a4 =
  //     wasm_f32x4_add(wasm_f32x4_add(wasm_f32x4_add(m4_1, m4_2), m4_3),
  //     m4_4);

  // return wasm_f32x4_extract_lane(a4, 0);
}

v128_t const tmp_f32x4_const1 = wasm_f32x4_const(1, 1, -1, 1);
float mat4_determinant_simd5(float const te[16]) {
  // clang-format off
  float n11 = te[0], n21 = te[1], n31 = te[2], n41 = te[3], n12 = te[4],
        n22 = te[5], n32 = te[6], n42 = te[7], n13 = te[8], n23 = te[9],
        n33 = te[10], n43 = te[11], n14 = te[12], n24 = te[13], n34 = te[14],
        n44 = te[15],
        //      l1     r1    l2    r2     m1     l1    r3    l3    r2     m2    l2     r3    l3    l1     m3
        t11 =   (n34 * n23 - n33 * n24) * n42 - (n34 * n22 - n32 * n24) * n43 + (n33 * n22 - n32 * n23) * n44,
        t12 =   (n14 * n33 - n13 * n34) * n42 - (n14 * n32 - n12 * n34) * n43 + (n13 * n32 - n12 * n33) * n44,
        t13 = -((n14 * n23 - n13 * n24) * n42 - (n14 * n22 - n12 * n24) * n43 + (n13 * n22 - n12 * n23) * n44),
        t14 =   (n14 * n23 - n13 * n24) * n32 - (n14 * n22 - n12 * n24) * n33 + (n13 * n22 - n12 * n23) * n34;

  float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
  // clang-format on

  v128_t n_1 = wasm_v128_load(te);
  //   v128_t l1, l2, l3, r1, r2, r3, m1, m2, m3, s1, s2, s3, t1_;
  v128_t l1, l2, l3, r1, r2, r3, m, s1, s2, s3, t1_;

  l1 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n14), 0, n34);
  l2 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n13), 0, n33);
  l3 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n12), 0, n32);
  r1 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n23), 1, n33);
  r2 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n24), 1, n34);
  r3 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n22), 1, n32);
  //   m1 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n42), 3, n32);
  //   m2 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n43), 3, n33);
  //   m3 = wasm_f32x4_replace_lane(wasm_f32x4_splat(n44), 3, n34);

  m = wasm_f32x4_replace_lane(wasm_f32x4_splat(n42), 3, n32);
  s1 = wasm_f32x4_mul(l1, r1);
  s1 = wasm_f32x4_sub(s1, wasm_f32x4_mul(l2, r2));
  s1 = wasm_f32x4_mul(s1, m);

  m = wasm_f32x4_replace_lane(wasm_f32x4_splat(n43), 3, n33);
  s2 = wasm_f32x4_mul(l1, r3);
  s2 = wasm_f32x4_sub(s2, wasm_f32x4_mul(l3, r2));
  s2 = wasm_f32x4_mul(s2, m);

  m = wasm_f32x4_replace_lane(wasm_f32x4_splat(n44), 3, n34);
  s3 = wasm_f32x4_mul(l2, r3);
  s3 = wasm_f32x4_sub(s3, wasm_f32x4_mul(l3, r1));
  s3 = wasm_f32x4_mul(s3, m);

  t1_ = wasm_f32x4_mul(wasm_f32x4_add(wasm_f32x4_sub(s1, s2), s3),
                       tmp_f32x4_const1);

  // dot from glm
  v128_t mul0 = wasm_f32x4_mul(n_1, t1_);

  // 貌似没明显性能差距
  // return wasm_f32x4_extract_lane(mul0, 0) + wasm_f32x4_extract_lane(mul0, 1)
  // +
  //        wasm_f32x4_extract_lane(mul0, 2) + wasm_f32x4_extract_lane(mul0, 3);

  v128_t swp0 = wasm_i32x4_shuffle(mul0, mul0, 3, 2, 1, 0);
  v128_t add0 = wasm_f32x4_add(mul0, swp0);  // 0, 1, 2, 3
  v128_t swp1 = wasm_i32x4_shuffle(add0, add0, 1, 0, 3, 2);
  v128_t add1 = wasm_f32x4_add(add0, swp1);  // 0, 1, 2, 3

  return wasm_f32x4_extract_lane(add1, 0);
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

  v128_t *m1V128Ptr = mat4_v128_1;
  v128_t *m2V128Ptr = mat4_v128_2;
  v128_t *tmp3Ptr;

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
  if (type == 4)
    for (i = 0; i < loop; i++) {
      tmp3Ptr = m1V128Ptr;
      m1V128Ptr = m2V128Ptr;
      m2V128Ptr = tmp3Ptr;
      mat4_multiply_simd_glm(m1V128Ptr, m2V128Ptr, mat4_v128_3);
    }
  if (type == 5)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      // 这为什么不耗时; 现在耗时正常了, 基本吻合loop in js的差距
      m1Ptr[0] = mat4_determinant(m1);
    }
  if (type == 6)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      m1Ptr[0] = mat4_determinant_simd(m1Ptr);
    }
  if (type == 7)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      m1Ptr[0] = mat4_determinant_simd2(m1Ptr);
    }
  if (type == 8)
    for (i = 0; i < loop; i++) {
      tmp1Ptr = m1Ptr;
      m1Ptr = m2Ptr;
      m2Ptr = tmp1Ptr;
      m1Ptr[0] = mat4_determinant_simd5(m1Ptr);
    }
}

#endif
}
