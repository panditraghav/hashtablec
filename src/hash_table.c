#include "hash_table.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define HT_PRIME_1 29
#define HT_PRIME_2 37

static HashTableItem HT_DELETED_ITEM = {NULL, NULL};

static HashTableItem *ht_new_item(const char *key, const char *value) {
  HashTableItem *i = (HashTableItem *)malloc(sizeof(HashTableItem));
  i->key = strdup(key);
  i->value = strdup(value);
  return i;
}

HashTable *ht_new() {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(HashTableItem));
  return ht;
}

static void ht_delete_item(HashTableItem *item) {
  free(item->key);
  free(item->value);
  free(item);
}

void ht_delete_hash_table(HashTable *ht) {
  for (size_t i = 0; i < ht->size; i++) {
    HashTableItem *item = ht->items[i];
    if (item != NULL) {
      ht_delete_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static int ht_hash(const char *s, const int a, const int m) {
  long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
    hash += (long)pow(a, len_s - (i + 1)) * s[i];
    hash = hash % m;
  }

  return (int)hash;
}

static int ht_get_hash(const char *s, const int num_buckets,
                       const int attempts) {
  int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);

  return (hash_a + (hash_b + 1) * attempts) % num_buckets;
}

void ht_insert(HashTable *ht, const char *key, const char *value) {
  HashTableItem *item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  HashTableItem *current_item = ht->items[index];
  int i = 1;

  while (current_item != NULL) {
    if (current_item != &HT_DELETED_ITEM &&
        strcmp(current_item->key, key) == 0) {
      ht_delete_item(current_item);
      ht->items[index] = item;
      return;
    }
    index = ht_get_hash(item->key, ht->size, i);
    current_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}

char *ht_search(HashTable *ht, const char *key) {
  int index = ht_get_hash(key, ht->size, 0);
  HashTableItem *item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
      return ht->items[index]->value;
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}

void ht_delete(HashTable *ht, const char *key) {
  int index = ht_get_hash(key, ht->size, 0);
  HashTableItem *item = ht->items[index];
  int i = 1;

  while (item != NULL) {
    if (item == &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
      ht_delete_item(item);
      ht->items[index] = &HT_DELETED_ITEM;
      ht->count--;
      return;
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
}
