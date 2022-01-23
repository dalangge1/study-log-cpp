#include <chrono>
#include <emmintrin.h>
#include <iostream>
#include <smmintrin.h>
#include <xmmintrin.h>

std::uint64_t rdtsc() {
  std::uint32_t lo, hi;

  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (std::uint64_t)hi << 32 | lo;
}

// clang-format off
#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)
// clang-format on

struct Matrix4 {
  union {
    float elements[16];
    float m[4][4];
    __m128 mVec[4];
  };
};

template <class Tp> inline void DoNotOptimize(Tp &value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

namespace SSE {
// for column major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A2 |
//                                              | A1  A3 |
// 2x2 column major Matrix multiply A*B
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2) {
  return _mm_add_ps(
      _mm_mul_ps(vec1, VecSwizzle(vec2, 0, 0, 3, 3)),
      _mm_mul_ps(VecSwizzle(vec1, 2, 3, 0, 1), VecSwizzle(vec2, 1, 1, 2, 2)));
}
// 2x2 column major Matrix adjugate multiply (A#)*B
inline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2) {
  return _mm_sub_ps(
      _mm_mul_ps(VecSwizzle(vec1, 3, 0, 3, 0), vec2),
      _mm_mul_ps(VecSwizzle(vec1, 2, 1, 2, 1), VecSwizzle(vec2, 1, 0, 3, 2)));
}
// 2x2 column major Matrix multiply adjugate A*(B#)
inline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2) {
  return _mm_sub_ps(
      _mm_mul_ps(vec1, VecSwizzle(vec2, 3, 3, 0, 0)),
      _mm_mul_ps(VecSwizzle(vec1, 2, 3, 0, 1), VecSwizzle(vec2, 1, 1, 2, 2)));
}

// Inverse function is the same no matter column major or row major
// this version treats it as column major
inline Matrix4 GetInverse(const Matrix4 &inM) {
  // use block matrix method
  // A is a matrix, then i(A) or iA means inverse of A, A# (or A_ in code) means
  // adjugate of A, |A| (or detA in code) is determinant, tr(A) is trace

  // sub matrices
  __m128 A = VecShuffle_0101(inM.mVec[0], inM.mVec[1]);
  __m128 C = VecShuffle_2323(inM.mVec[0], inM.mVec[1]);
  __m128 B = VecShuffle_0101(inM.mVec[2], inM.mVec[3]);
  __m128 D = VecShuffle_2323(inM.mVec[2], inM.mVec[3]);

#if 0
	__m128 detA = _mm_set1_ps(inM.m[0][0] * inM.m[1][1] - inM.m[0][1] * inM.m[1][0]);
	__m128 detC = _mm_set1_ps(inM.m[0][2] * inM.m[1][3] - inM.m[0][3] * inM.m[1][2]);
	__m128 detB = _mm_set1_ps(inM.m[2][0] * inM.m[3][1] - inM.m[2][1] * inM.m[3][0]);
	__m128 detD = _mm_set1_ps(inM.m[2][2] * inM.m[3][3] - inM.m[2][3] * inM.m[3][2]);
#else
  // determinant as (|A| |C| |B| |D|)
  __m128 detSub =
      _mm_sub_ps(_mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 0, 2, 0, 2),
                            VecShuffle(inM.mVec[1], inM.mVec[3], 1, 3, 1, 3)),
                 _mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 1, 3, 1, 3),
                            VecShuffle(inM.mVec[1], inM.mVec[3], 0, 2, 0, 2)));
  __m128 detA = VecSwizzle1(detSub, 0);
  __m128 detC = VecSwizzle1(detSub, 1);
  __m128 detB = VecSwizzle1(detSub, 2);
  __m128 detD = VecSwizzle1(detSub, 3);
