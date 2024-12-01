#include <commons.h>
#include <chrono>
#include <iostream>

void measureExecutionTime(const std::function<void()>& func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Execution Time: " << duration.count() << " ms\n";
}