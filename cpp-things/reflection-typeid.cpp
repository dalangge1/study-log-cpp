#include <iostream>
#include <string>

class Object1 {
public:
  //用于在运行时获取对象的真实类型名称
  virtual std::string getClassName() const {
#ifdef _MSC_VER
    return typeid(*this).name() + 6;
#else
    const char *name = typeid(*this).name();

    while (*name >= '0' && *name <= '9')
      name++;

    return name;
#endif
  }
};

int main(void) {
  Object1 obj;
  Object1 *ptr = new Object1();
  std::cout << obj.getClassName() << std::endl;
  std::cout << ptr->getClassName() << std::endl;
}