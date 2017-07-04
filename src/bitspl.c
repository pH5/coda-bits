/* CODA firmware splitter
 * SPDX-License-Identifier: 0BSD
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "reorder.h"

static int usage()
{
	fprintf(stderr, "Usage: bitspl vpu_fw.bin\n");
	return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
	unsigned char buf[0x800];
	unsigned char vector_start[8];
	char outname[32] = "00000.bin";
	unsigned int n, i, addr = 0;
	FILE *f, *outf;

	if (argc < 2)
		return usage();

	f = fopen(argv[1], "rb");
	if (!f) {
		fprintf(stderr, "Failed to open '%s': %s\n", argv[1],
			strerror(errno));
		return EXIT_FAILURE;
	}

	outf = fopen(outname, "wb");
	if (!f) {
		fprintf(stderr, "Failed to open '%s': %s\n", outname,
			strerror(errno));
		return EXIT_FAILURE;
	}

	for (i = 0; i < 8; i += 2) {
		n = get_word(buf + i, f);
		if (n != 2) {
			fprintf(stderr, "Failed to read from '%s': %s\n",
				argv[1], strerror(errno));
			return EXIT_FAILURE;
		}
	}

	memcpy(vector_start, buf, 8);

	printf("vector at 0x%05x, writing %05x.bin\n", addr, addr);

	while (n == 2) {
		for (i = 8; i < 0x800; i += 2) {
			n = get_word(buf + i, f);
			if (n != 2)
				break;
		}

		n = fwrite(buf, 1, 0x800, outf);
		if (n != 0x800) {
			fprintf(stderr, "Failed to write 0x%05x: %s\n", addr,
				strerror(errno));
			return EXIT_FAILURE;
		}

		memset(buf, 0, sizeof(buf));

		for (i = 0; i < 8; i += 2) {
			n = get_word(buf + i, f);
			if (n != 2)
				break;
		}

		addr += 0x800;

		if (memcmp(buf, vector_start, 8) == 0) {
			printf("vector at 0x%05x, writing %05x.bin\n", addr, addr);

			fclose(outf);

			snprintf(outname, sizeof(outname), "%05x.bin", addr);
			outf = fopen(outname, "wb");
			if (!f) {
				fprintf(stderr, "Failed to open '%s': %s\n", outname,
						strerror(errno));
				return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
}
