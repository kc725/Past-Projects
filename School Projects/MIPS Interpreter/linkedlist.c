#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <stdbool.h>

struct linkedlist {
    struct linkedlist_node *first;
    // TODO: define linked list metadata
	int size;
};

typedef struct linkedlist_node {
    // TODO: define the linked list node
	int key;
	int value;
	struct linkedlist_node *next;
} linkedlist_node_t;

linkedlist_t *ll_init() {
    // TODO: create a new linked list
	linkedlist_t *newlist;
	newlist = malloc(sizeof(linkedlist_t));
	newlist->size = 0;
	return newlist;
}

void ll_add(linkedlist_t *list, int key, int value) {
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
	int check = ll_get(list, key);
	if (ll_size(list) == 0 || check == 0) {
		linkedlist_node_t *newnode = malloc(sizeof(linkedlist_node_t));
		newnode->key = key;
		newnode->value = value;
		newnode->next = list->first;
		list->first = newnode;
		list->size++;
	}
	else {
		linkedlist_node_t *temp = list->first;
		for (int i = 0; i < ll_size(list); i++) {
			if (temp->key == key) {
				temp->value = value;
				break;
			}
			temp = temp->next;
		}
	}
}

int ll_get(linkedlist_t *list, int key) {
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
	linkedlist_node_t *check = list->first;
	for (int i = 0; i < ll_size(list); i++) {
		if (check->key == key) {
			return check->value;
		}
		check = check->next;
	}
	return 0;
}

int ll_size(linkedlist_t *list) {
    // TODO: return the number of nodes in this linked list
	return list->size;
}
