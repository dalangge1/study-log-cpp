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
void threadSort(std::vector<T> &array, int minThreadPayload = 5000,
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

  if (threadUsed <= 1) {
    std::sort(array.begin(), array.end());
    return;
  }

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

  std::sort(array.begin(), array.end());
}

template <typename T>
void threadSortByMerge(std::vector<T> &array, int minThreadPayload = 32,
                       int maxThread = std::thread::hardware_concurrency()) {
  auto length = array.size();
  int batchNum = std::ceilf((float)length / (float)minThreadPayload);
  int threadUsed = std::min(batchNum, maxThread);
  int batchJobSize = std::ceilf((float)length / (float)threadUsed);
  if (threadUsed <= 1) {
    std::sort(array.begin(), array.end());
    return;
  }

  auto sort = [&](int threadId) {
    int startIndex = batchJobSize * threadId;
    int endIndex = std::min((int)length, batchJobSize * (threadId + 1));
    std::sort(array.begin() + startIndex, array.begin() + endIndex);
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < threadUsed; i++)
    threads.push_back(std::thread(sort, i));

  for (auto &thread : threads)
    thread.join();

  std::vector<std::vector<T>> chunks;
  for (int i = 0; i < threadUsed; i++) {
    int startIndex = batchJobSize * i;
    int endIndex = std::min((int)length, batchJobSize * (i + 1));
    chunks.push_back({array.begin() + startIndex, array.begin() + endIndex});
  }

  auto mergeSort = [&](std::vector<T> &input, std::vector<T> &chunk,
                       std::vector<T> &output, int inputLen) -> int {
    int inputIndex = 0;
    int chunkIndex = 0;
    int outputIndex = 0;
    int inputEndIndex = inputLen;
    int chunkEndIndex = chunk.size();

    // printf("inputEndIndex: %d chunkEndIndex: %d \n", inputEndIndex,
    // chunkEndIndex);

    while (!(inputIndex == inputEndIndex || chunkIndex == chunkEndIndex)) {
      // printf("0 ci: %d ii: %d oi: %d \n", chunkIndex, inputIndex,
      // outputIndex);
      if (chunk[chunkIndex] <= input[inputIndex]) {
        output[outputIndex++] = chunk[chunkIndex++];
      } else {
        output[outputIndex++] = input[inputIndex++];
      }
    }

    while (inputIndex != inputEndIndex) {
      // printf("1 ci: %d ii: %d oi: %d \n", chunkIndex, inputIndex,
      // outputIndex);
      output[outputIndex++] = input[inputIndex++];
    }
    while (chunkIndex != chunkEndIndex) {
      // printf("2 ci: %d ii: %d oi: %d \n", chunkIndex, inputIndex,
      // outputIndex);
      output[outputIndex++] = chunk[chunkIndex++];
    }

    return chunkEndIndex + inputEndIndex + 1;
  };

  if (threadUsed == 2) {
    mergeSort(chunks[0], chunks[1], array, chunks[0].size());
    return;
  }

  if (threadUsed > 2) {
    std::vector<T> tmp(array.size());
    std::vector<T> *input = &tmp;
    std::vector<T> *output = &array;
    std::vector<T> *tmpPtr;

    int inputLen = mergeSort(chunks[0], chunks[1], tmp, chunks[0].size());
    for (int i = 2; i < threadUsed; i++) {
      inputLen = mergeSort(*input, chunks[i], *output, inputLen);

      tmpPtr = input;
      input = output;
      output = tmpPtr;
    }

    // 奇数需要把tmp 复制到array
    if ((threadUsed - 2) & 1) {
      array = tmp;
    }
  }
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
  for (long long i = 0; i < 40000; i++) {
    float n = random(0, 9);
    array.push_back(n);
    array1.push_back(n);
  }

  auto startTime = std::chrono::high_resolution_clock::now();
  std::sort(array1.begin(), array1.end());
  auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                      std::chrono::high_resolution_clock::now() - startTime)
                      .count();
  std::cout << "sort cost:" << duration << std::endl;
  // -O2 100000000 cost 8.81494
  // -O2 50000 cost 0.00287379
  // -O2 40000 cost 0.00212787
  // -O2 30000 cost 0.00140916
  // -O2 20000 cost 0.00108422
  // -O2 10000 cost 0.000463468

  startTime = std::chrono::high_resolution_clock::now();
  threadSort(array);
  duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                 std::chrono::high_resolution_clock::now() - startTime)
                 .count();
  std::cout << "threadSort cost:" << duration << std::endl;
  // -O2 100000000 merge by std::sort cost 4.30087
  // -O2 50000 merge by std::sort cost 0.00219565
  // -O2 50000 merge by std::sort cost 0.00165555
  // -O2 30000 merge by std::sort cost 0.00112804
  // -O2 20000 merge by std::sort cost 0.001062
  // -O2 10000 merge by std::sort cost 0.000543246

  startTime = std::chrono::high_resolution_clock::now();
  threadSortByMerge(array);
  duration = std::chrono::duration_cast<std::chrono::duration<double>>(
                 std::chrono::high_resolution_clock::now() - startTime)
                 .count();
  std::cout << "threadSortByMerge cost:" << duration << std::endl;
  // -O2 100000000 merge by mergesort cost 2.02728
  // -O2 50000 merge by mergesort cost 0.00121518
  // -O2 40000 merge by mergesort cost 0.00107022
  // -O2 30000 merge by mergesort cost 0.000896142
  // -O2 20000 merge by mergesort cost 0.000807467
  // -O2 10000 merge by mergesort cost 0.000583132
}