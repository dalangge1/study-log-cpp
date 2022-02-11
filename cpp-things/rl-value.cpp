void fn2(int &&t) {}

int main(void) {
  int x = 6;  // x是左值，6是右值
  int &y = x; // 左值引用，y引用x

  // int &z1 = x * 6;       // 错误，x*6是一个右值
  const int &z2 = x * 6; // 正确，可以将一个const引用绑定到一个右值

  int &&z3 = x * 6; // 正确，右值引用
  // int &&z4 = x;     // 错误，x是一个左值

  auto fn = []() {}; // lambda 是一个右值
  auto &&fn1 = fn;

  fn2(2);

  auto &&z5 = z3 * 1;
}