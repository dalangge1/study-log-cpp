#include <iostream>
#include <memory>
#include <map>
#include <string>
struct A {
  float f;
  A(float f) : f(f) {}
};
struct B {
  std::shared_ptr<A> a = nullptr;
};

enum DT { Base, T0, T1 };

struct C {
  DT t = DT::Base;
  std::map<std::string, std::unique_ptr<char>> map;
  std::unique_ptr<B> b;
  C() : b(std::make_unique<B>()) {}
  C(DT t) : b(std::make_unique<B>()), t(t) {}
  C(C &s) : b(std::make_unique<B>(*s.b)) {}
  C *clone();
  virtual DT getType() { return DT::Base; }
};

struct D0 : C {
  float f;
  int i;
  D0(float f) : i(f), f(f), C(DT::T0) {}
  DT getType() override { return DT::T0; }
};
struct D1 : C {
  A a;
  D1(float f) : a(f), C(DT::T1) {}
  DT getType() override { return DT::T1; }
};

C *C::clone() {
  switch (getType()) {
  case DT::Base:
    return new C(*this);
  case DT::T0:
    return new D0(*reinterpret_cast<D0 *>(this));
  case DT::T1:
    return new D1(*reinterpret_cast<D1 *>(this));
  }
}

int main(void) {
  C *c = new C();
  c->b->a = std::make_shared<A>(1);
  C *cd0_clone;
  C *cd1_clone;
  // C *cd_clone;
  for (int i = 0; i < 3; i++) {
    c->b->a->f = i;
    C *c0 = new C{*c};
    D0 *d0 = new D0{(float)i};
    D1 *d1 = new D1{(float)i};
    C *cd = d0;
    cd0_clone = cd->clone();
    cd = d1;
    cd1_clone = cd->clone();
    // cd_clone = new C(d1);
    std::cout << (c->b.get() != c0->b.get()) << std::endl;
    std::cout << (c0->b->a == c->b->a) << std::endl;
    std::cout << (c0->b->a->f == c->b->a->f) << std::endl;
    delete c0;
    // 不能delete多次
    // delete c0;
    // std::cout << (c->b->a->f) << std::endl;
    std::cout << (reinterpret_cast<D0 *>(cd0_clone)->f == d0->f) << std::endl;
    std::cout << (reinterpret_cast<D0 *>(cd0_clone)->b.get() != d0->b.get())
              << std::endl;
    std::cout << (reinterpret_cast<D1 *>(cd1_clone)->a.f == d1->a.f)
              << std::endl;
    std::cout << (reinterpret_cast<D1 *>(cd1_clone)->b.get() != d1->b.get())
              << std::endl;
    delete cd0_clone;
    delete cd1_clone;
    std::cout << "=======" << std::endl;
  }
}