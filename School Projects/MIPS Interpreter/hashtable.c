#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets
	linkedlist_t **table;
	int size;
	int count;
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
	hashtable_t *newhash = malloc(sizeof(hashtable_t));
	newhash->size = num_buckets;
	newhash->count = 0;
	newhash->table = calloc(newhash->size, sizeof(linkedlist_t*));
	for (int i = 0; i < num_buckets; i++) {
		newhash->table[i] = ll_init();
	}
	return newhash;
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
	int index = hash(key, table->size);
	linkedlist_t *tableindex = table->table[index];
	int size = ll_size(tableindex);
	ll_add(tableindex, key, value);
	if (ll_size(tableindex) > size) {
		table->count++;
	}
}

int ht_get(hashtable_t *table, int key) {
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
	int tableindex = hash(key, table->size);
	linkedlist_t *lltable = table->table[tableindex];
	return ll_get(lltable, key);
}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
	return table->count;
}
