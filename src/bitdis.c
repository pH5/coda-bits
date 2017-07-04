/* CODA BIT processor disassembler
 * SPDX-License-Identifier: 0BSD
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "iset.h"
#include "regs.h"
#include "reorder.h"

const char *find_reg_name(uint16_t addr)
{
	static char reg_name[22];
	int i;

	for (i = 0; regs[i].name; i++) {
		if (regs[i].addr == addr) {
			snprintf(reg_name, 22, " ; %s", regs[i].name);
			return reg_name;
		}
	}

	return "";
}

static int usage()
{
	fprintf(stderr, "Usage: bitdis vpu_fw.bin\n");
	return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
	unsigned char buf[2];
	unsigned int addr, insn, parm;
	char *inpath;
	FILE *f;
	int i, n;
	int opt;

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
		default:
			return usage();
		}
	}

	if (optind != argc - 1)
		return usage();
	inpath = argv[optind];

	f = fopen(inpath, "rb");
	if (!f) {
		fprintf(stderr, "Failed to open '%s': %s\n", argv[1],
			strerror(errno));
		return EXIT_FAILURE;
	}

	n = get_word(buf, f);
	if (n != 2) {
		fprintf(stderr, "Failed to read from '%s': %s\n", argv[1],
			strerror(errno));
		return EXIT_FAILURE;
	}

	printf("Disassembly of %s:\n\n", argv[1]);
	while (n == 2) {
		const struct inst_mask *imask;
		uint16_t code, reg_mask, imm_mask;
		enum inst_type type;

		insn = buf[0] | buf[1] << 8;
		for (i = 0; iset[i].code; i++) {
			type = iset[i].type;
			imask = &imasks[type];
			if ((insn & imask->code_mask) == iset[i].code) {
				code = iset[i].code;
				break;
			}
		}

		if (!iset[i].code) {
			printf("  %04x: %04x\n", addr, insn);
			addr += 2;
			n = get_word(buf, f);
			continue;
		}

		if (imask->parm) {
			n = get_word(buf, f);
			if (n != 2)
				break;
			parm = buf[0] | buf[1] << 8;
			printf("  %04x: %04x %04x", addr, insn, parm);
		} else {
			printf("  %04x: %04x     ", addr, insn);
		}

		printf("\t%s\t", iset[i].name);

		reg_mask = imask->reg_mask;
		imm_mask = imask->imm_mask;

		if (reg_mask == 0x3) {
			printf("$%u, $%u", insn & 1, (insn >> 1) & 1);
		} else if (reg_mask) {
			printf("$%u", !!(insn & reg_mask));
			if (imm_mask || imask->parm)
				printf(", ");
		}

		if (imm_mask) {
			int shift = ffs(imm_mask) - 1;
			uint16_t imm = (insn & imm_mask) >> shift;

			if (code == 0xa800) { /* and.b */
				printf("#0x%x", imm);
			} else if (type == INST_J) {
				/* short jumps */
				if (imm < 64)
					imm = addr + imm * 2;
				else
					imm = addr + imm * 2 - 256;
				printf("0x%x", imm);
			} else {
				printf("#%d", imm);
			}

			if (imask->parm)
				printf(", ");
		}

		if (imask->parm) {
			if (type == INST_PCU) {
				/* ipb load & store */
				parm *= 4;
				printf("0x%x%s", parm, find_reg_name(parm));
			} else if (type == INST_JUMP || type == INST_CALL) {
				/* long jumps and calls */
				parm *= 2;
				printf("0x%x", parm);
			} else {
				printf("0x%x", parm);
			}
			addr += 2;
		}

		printf("\n");

		/* unconditional jumps without return outside vector table */
		if ((type == INST_JUMP || type == INST_RET || type == INST_J) &&
		    (code & 0x000f) == 0x000e && addr >= 0x40)
			printf("\n");

		addr += 2;
		n = get_word(buf, f);
	};
}
