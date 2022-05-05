#include <_types/_uint8_t.h>
#include <alloca.h>
#include <iostream>
#include <malloc/_malloc.h>
#include <sys/syslimits.h>
#include <xmmintrin.h>

bool isAligned(const void *ptr) {
  return !((unsigned long)ptr & 0x00000000000F);
}

float *getUnAlignedAddress() {
  float *ptr = (float *)malloc(sizeof(float) * 4 * 2);
  // 判断是否对齐, 判断16进制的最后一位是否为0
  if (isAligned(ptr))
    return ptr + 1;
  return ptr;
}

int main(void) {
  // float 32位 4个字节 * 4
  float *ptr1 = (float *)alloca(sizeof(float) * 4 * 2);
  float ptr2[4 * 2] = {0};
  float *ptr3 = (float *)malloc(sizeof(float) * 4 * 2);
  float *ptr4 = (float *)aligned_alloc(16, sizeof(float) * 4 * 2);
  float *ptr5 = getUnAlignedAddress();
  float *ptr6 = getUnAlignedAddress();

  std::cout << "address of ptr1: " << ptr1 << std::endl;
  std::cout << "address of ptr2: " << ptr2 << std::endl;
  std::cout << "address of ptr3: " << ptr3 << std::endl;
  std::cout << "address of ptr4: " << ptr4 << std::endl;
  std::cout << "address of ptr5: " << ptr5 << std::endl;
  std::cout << "address of ptr6: " << ptr6 << std::endl;

  std::cout << "address of ptr11: " << ptr1 + 1 << std::endl;
  std::cout << "address of ptr12: " << ptr1 + 2 << std::endl;
  std::cout << "address of ptr13: " << ptr1 + 3 << std::endl;
  std::cout << "address of ptr14: " << ptr1 + 4 << std::endl;
  // 一个float 4字节, 32位
  // 4字节 16位内存地址,如何判断能被16字节整除?
  // 最小单位是1个字节, 所以下一个float下标是+4, 再下一个是+8
  // 内存地址0x00000000, 下一个float 就是0x00000004
  // 16字节对齐就是说 0x00000000010, 最后一位为0

  ptr6[0] = ptr5[0] = 0;
  ptr6[1] = ptr5[1] = 1;
  ptr6[2] = ptr5[2] = 2;
  ptr6[3] = ptr5[3] = 3;

#if 1
  // 使用不要求字节对齐的则不会
  __m128 v0 = _mm_loadu_ps(ptr5);
  __m128 v1 = _mm_loadu_ps(ptr6);
  _mm_storeu_ps(ptr5, _mm_add_ps(v0, v1));
#else
  // 非对齐会硬件报错
  __m128 v0 = _mm_load_ps(ptr5);
  __m128 v1 = _mm_load_ps(ptr6);
  _mm_store_ps(ptr5, _mm_add_ps(v0, v1));
#endif

  std::cout << ptr5[0] << ',' << ptr5[1] << ',' << ptr5[2] << ',' << ptr5[3]
            << std::endl;

#define SAR_ALIGNMENT(type, align) type __attribute__((aligned(align)))
#define SAR_ALIGNMENT_16(type) SAR_ALIGNMENT(type, 16)

// 如何让分配16字节对齐呢?, 两种,一种是栈分配, 一种是动态分配
#if 1
  SAR_ALIGNMENT_16(struct) Matrix { float m[16]; };
#else
  struct Matrix { float m[16]; };
#endif

  Matrix m0 = {};
  float t0 = 0;
  Matrix m1 = {};
  float t1 = 0;
  float t2 = 0;
  Matrix m2 = {};
  float t3 = 0;
  float t4 = 0;
  float t5 = 0;
  Matrix m3 = {};
  float t6 = 0;
  float t7 = 0;
  float t8 = 0;
  std::cout << "address of m0: " << &m0 << std::endl;
  std::cout << "address of m1: " << &m1 << std::endl;
  std::cout << "address of m2: " << &m2 << std::endl;
  std::cout << "address of m3: " << &m3 << std::endl;
  Matrix *m = nullptr;

  if (!isAligned(&m0))
    m = &m0;
  else if (!isAligned(&m1))
    m = &m1;
  else if (!isAligned(&m2))
    m = &m2;
  else if (!isAligned(&m3))
    m = &m3;

  std::cout << "address of m : " << m << std::endl;

  struct Transform {
    Matrix mat4;
    struct Vec3 {
      float x;
      float y;
      float z;
    } vec3;
    struct Vec2 {
      float x;
      float y;
    } vec2;
    // Matrix mat4;
  };

  Transform T0 = {};
  float t00 = 0;
  Transform T1 = {};
  float t10 = 0;
  float t20 = 0;
  Transform T2 = {};
  float t30 = 0;
  float t40 = 0;
  float t50 = 0;
  Transform T3 = {};
  float t60 = 0;
  float t70 = 0;
  float t80 = 0;

  std::cout << "address of T0.mat4: " << &T0.mat4 << std::endl;
  std::cout << "address of T1.mat4: " << &T1.mat4 << std::endl;
  std::cout << "address of T2.mat4: " << &T2.mat4 << std::endl;
  std::cout << "address of T3.mat4: " << &T3.mat4 << std::endl;
  m = nullptr;

  if (!isAligned(&T0.mat4))
    m = &T0.mat4;
  else if (!isAligned(&T1.mat4))
    m = &T1.mat4;
  else if (!isAligned(&T2.mat4))
    m = &T2.mat4;
  else if (!isAligned(&T3.mat4))
    m = &T3.mat4;

  std::cout << "address of m : " << m << std::endl;

  Matrix *mp0 = new Matrix();
  Matrix *mp1 = new Matrix();
  Matrix *mp2 = new Matrix();
  Matrix *mp3 = new Matrix();
  std::cout << "address of mp0: " << &(*mp0) << std::endl;
  std::cout << "address of mp1: " << &(*mp1) << std::endl;
  std::cout << "address of mp2: " << &(*mp2) << std::endl;
  std::cout << "address of mp3: " << &(*mp3) << std::endl;

  if (!isAligned(mp0))
    m = mp0;
  else if (!isAligned(mp1))
    m = mp1;
  else if (!isAligned(mp2))
    m = mp2;
  else if (!isAligned(mp3))
    m = mp3;

  std::cout << "address of m : " << m << std::endl;

  Matrix *m5 = (Matrix *)getUnAlignedAddress();
  std::cout << "address of m5: " << m5 << std::endl;
}