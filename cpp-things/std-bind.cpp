#include <functional>
#include <iostream>

class Base {
public:
  void display_sum(int a1, int a2) { std::cout << a1 + a2 + m_data << '\n'; }

  int m_data = 30;
};

int main() {
  Base base;
  auto fn = std::bind(&Base::display_sum, &base, 100, std::placeholders::_1);
  fn(20);
}