/* CODA BIT processor instruction set
 * SPDX-License-Identifier: 0BSD
 */
#ifndef __ISET_H__
#define __ISET_H__

#include <stdbool.h>
#include <stdint.h>

enum inst_type {
	INST_ALUB,
	INST_ALUH,
	INST_ALUW,
	INST_PCU,
	INST_JUMP,
	INST_CALL,
	INST_RET,
	INST_J,
	INST_UNIQ,
	NUM_INST_TYPES,
};

static const struct inst_mask {
	uint16_t code_mask;
	uint16_t imm_mask;
	uint16_t reg_mask;
	bool parm;
} imasks[NUM_INST_TYPES] = {
	[INST_ALUB] = { 0xff00, 0x00fe, 0x0001, 0 },
	[INST_ALUH] = { 0xfffe, 0x0000, 0x0001, 1 },
	[INST_ALUW] = { 0xfffc, 0x0000, 0x0003, 0 },
	[INST_PCU]  = { 0xfffe, 0x0000, 0x0001, 1 },
	[INST_JUMP] = { 0xfffe, 0x0000, 0x0001, 1 },
	[INST_CALL] = { 0xfffe, 0x0000, 0x0001, 1 },
	[INST_RET]  = { 0xfffe, 0x0000, 0x0001, 0 },
	[INST_J]    = { 0xf00e, 0x0ff0, 0x0001, 0 },
	[INST_UNIQ] = { 0xffff, 0x0000, 0x0000, 0 },
};

static const struct inst {
	uint16_t code;
	enum inst_type type;
	char *name;
} iset[] = {
	{ 0xa000, INST_ALUB, "add.b"  },
	{ 0xa100, INST_ALUB, "sub.b"  },
	{ 0xa200, INST_ALUB, "mv.b" },
	{ 0xa800, INST_ALUB, "and.b"  },
	{ 0xae00, INST_ALUB, "lsl"  },
	{ 0xaf00, INST_ALUB, "lsr"  },
	{ 0xce00, INST_UNIQ, "int" },
	{ 0xe002, INST_ALUH, "sub.h" },
	{ 0xe004, INST_ALUH, "mv.h" },
	{ 0xe008, INST_ALUH, "and.h" },
	{ 0xe00a, INST_ALUH, "or.h" },
	{ 0xe040, INST_ALUW, "add.w" },
	{ 0xe044, INST_ALUW, "sub.w" },
	{ 0xe050, INST_ALUW, "and.w" },
	{ 0xe054, INST_ALUW, "or.w" },
	{ 0xe0c0, INST_PCU,  "ld.w" },
	{ 0xe0c2, INST_PCU,  "st.w" },
	{ 0xe190, INST_UNIQ, "nop" },
	{ 0xe402, INST_JUMP, "jumple" },
	{ 0xe408, INST_JUMP, "jumpnz" },
	{ 0xe40a, INST_JUMP, "jumpz" },
	{ 0xe40e, INST_JUMP, "jump" },
	{ 0xe412, INST_CALL, "callle" },
	{ 0xe418, INST_CALL, "callnz" },
	{ 0xe41a, INST_CALL, "callz" },
	{ 0xe41e, INST_CALL, "call" },
	{ 0xe422, INST_RET,  "retle" },
	{ 0xe428, INST_RET,  "retnz" },
	{ 0xe42a, INST_RET,  "retz" },
	{ 0xe42e, INST_RET,  "ret" },
	{ 0xf002, INST_J,    "jle" },
	{ 0xf008, INST_J,    "jnz" },
	{ 0xf00a, INST_J,    "jz" },
	{ 0xf00e, INST_J,    "j" },
	{ /* sentinel */ }
};

#endif /* __ISET_H__ */
