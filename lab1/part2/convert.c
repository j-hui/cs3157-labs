/* Copyright (C) 2019 John Hui
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

int main(void)
{
	mpz_t a, b, c;
	char *s;

	mpz_inits(a, b, c, NULL);

	mpz_inp_str(a, NULL, 10);
	s = mpz_get_str(NULL, 10, a);
	printf("signed dec:   %s\n", s);
	free(s);

	mpz_set_str(b, "0", 10);
	if (mpz_cmp(a, b) < 0) {
		mpz_set_str(b, "2", 10);
		mpz_pow_ui(b, b, sizeof(int) * 8);
		mpz_add(b, b, a);
	} else {
		mpz_set(b, a);
	}

	s = mpz_get_str(NULL, 10, b);
	printf("unsigned dec: %s\n", s);

	s = mpz_get_str(NULL, 16, b);
	printf("hex:          %s\n", s);

	s = mpz_get_str(NULL, 2, b);

	printf("binary:       ");
	mpz_set_str(b, "1", 10);
	mpz_set_str(c, "0", 10);
	mpz_set_ui(a, sizeof(int) * 8);
	do {
		mpz_sub(a, a, b);
		if (mpz_get_ui(a) < strlen(s))
			printf("%c", s[strlen(s) - mpz_get_ui(a) - 1]);
		else
			printf("0");

		mpz_set_str(b, "4", 10);
		mpz_tdiv_r(c, a, b);
		if (!mpz_get_ui(c))
			printf(" ");

		mpz_set_str(b, "1", 10);
		mpz_set_str(c, "0", 10);
	} while (mpz_cmp(a, c));
	printf("\n");

	mpz_clears(a, b, c, NULL);
	return 0;
}
