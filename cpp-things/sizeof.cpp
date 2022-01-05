
#include <iostream>
using namespace std;

// https://light-city.club/sc/basic_content/sizeof/

/**
 * @file blackclass.cpp
 * @brief 空类的大小为1字节
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
class A {};

/**
 * @file static.cpp
 * @brief 静态数据成员
 * 静态数据成员被编译器放在程序的一个global data
 * members中，它是类的一个数据成员，但不影响类的大小。不管这个类产生了多少个实例，还是派生了多少新的类，静态数据成员只有一个实例。静态数据成员，一旦被声明，就已经存在。
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
class B {
public:
  char b;
  virtual void fun(){};
  static int c;
  static int d;
  static int f;
};

/**
 * @file morevir.cpp
 * @brief 对于包含虚函数的类，不管有多少个虚函数，只有一个虚指针,vptr的大小。
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
class C {
  virtual void fun();
  virtual void fun1();
  virtual void fun2();
  virtual void fun3();
};

int main() {
  cout << sizeof(A) << endl;

  /**
   * @brief 16  字节对齐、静态变量不影响类的大小、vptr指针=8
   */
  cout << sizeof(B) << endl;

  cout << sizeof(C) << endl; // 8
  return 0;
}