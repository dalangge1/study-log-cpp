#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

template <typename T> void printArray(std::vector<T> &array);

// 目标是多线程排序
// 线程数目,任务拆分,都是内部函数实现

template <typename T>
void threadSort(std::vector<T> &array, int minThreadPayload = 32,
                int maxThread = std::thread::hardware_concurrency()) {
  // 系统可用线程数
  // 单个线程处理最低下线

  // 把数组拆分为batch
  auto length = array.size();
  int batchNum = std::ceilf((float)length / (float)minThreadPayload);
  int threadUsed = std::min(batchNum, maxThread);
  int batchJobSize = std::ceilf((float)length / (float)threadUsed);
  // printf("batchNum %d threadUsed:%d batchJobSize:%d \n", batchNum,
  // threadUsed,
  //        batchJobSize);

  auto sort = [&](int threadId) {
    // cout可能被分割...
    // std::cout << "threadId:" << threadId << std::endl;

    int startIndex = batchJobSize * threadId;
    int endIndex = std::min((int)length, batchJobSize * (threadId + 1));
    // printf("threadId %d s:%d e:%d \n", threadId, startIndex, endIndex);
    std::sort(array.begin() + startIndex, array.begin() + endIndex);
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < threadUsed; i++)
    threads.push_back(std::thread(sort, i));

  for (auto &thread : threads)
    thread.join();

  // TODO 归并
  std::sort(array.begin(), array.end());
}

inline float random(float min, float max) {
  return min + (max - min) * (std::rand() / (float)RAND_MAX);
}

template <typename T> void printArray(std::vector<T> &array) {
  auto size = array.size();
  std::cout << "array: ";
  for (int i = 0; i < size; i++) {
    std::cout << array[i] << ' ';
  }
  std::cout << std::endl;
}

int main(void) {
  std::vector<float> array{9, 8, 7, 6, 5, 4, 3, 2, 1};

  // std::sort(array.begin() + 0, array.begin() + 4);
  // printArray(array);

  // std::sort(array.begin() + 4, array.begin() + array.size());
  // printArray(array);

  threadSort(array, 2, 2);
  printArray(array);

  array.clear();
  std::vector<float> array1{};
  for (long long i = 0; i < 100000000; i++) {
    float n = random(0, 9);
    array.push_back(n);
    array1.push_back(n);
  }

  auto startTime = std::chrono::high_resolution_clock::now();
  std::sort(array1.begin(), array1.end());
  auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                      std::chrono::high_resolution_clock::now() - startTime)
                      .count();
  std::cout << "sort cost:" << duration << std::endl; // 9.11922

  startTime = std::chrono::high_resolution_clock::now();
  threadSort(array);
  duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                 std::chrono::high_resolution_clock::now() - startTime)
                 .count();
  std::cout << "threadSort cost:" << duration << std::endl; // 4.43894
}