
#include <iostream>

const int a = 100;

// 常量表达式 constexpr
constexpr int expr() { return a; }

const int b = expr();

struct StructA {
  const int a;
  int b = 1;
  StructA(int i) : a(i) {} // 需要list-initialization初始化常量
};

const char * a0; //指向const对象的指针或者说指向常量的指针。
char const * a1; //同上
char * const a2 = nullptr; //指向类型对象的const指针。或者说常指针、const指针。
const char * const a3 = nullptr; //指向const对象的const指针。

int main(int argc, char *argv[]) {
  const StructA structa{a};

  std::cout << a << std::endl;
}

// 4）可以节省空间，避免不必要的内存分配
// 是否也意味着cache不友好? 而立即数对cache更友好

