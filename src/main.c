#include "hash_table.h"

int main() {
  HashTable *ht = ht_new();
  ht_delete_hash_table(ht);
  return 0;
}
