#define HT_IMPLEMENTATION
#include "hash_table.h"
#include <stdio.h>

int main() {
  HashTable *ht = ht_new();
  int start = 0;
  int end = 36;
  char key[15], value[11];

  // int i = 1;
  // sprintf(key, "%c=key-%d", i % 95 + 33, i);
  // sprintf(value, "value-%d", i);
  // ht_insert(ht, key, value);
  //
  // i = 11;
  // sprintf(key, "%c=key-%d", i % 95 + 33, i);
  // sprintf(value, "value-%d", i);
  // ht_insert(ht, key, value);
  //
  // i = 11;
  // sprintf(key, "%c=key-%d", i % 95 + 33, i);
  // ht_delete_item(ht, key);
  //
  // i = 1;
  // sprintf(key, "%c=key-%d", i % 95 + 33, i);
  // printf("[SEARCH] %s : %s\n", key, ht_search(ht, key));
  //
  // ht_delete_item(ht, key);
  //
  // printf("[INFO AFTER DELETE] base_size: %d, size: %d, item_count: %d, "
  //        "list_count: %d\n",
  //        ht->base_size, ht->size, ht->item_count, ht->list_count);
  //
  // return 0;
  //
  for (int i = start; i <= end; i++) {
    sprintf(key, "%c=key-%d", i % 95 + 33, i);
    sprintf(value, "value-%d", i);
    printf("[INSERT] key: %s, value: %s\n", key, value);
    ht_insert(ht, key, value);
  }

  printf("[INFO BEFORE DELETE] base_size: %d, size: %d, item_count: %d, "
         "list_count: %d\n",
         ht->base_size, ht->size, ht->item_count, ht->list_count);

  for (int i = start; i <= 21; i++) {
    sprintf(key, "%c=key-%d", i % 95 + 33, i);
    // sprintf(value, "value-%d", i);
    ht_delete_item(ht, key);
    // printf("[Result] %d %s : %s\n", i, key, ht_search(ht, key));
  }

  printf("[INFO AFTER DELETE] base_size: %d, size: %d, item_count: %d, "
         "list_count: %d\n",
         ht->base_size, ht->size, ht->item_count, ht->list_count);

  ht_delete(ht);
  return 0;
}
