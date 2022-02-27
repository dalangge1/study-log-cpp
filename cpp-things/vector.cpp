#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

template <class T> void printArray(std::vector<T> &array) {
  for (auto item : array) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

struct A {
  float *ptr;
  float f;
};

void setArrayA(std::vector<A> &arrayA) {
  A a;
  a.f = 1;
  a.ptr = &a.f;
  arrayA.push_back(a);
}

struct vec4 {
  float x;
  float y;
  float z;
  float w;
};
inline std::ostream &operator<<(std::ostream &s, const vec4 &v) {
  s << '[' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << ']';
  return s;
}
int main(void) {
  std::vector<float> array{};
  array.push_back(1);
  array.push_back(2);
  auto arrayClone = array;
  printArray(arrayClone);

  // 这种方式是不行的
  std::vector<A> arrayA;

  setArrayA(arrayA);

  for (auto _ : arrayA) {
    *(_.ptr) = 0;
    printf("a.f: %f \n", _.f);
  }

  std::vector<A> arrayB{10};
  std::cout << arrayB.size() << std::endl;

  vec4 arrayVec4[3];
  vec4 *vec4Ptr = arrayVec4;
  vec4Ptr[0] = {1, 0, 0, 0};
  vec4Ptr[1] = {2, 0, 0, 0};
  vec4Ptr[2] = {3, 0, 0, 0};
  std::cout << arrayVec4[0] << std::endl;
  std::cout << arrayVec4[1] << std::endl;
  std::cout << arrayVec4[2] << std::endl;

  // std::fill((float *)vec4Ptr, (float *)(vec4Ptr + 3),
  //           -2);
  std::fill_n((float *)vec4Ptr, 3 * 4, -2);
  std::cout << arrayVec4[0] << std::endl;
  std::cout << arrayVec4[1] << std::endl;
  std::cout << arrayVec4[2] << std::endl;
}