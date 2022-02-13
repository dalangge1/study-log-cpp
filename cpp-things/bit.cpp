
#include <_types/_uint32_t.h>
#include <iostream>

struct CommandHeader {
  uint32_t size : 21;
  uint32_t command : 11;
};

int main(void) { std::cout << sizeof(CommandHeader) << "bytes" << std::endl; }