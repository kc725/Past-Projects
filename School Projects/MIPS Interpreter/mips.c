#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "mips.h"

	/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int UNKNOWN_TYPE = 3;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
int get_op_type(char *op) {
	const char *r_type_op[] = { "addu", "subu", "and", "or", "xor", "nor", "slt", "sltu" , "movn", "movz", "sllv", "srlv", "srav" };
	const char *i_type_op[] = { "addiu", "andi", "ori", "xori", "slti", "sltiu", "sll", "srl", "sra", "lui" };
	const char *mem_type_op[] = { "lw", "lb", "lbu", "sw", "sb" };
	for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++) {
		if (strcmp(r_type_op[i], op) == 0) {
			return R_TYPE;
		}
	}
	for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++) {
		if (strcmp(i_type_op[i], op) == 0) {
			return I_TYPE;
		}
	}
	for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++) {
		if (strcmp(mem_type_op[i], op) == 0) {
			return MEM_TYPE;
		}
	}
	return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *memory;

void init(registers_t *starting_registers) {
	registers = starting_registers;

	// TODO: initialize any additional variables needed for state
	memory = ht_init(250);
}

// TODO: create any necessary helper functions

void step(char *instruction) {

	// Extracts and returns the substring before the first space character,
	// by replacing the space character with a null-terminator.
	// `instruction` now points to the next character after the space
	// See `man strsep` for how this library function works
	char *op = strsep(&instruction, " ");
	// Uses the provided helper function to determine the type of instruction
	int op_type = get_op_type(op);
	// Skip this instruction if it is not in our supported set of instructions
	if (op_type == UNKNOWN_TYPE) {
		return;
	}

	// TODO: write logic for evaluating instruction on current interpreter state
	if (op_type == 0) {
		char *instr = strdup(instruction);
		strsep(&instr, "$");
		char *firstreg = strsep(&instr, ",");
		firstreg = strsep(&firstreg, " ");
		strsep(&instr, "$");
		char *secondreg = strsep(&instr, ",");
		secondreg = strsep(&secondreg, " ");
		strsep(&instr, "$");
		char *thirdreg = strsep(&instr, " ");

		int firstregcontents = atoi(firstreg);
		int secondregcontents = atoi(secondreg);
		int thirdregcontents = atoi(thirdreg);

		int firstarg = registers->r[secondregcontents];
		int secondarg = registers->r[thirdregcontents];

		if (firstregcontents != 0) {
			if (strcmp(op, "addu") == 0) {
				registers->r[firstregcontents] = firstarg + secondarg;
			}
			else if (strcmp(op, "subu") == 0) {
				registers->r[firstregcontents] = firstarg - secondarg;
			}
			else if (strcmp(op, "and") == 0) {
				registers->r[firstregcontents] = firstarg & secondarg;
			}
			else if (strcmp(op, "or") == 0) {
				registers->r[firstregcontents] = firstarg | secondarg;
			}
			else if (strcmp(op, "xor") == 0) {
				registers->r[firstregcontents] = firstarg ^ secondarg;
			}
			else if (strcmp(op, "movz") == 0) {
				if (secondarg == 0) {
					registers->r[firstregcontents] = firstarg;
				}
			}
			else {
				if (firstarg < secondarg) {
					registers->r[firstregcontents] = 1;
				}
				else {
					registers->r[firstregcontents] = 0;
				}
			}
		}
	}

	else if (op_type == 1) {
		if (strcmp(op, "lui") == 0) {
			char *instr = strdup(instruction);
			strsep(&instr, "$");
			char *freg = strsep(&instr, ",");
			freg = strsep(&freg, " ");
			long secondarg = strtol(instr, NULL, 0);
			int reg = atoi(freg);
			if (reg != 0) {
				registers->r[reg] = secondarg << 16;
			}
		}
		else {
			char *instr = strdup(instruction);
			strsep(&instr, "$");
			char *firstreg = strsep(&instr, ",");
			firstreg = strsep(&firstreg, " ");
			strsep(&instr, "$");
			char *secondreg = strsep(&instr, ",");
			secondreg = strsep(&secondreg, " ");
			long secondarg = strtol(instr, NULL, 0);

			int firstregcontents = atoi(firstreg);
			int secondregcontents = atoi(secondreg);

			int firstarg = registers->r[secondregcontents];

			if (firstregcontents != 0) {
				if (strcmp(op, "addiu") == 0) {
					if (secondarg & 0x8000) {
						secondarg = secondarg | 0xffff0000;
					}
					registers->r[firstregcontents] = firstarg + secondarg;
				}
				else if (strcmp(op, "andi") == 0) {
					secondarg = secondarg & 0xffff;
					registers->r[firstregcontents] = firstarg & secondarg;
				}
				else if (strcmp(op, "ori") == 0) {
					secondarg = secondarg & 0xffff;
					registers->r[firstregcontents] = firstarg | secondarg;
				}
				else if (strcmp(op, "xori") == 0) {
					secondarg = secondarg & 0xffff;
					registers->r[firstregcontents] = firstarg ^ secondarg;
				}
				else if (strcmp(op, "slti") == 0) {
					if (firstarg < secondarg) {
						registers->r[firstregcontents] = 1;
					}
					else {
						registers->r[firstregcontents] = 0;
					}
				}
				else if (strcmp(op, "xori") == 0) {
					secondarg = secondarg & 0xffff;
					registers->r[firstregcontents] = firstarg ^ secondarg;
				}
				else if (strcmp(op, "sll") == 0) {
					registers->r[firstregcontents] = firstarg << secondarg;
				}
				else if (strcmp(op, "sra") == 0) {
					signed int farg = firstarg;
					registers->r[firstregcontents] = farg >> secondarg;
				}
			}
		}
	}
	else if (op_type == 2) {
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

		if (strcmp(op, "sw") == 0) {
			int value = registers->r[firstregcontents];
			int s1 = value & 0xff;
			value = value >> 8;
			int s2 = value & 0xff;
			value = value >> 8;
			int s3 = value & 0xff;
			value = value >> 8;
			int s4 = value & 0xff;

			int hindex1 = registers->r[secondregcontents] + offset;
			int hindex2 = hindex1+1;
			int hindex3 = hindex2+1;
			int hindex4 = hindex3+1;

			ht_add(memory, hindex1, s1);
			ht_add(memory, hindex2, s2);
			ht_add(memory, hindex3, s3);
			ht_add(memory, hindex4, s4);
		}
		else if (strcmp(op, "sb") == 0) {
			int value = registers->r[firstregcontents];
			value = value & 0xff;
			int hindex = registers->r[secondregcontents] + offset;
			ht_add(memory, hindex, value);
		}
		else if (strcmp(op, "lbu") == 0) {
			if (firstregcontents != 0) {
				int hindex = registers->r[secondregcontents] + offset;
				int value = ht_get(memory, hindex);
				registers->r[firstregcontents] = value;
			}
		}
		else if (strcmp(op, "lb") == 0) {
			if (firstregcontents != 0) {
				int hindex = registers->r[secondregcontents] + offset;
				int value = ht_get(memory, hindex);
				if (value & 0x80) {
					value = value | 0xffffff00;
				}
				registers->r[firstregcontents] = value;
			}
		}
		else {
			if (firstregcontents != 0) {
				int hindex1 = registers->r[secondregcontents] + offset;
				int hindex2 = hindex1 + 1;
				int hindex3 = hindex2 + 1;
				int hindex4 = hindex3 + 1;

				int l1 = ht_get(memory, hindex1);
				int l2 = ht_get(memory, hindex2);
				int l3 = ht_get(memory, hindex3);
				int l4 = ht_get(memory, hindex4);

				int value = l4;
				value = value << 8;
				value = value | l3;
				value = value << 8;
				value = value | l2;
				value = value << 8;
				value = value | l1;

				registers->r[firstregcontents] = value;
			}
		}
	}

}
