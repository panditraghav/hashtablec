#define HT_IMPLEMENTATION
#include "hash_table.h"
#include <stdio.h>

int main() {
  HashTable *ht = ht_new();
  int start = 0;
  int end = 1024;
  char key[15], value[11];

  for (int i = start; i <= end; i++) {
    sprintf(key, "%c=key-%d", i % 95 + 32, i);
    sprintf(value, "value-%d", i);
    printf("[INSERT] key: %s, value: %s\n", key, value);
    ht_insert(ht, key, value);
  }

  for (int i = start; i <= end; i++) {
    sprintf(key, "%c=key-%d", i % 95 + 32, i);
    sprintf(value, "value-%d", i);
    printf("[Result] %d %s : %s\n", i, key, ht_search(ht, key));
  }

  printf("[INFO] base_size: %d, size: %d, item_count: %d, list_count: %d\n", ht->base_size, ht->size,
         ht->item_count, ht->list_count);
  ht_delete(ht);
  return 0;
}
