/* CODA firmware information tool
 * SPDX-License-Identifier: 0BSD
 */
#include <stdint.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reorder.h"

#define MAX_TEMPLATE_LEN 12

enum coda_product_id {
	CODA_MX27_TO1 = 0xf000,
	CODA_DX6 = 0xf001,
	CODA_MX51 = 0xf00a,
	CODA_7541 = 0xf012,
	CODA_960 = 0xf020,
};

#define ANY 0xffff

static const uint16_t template_product_id[3] = {
	0xe004, ANY,	/* mv.h $0, product_id */
	0xae20,		/* lsl  $0, #16 */
};

static const uint16_t template_imx2[5] = {
	0xe005, ANY,	/* mv.h $1, firmware_version_number */
	0xe056,		/* or.w $0, $1 */
	0xe0c2, 0x0070,	/* st.w $0, 0x1c0 ; FIRMWARE_VERNUM */
};

static const uint16_t template_imx5[4] = {
	0xe00a, ANY,	/* or.h $0, firmware_version_number */
	0xe0c2, 0x0070,	/* st.w $0, 0x1c0 */
};

static const uint16_t template_imx6[12] = {
	0xe005, ANY,	/* mv.h $1, firmware_version_number */
	0xe056,		/* or.w $0, $1 */
	0xe0c2, 0x0070,	/* st.w $0, 0x1c0 ; FIRMWARE_VERNUM */
	0xe004, 0x0000, /* mv.h $0, #0x0 */
	0xae20,		/* lsl  $0, #16 */
	0xe00a, ANY,	/* or.h $0, firmware_code_revision */
	0xe0c2, 0x0071,	/* st.w $0, 0x1c4 ; FIRMWARE_CODE_REV */
};

static const char *coda_product_name(enum coda_product_id pid)
{
	switch (pid) {
	case CODA_MX27_TO1:
		return "i.MX27 TO1";
	case CODA_DX6:
		return "CodaDx6 (i.MX27 TO2)";
	case CODA_MX51:
		return "i.MX51";
	case CODA_7541:
		return "CODA7541 (i.MX53)";
	case CODA_960:
		return "CODA960 (i.MX6)";
	default:
		return "unknown";
	}
}

static const uint16_t *find_template(FILE *f, const uint16_t *template,
				     int template_len)
{
	static uint16_t code[MAX_TEMPLATE_LEN];
	unsigned char buf[2];
	uint16_t insn;
	int n, i = 0;
	int addr = 0;

	n = get_word(buf, f);
	if (n != 2)
		return NULL;

	while (n == 2) {
		insn = buf[0] | buf[1] << 8;
		if (insn == template[i] || template[i] == ANY)
			code[i++] = insn;
		else
			i = 0;
		if (i == template_len)
			return code;
		n = get_word(buf, f);
		addr += n;
	}
	return NULL;
}

int usage()
{
	fprintf(stderr, "Usage: coda-fwinfo vpu_fw.bin\n");
	return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
	uint16_t pid, ver = 0, rev = 0;
	const uint16_t *code;
	const char *inpath;
	FILE *f;
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

	code = find_template(f, template_product_id, 3);
	if (code == NULL) {
		if (errno)
			fprintf(stderr, "Failed to read from '%s': %d (%m)\n",
				argv[1], errno);
		else
			fprintf(stderr, "Failed to find product id template\n");
		return EXIT_FAILURE;
	}
	pid = code[1];

	printf("Product ID: 0x%04x, %s\n", pid, coda_product_name(pid));
	switch (pid) {
	case CODA_MX27_TO1:
	case CODA_DX6:
		code = find_template(f, template_imx2, 5);
		break;
	case CODA_MX51:
	case CODA_7541:
		code = find_template(f, template_imx5, 4);
		break;
	case CODA_960:
		code = find_template(f, template_imx6, 12);
		if (code)
			rev = code[9];
		break;
	}
	if (code == NULL) {
		fprintf(stderr, "Failed to find firmware version template\n");
		return EXIT_FAILURE;
	}
	ver = code[1];

	printf("Version Number: 0x%04x (%d.%d.%d)\n", ver, ver >> 12, (ver >> 8) & 0xf, ver & 0xff);
	if (rev)
		printf("Code Revision: %d\n", rev);
};
