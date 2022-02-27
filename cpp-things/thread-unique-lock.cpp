#include <cstdio>
#include <iostream> // std::cout
#include <list>
#include <mutex>  // std::mutex, std::unique_lock
#include <thread> // std::thread

std::mutex mtx; // mutex for critical section
// bool exiting = false;

void print_block(int n, char c) {
  // critical section (exclusive access to std::cout signaled by lifetime of
  // lck):
  std::unique_lock<std::mutex> lck(mtx);
  // while (!mtx.try_lock());

  // mtx.lock();
  for (int i = 0; i < n; ++i) {
    // std::cout << c;
    std::printf("%c", c);
  }
  std::printf("\n");
  // mtx.unlock();
  // std::cout << '\n';
  //   while (!exiting);
}

std::list<int> myList;
std::mutex myMutex;

void AddToList(int max, int interval) {
  std::lock_guard<std::mutex> guard(myMutex);
  std::cout << "AddToList";
  for (int i = 0; i < max; ++i) {
    if (i % interval == 0)
      myList.push_back(i);
  }
}
void PrintList() {
  std::lock_guard<std::mutex> guard(myMutex);
  for (auto &iter : myList)
    std::cout << iter << ",";
}

int main() {
  std::thread th1(print_block, 500, '*');
  std::thread th2(print_block, 500, '$');
  //   std::cout << "main";

  th1.join();
  th2.join();

  int max = 15;
  std::thread t1(AddToList, max, 1);
  std::cout << "main1";
  std::thread t2(AddToList, max, 2);
  std::cout << "main2";
  std::thread t3(PrintList);
  std::cout << "main2";
  t1.join();
  t2.join();
  t3.join();

  return 0;
}