#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

int main() {
	/*
    hashtable_t *table = ht_init(16);
    printf("Adding mapping from 10 -> 123\n");
    ht_add(table, 10, 123);
    printf("Get 10 -> %d (expected 123)\n", ht_get(table, 10));
    ht_add(table, 10, 256);
    printf("Adding mapping from 10 -> 256\n");
    printf("Get 10 -> %d (expected 256)\n", ht_get(table, 10));
    printf("Size = %d (expected 1)\n", ht_size(table));
    printf("Adding mapping from 20 -> 9\n");
    ht_add(table, 20, 9);
    printf("Get 20 -> %d (expected 9)\n", ht_get(table, 20));
    printf("Size = %d (expected 2)\n", ht_size(table));
	*/

	hashtable_t *memory = ht_init(250);

	char *instruction = "sw $1234567, 0($2)";

	char *instr = strdup(instruction);
	strsep(&instr, "$");
	char *firstreg = strsep(&instr, ",");
	firstreg = strsep(&firstreg, " ");
	char *imm = strsep(&instr, "$");
	imm = strsep(&imm, "(");
	char *secondreg = strsep(&instr, ")");
	secondreg = strsep(&secondreg, " ");

	int firstregcontents = atoi(firstreg);
	int secondregcontents = atoi(secondreg);

	long offset = strtol(imm, NULL, 0);
	if (offset & 0x8000) {
		offset = offset | 0xffff0000;
	}

	int value = firstregcontents;
	int s1 = value & 0xff;
	value = value >> 8;
	int s2 = value & 0xff;
	value = value >> 8;
	int s3 = value & 0xff;
	value = value >> 8;
	int s4 = value & 0xff;
	
	printf("%d\n", s1);
	printf("%d\n", s2);
	printf("%d\n", s3);
	printf("%d\n", s4);

	int hindex1 = secondregcontents + offset;
	int hindex2 = hindex1 + 1;
	int hindex3 = hindex2 + 1;
	int hindex4 = hindex3 + 1;

	printf("%d\n", hindex1);
	printf("%d\n", hindex2);
	printf("%d\n", hindex3);
	printf("%d\n", hindex4);

	ht_add(memory, hindex1, s1);
	ht_add(memory, hindex2, s2);
	ht_add(memory, hindex3, s3);
	ht_add(memory, hindex4, s4);

	printf("%d\n", ht_get(memory, hindex1));
	printf("%d\n", ht_get(memory, hindex2));
	printf("%d\n", ht_get(memory, hindex3));
	printf("%d\n", ht_get(memory, hindex4));

}
