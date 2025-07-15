#ifndef HT_HASHTABLE
#define HT_HASHTABLE

typedef struct {
  char *key;
  char *value;
} HashTableItem;

typedef struct {
  int size;
  int base_size;
  int count;
  HashTableItem **items;
} HashTable;

HashTable *ht_new();
void ht_insert(HashTable *ht, const char *key, const char *value);
char *ht_search(HashTable *ht, const char *key);
void ht_delete(HashTable *ht);
void ht_delete_item(HashTable *ht, const char *key);

// #define HT_IMPLEMENTATION
#ifdef HT_IMPLEMENTATION

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define HT_PRIME_1 501
#define HT_PRIME_2 103
#define HT_INITIAL_BASE_SIZE 50

/*--------------------------INTERNAL UTILITY FUNCTIONS------------------------*/
static HashTableItem HT_DELETED_ITEM = {NULL, NULL};

static int is_prime(const int x);
static int next_prime(int x);

static HashTableItem *ht_new_item(const char *key, const char *value);
static HashTable *ht_new_sized(const int base_size);

static void ht_resize(HashTable *ht, const int base_size);
static void ht_resize_up(HashTable *ht);
static void ht_resize_down(HashTable *ht);

static void ht_free_item(HashTableItem *item);

static int ht_hash(const char *s, const int a, const int m);
static int ht_get_hash(const char *s, const int num_buckets,
                       const int attempts);

/*------------------API FUNCTION IMPLEMENTATION------------------------------*/
HashTable *ht_new() { return ht_new_sized(HT_INITIAL_BASE_SIZE); }

void ht_delete(HashTable *ht) {
  for (size_t i = 0; i < ht->size; i++) {
    HashTableItem *item = ht->items[i];
    if (item != NULL) {
      ht_free_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

void ht_insert(HashTable *ht, const char *key, const char *value) {
  const int load = ht->count * 100 / ht->size;
  if (load > 70) {
    ht_resize_up(ht);
  }

  HashTableItem *item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  HashTableItem *current_item = ht->items[index];
  int i = 1;

  while (current_item != NULL) {
    if (current_item != &HT_DELETED_ITEM &&
        strcmp(current_item->key, key) == 0) {
      ht_free_item(current_item);
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

void ht_delete_item(HashTable *ht, const char *key) {
  const int load = ht->count * 100 / ht->size;
  if (load < 10) {
    ht_resize_down(ht);
  }
  int index = ht_get_hash(key, ht->size, 0);
  HashTableItem *item = ht->items[index];
  int i = 1;

  while (item != NULL) {
    if (item == &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
      ht_free_item(item);
      ht->items[index] = &HT_DELETED_ITEM;
      ht->count--;
      return;
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
}

/*-------------------STATIC FUNCTIONS DEFINATION-----------------------------*/

static HashTableItem *ht_new_item(const char *key, const char *value) {
  HashTableItem *i = (HashTableItem *)malloc(sizeof(HashTableItem));
  i->key = strdup(key);
  i->value = strdup(value);
  return i;
}

static HashTable *ht_new_sized(const int base_size) {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->base_size = base_size;

  ht->size = next_prime(ht->base_size);

  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(HashTableItem *));
  return ht;
}

static void ht_resize(HashTable *ht, const int base_size) {
  if (base_size < HT_INITIAL_BASE_SIZE) {
    return;
  }
  HashTable *new_ht = ht_new_sized(base_size);

  for (int i = 0; i < ht->size; i++) {
    HashTableItem *item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_insert(new_ht, item->key, item->value);
    }
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  // To delete new_ht, we give it ht's size and items
  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  HashTableItem **tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_delete(new_ht);
}

static void ht_resize_up(HashTable *ht) {
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

static void ht_resize_down(HashTable *ht) {
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}

static void ht_free_item(HashTableItem *item) {
  free(item->key);
  free(item->value);
  free(item);
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

/*
 * Returns:
 *  1  - prime
 *  0  - not prime
 *  -1 - undefined (i.e. x < 2)
 */
static int is_prime(const int x) {
  if (x < 2) {
    return -1;
  }
  if (x < 4) {
    return 1;
  }
  if ((x % 2) == 0) {
    return 0;
  }
  for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
    if ((x % i) == 0) {
      return 0;
    }
  }
  return 1;
}

static int next_prime(int x) {
  while (is_prime(x) != 1) {
    x++;
  }
  return x;
}

#endif // HT_IMPLEMENTATION

#endif // !HT_HASHTABLE