#endif

  // let iM = 1/|M| * | X  Y |
  //                  | Z  W |

  // D#C
  __m128 D_C = Mat2AdjMul(D, C);
  // A#B
  __m128 A_B = Mat2AdjMul(A, B);
  // X# = |D|A - B(D#C)
  __m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
  // W# = |A|D - C(A#B)
  __m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));

  // |M| = |A|*|D| + ... (continue later)
  __m128 detM = _mm_mul_ps(detA, detD);

  // Y# = |B|C - D(A#B)#
  __m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
  // Z# = |C|B - A(D#C)#
  __m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));

  // |M| = |A|*|D| + |B|*|C| ... (continue later)
  detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

  // tr((A#B)(D#C))
  __m128 tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0, 2, 1, 3));
  tr = _mm_hadd_ps(tr, tr);
  tr = _mm_hadd_ps(tr, tr);
  // |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C))
  detM = _mm_sub_ps(detM, tr);

  const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
  // (1/|M|, -1/|M|, -1/|M|, 1/|M|)
  __m128 rDetM = _mm_div_ps(adjSignMask, detM);

  X_ = _mm_mul_ps(X_, rDetM);
  Y_ = _mm_mul_ps(Y_, rDetM);
  Z_ = _mm_mul_ps(Z_, rDetM);
  W_ = _mm_mul_ps(W_, rDetM);

  Matrix4 r;

  // apply adjugate and store, here we combine adjugate shuffle and store
  // shuffle
  r.mVec[0] = VecShuffle(X_, Z_, 3, 1, 3, 1);
  r.mVec[1] = VecShuffle(X_, Z_, 2, 0, 2, 0);
  r.mVec[2] = VecShuffle(Y_, W_, 3, 1, 3, 1);
  r.mVec[3] = VecShuffle(Y_, W_, 2, 0, 2, 0);

  return r;
}
} // namespace SSE

namespace Reference {
inline Matrix4 GetInverse(const Matrix4 &mat4) {
  Matrix4 out;

  float a00 = mat4.elements[0], a01 = mat4.elements[1], a02 = mat4.elements[2],
        a03 = mat4.elements[3];
  float a10 = mat4.elements[4], a11 = mat4.elements[5], a12 = mat4.elements[6],
        a13 = mat4.elements[7];
  float a20 = mat4.elements[8], a21 = mat4.elements[9], a22 = mat4.elements[10],
        a23 = mat4.elements[11];
  float a30 = mat4.elements[12], a31 = mat4.elements[13],
        a32 = mat4.elements[14], a33 = mat4.elements[15];

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
  float det =
      b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

  if (det == 0) {
    // out.Set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return out;
  }
  det = 1.0 / det;

  out.elements[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
  out.elements[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
  out.elements[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
  out.elements[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
  out.elements[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
  out.elements[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
  out.elements[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
  out.elements[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
  out.elements[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
  out.elements[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
  out.elements[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
  out.elements[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
  out.elements[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
  out.elements[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
  out.elements[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
  out.elements[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;
  return out;
}
} // namespace Reference

int main(void) {
  using namespace std;
  using namespace chrono;

  const int iteration = 10000;
  Matrix4 mat4A{7, 3, 6, 9, 2, 3, 2, 5, 1, 9, 5, 8, 3, 7, 2, 2};
  Matrix4 mat4B{6, 3, 5, 9, 1, 3, 1, 5, 1, 8, 5, 7, 3, 6, 2, 1};
  Matrix4 mat4C;

  auto start = high_resolution_clock::now();
  auto start_count = rdtsc();
  for (int i = 0; i < iteration; i++) {
    mat4C = SSE::GetInverse(i % 2 ? mat4A : mat4B);
    // Matrix4 out = SSE::GetInverse(i % 2 ? mat4A : mat4B);
    // // mat4.m[0][1]++;
    // DoNotOptimize(out);
    DoNotOptimize(mat4C);
  }
  auto SSE_time = high_resolution_clock::now() - start;
  auto SSE_count = rdtsc() - start_count;

  mat4A = {7, 3, 6, 9, 2, 3, 2, 5, 1, 9, 5, 8, 3, 7, 2, 2};
  mat4B = {6, 3, 5, 9, 1, 3, 1, 5, 1, 8, 5, 7, 3, 6, 2, 1};

  start = high_resolution_clock::now();
  start_count = rdtsc();
  for (int i = 0; i < iteration; i++) {
    mat4C = Reference::GetInverse(i % 2 ? mat4A : mat4B);
    // Matrix4 out = Reference::GetInverse(i % 2 ? mat4A : mat4B);
    // // mat4A.m[0][1]++;
    // DoNotOptimize(out);
    DoNotOptimize(mat4C);
  }
  auto Reference_time = high_resolution_clock::now() - start;
  auto Reference_count = rdtsc() - start_count;

  DoNotOptimize(mat4C);

  cout << "[T]SSE       :" << SSE_time.count() / milliseconds::period::den
       << '\n';
  cout << "[T]Reference :" << Reference_time.count() / milliseconds::period::den
       << '\n';
  cout << "[C]SSE       :" << SSE_count << '\n';
  cout << "[C]Reference :" << Reference_count << '\n';
}