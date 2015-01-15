#include <vector>
#include <thread>
#include <iostream>
#include <cassert>
#include "tm-list.h"

using namespace std;

const int kNumOps = 1000000;
const int kNumThreads = 4;

void Add(struct list_head *list) {
  for (int i = 0; i < kNumOps; ++i) {
    struct list_head *node = new struct list_head;
    list_add_head(node, list);
  }
}

void Delete(struct list_head *list) {
  for (int i = 0; i < kNumOps; ) {
    struct list_head *entry = list_del_head(list);
    if (entry) {
      delete entry;
      ++i;
    }
  }
}

int main() {
  struct list_head tm_list = LIST_HEAD_INIT(tm_list);
  vector<thread> threads;

  assert(kNumThreads % 2 == 0);
  for (int i = 0; i < kNumThreads; ++i) {
    threads.push_back(thread(i % 2 ? Add : Delete, &tm_list));
  }

  for (auto &t : threads) {
    t.join();
  }

  if (list_empty(&tm_list)) {
    cout << "Success!" << endl;
  } else {
    cout << "Wrong state!" << endl;
  }
}

