typedef struct {
  char *key;
  char *value;
} HashTableItem;

typedef struct {
  int size;
  int count;
  HashTableItem **items;
} HashTable;

HashTable *ht_new();
void ht_delete_hash_table(HashTable *ht);

void ht_insert(HashTable *ht, const char *key, const char *value);
char *ht_search(HashTable *ht, const char *key);
void ht_delete(HashTable *ht, const char *key);
