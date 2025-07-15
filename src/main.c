#include "hash_table.h"
#include <stdio.h>

int main() {
  HashTable *ht = ht_new();
  int start = 1;
  int end = 13;
  char key[9], value[11];

  for (int i = start; i <= end; i++) {
    sprintf(key, "key-%d", i);
    sprintf(value, "value-%d", i);
    ht_insert(ht, key, value);
  }

  for (int i = start; i <= end; i++) {
    sprintf(key, "key-%d", i);
    sprintf(value, "value-%d", i);
    printf("[Result] %s : %s\n", key, ht_search(ht, key));
  }

  printf("[INFO] base_size: %d, size: %d, count: %d\n", ht->base_size, ht->size, ht->count);
  ht_delete(ht);
  return 0;
}
