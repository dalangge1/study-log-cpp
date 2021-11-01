#include <wasm_simd128.h>

extern "C"
{
  void mat4_multiply(double *a, double *b, double *out)
  {

    float b11 = b[0],
          b12 = b[4],
          b13 = b[8],
          b14 = b[12];
    float b21 = b[1],
          b22 = b[5],
          b23 = b[9],
          b24 = b[13];
    float b31 = b[2],
          b32 = b[6],
          b33 = b[10],
          b34 = b[14];
    float b41 = b[3],
          b42 = b[7],
          b43 = b[11],
          b44 = b[15];

    float a1 = a[0],
          a2 = a[4],
          a3 = a[8],
          a4 = a[12];

    out[0] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
    out[4] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
    out[8] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
    out[12] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

    a1 = a[1],
    a2 = a[5],
    a3 = a[9],
    a4 = a[13];

    out[1] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
    out[5] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
    out[9] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
    out[13] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

    a1 = a[2],
    a2 = a[6],
    a3 = a[10],
    a4 = a[14];

    out[2] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
    out[6] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
    out[10] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
    out[14] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;

    a1 = a[3],
    a2 = a[7],
    a3 = a[11],
    a4 = a[15];

    out[3] = a1 * b11 + a2 * b21 + a3 * b31 + a4 * b41;
    out[7] = a1 * b12 + a2 * b22 + a3 * b32 + a4 * b42;
    out[11] = a1 * b13 + a2 * b23 + a3 * b33 + a4 * b43;
    out[15] = a1 * b14 + a2 * b24 + a3 * b34 + a4 * b44;
  }
}
