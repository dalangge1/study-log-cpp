#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
struct ABase {
  float f;
  explicit ABase(float f) : f(f) {}
  virtual void a() {}
  virtual ~ABase() = default;
};
struct B {
  float f;
  B(float f) : f(f) {}
};
struct A : ABase {
  std::unique_ptr<B> b;
  std::shared_ptr<B> bs;
  explicit A(float f, float fb)
      : ABase(f), b(std::make_unique<B>(fb)), bs(std::make_shared<B>(fb)) {}
  explicit A(A &a)
      : ABase(a.f), b(std::make_unique<B>(*a.b)), bs(new B(*a.bs.get())) {}
};
int main(void) {
  auto a = new A(1, 2);
  auto a0 = new A(*a);

  std::cout << (a->b == a0->b) << std::endl;
  std::cout << (a->b == a0->b) << std::endl;
  std::cout << (a->b->f == a0->b->f) << std::endl;
  std::cout << (a->bs.get() == a0->bs.get()) << std::endl;
  std::cout << (a->bs->f == a0->bs->f) << std::endl;
  a0->bs->f = 0;
  std::cout << (a->bs->f == a0->bs->f) << std::endl;

  std::map<std::string, B> map;
  map.emplace("a", B{1});
  map.emplace("b", B{3});
  std::cout << "map:" << std::endl;
  for (auto &[key, value] : map) {
    std::cout << '[' << key << "] = " << value.f << "; " << std::endl;
  }
  std::map<std::string, B> mapCloned(map);
  std::cout << "mapCloned:" << std::endl;
  for (auto &[key, value] : mapCloned) {
    std::cout << '[' << key << "] = " << value.f << "; " << std::endl;
  }
  // map是可以clone的
}