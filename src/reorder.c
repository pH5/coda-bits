/* CODA host memory layout to BIT processor layout reordering code
 * SPDX-License-Identifier: 0BSD
 */
#include <stdbool.h>
#include <stdio.h>

ssize_t get_word(unsigned char word[2], FILE *f)
{
	static int pos = 4;
	static bool reorder = false;
	static unsigned char buf[8];
	ssize_t n;
	int i;

	if (pos == 4) {
		n = fread(buf, 1, 8, f);
		if (n < 2)
			return n;
		pos = 0;

		if (buf[0] == 'M' && buf[1] == 'X') {
			/* skip Freescale header */
			fseek(f, 8, SEEK_CUR);
			n = fread(buf, 1, 8, f);
			if (n < 0)
				return 0;
		}

		if (buf[0] == 0x90 && buf[1] == 0xe1) {
			printf("Firmware in host memory order\n");
			reorder = true;
		} else if (buf[0] == 0x0e && buf[1] == 0xe4) {
			printf("Firmware in BIT processor memory order\n");
		} else {
			fprintf(stderr, "Unknown file type\n");
		}
	} else if (pos == 0) {
		n = fread(buf, 1, 8, f);
		if (n < 2)
			return n;
	}

	/* FIXME - this correctly reorders for i.MX51 and above only */
	if (reorder)
		i = 3 - pos;
	else
		i = pos;

	word[0] = buf[2 * i];
	word[1] = buf[2 * i + 1];

	pos = (pos + 1) % 4;

	return 2;
}
