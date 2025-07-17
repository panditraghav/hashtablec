# Hash table implementation in c

STB style single header file source code, just copy the hash_table.h file to your project repo and import like this
```c
#define HT_IMPLEMENTATION // Define this when you want to include implementation
#include "hash_table.h"
```

Also add math library as dependency in your build system
```meson
cc = meson.get_compiler('c')
math = cc.find_library('m')

executable('ht', src, include_directories: incdir, dependencies: [math])
```

## Public API
```c
typedef struct HashTableItem HashTableItem;
struct HashTableItem {
  char *key;
  char *value;
  HashTableItem *next;
};

typedef struct {
  int size;
  int base_size;
  int item_count;
  int list_count;
  HashTableItem **items;
} HashTable;

HashTable *ht_new();
void ht_insert(HashTable *ht, const char *key, const char *value);
char *ht_search(HashTable *ht, const char *key);
void ht_delete(HashTable *ht);
void ht_delete_item(HashTable *ht, const char *key);
```

## Build and run
```bash
meson setup builddir
meson compile -C builddir
./builddir/ht
```
