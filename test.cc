#include <vector>
#include <thread>
#include <iostream>
#include <cassert>
#include <atomic>
#include <chrono>
#include "tm-list.h"

using namespace std;
using namespace std::chrono;

const int kNumOps = 1000000;
const int kNumThreads = 2;

atomic_uint_fast64_t elapsed(0);

void Add(struct list_head *list) {
  auto begin = high_resolution_clock::now();
  for (int i = 0; i < kNumOps; ++i) {
    struct list_head *node = new struct list_head;
    list_add_tail(node, list);
  }
  auto d = high_resolution_clock::now() - begin;
  elapsed += duration_cast<microseconds>(d).count();
}

void Delete(struct list_head *list) {
  auto begin = high_resolution_clock::now();
  for (int i = 0; i < kNumOps; ) {
    struct list_head *entry = list_del_head(list);
    if (entry) {
      delete entry;
      ++i;
    }
  }
  auto d = high_resolution_clock::now() - begin;
  elapsed += duration_cast<microseconds>(d).count();
}

int main() {
  struct list_head tm_list = LIST_HEAD_INIT(tm_list);
  vector<thread> threads;

  assert(kNumThreads % 2 == 0);
  for (int i = 0; i < kNumThreads; ++i) {
    threads.push_back(thread(i % 2 ? Delete : Add, &tm_list));
  }

  for (auto &t : threads) {
    t.join();
  }

  if (list_empty(&tm_list)) {
    cout << (double)elapsed / (kNumThreads * kNumOps) << endl;
  } else {
    cout << "Wrong state!" << endl;
  }
}

