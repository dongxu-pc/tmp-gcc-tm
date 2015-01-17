
struct list_head {
  struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

static inline void list_add_tail(struct list_head *ne, struct list_head *head) {
  __transaction_atomic {
    head->prev->next = ne;
    ne->prev = head->prev;
    ne->next = head;
    head->prev = ne;
  }
}

static inline int list_empty(const struct list_head *head)
{
  return head->next == head;
}

static inline struct list_head *list_del_head(struct list_head *head) {
  __transaction_atomic {
    if (list_empty(head)) return nullptr;
    struct list_head* entry = head->next;

    entry->next->prev = head;
    head->next = entry->next;

    entry->next = nullptr;
    entry->prev = nullptr;
    return entry;
  }
}
