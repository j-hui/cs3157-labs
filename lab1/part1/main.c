#include <stdio.h>
#include <gmp.h>
#include "prime.h"
#include "gcd.h"

int main(void)
{
	mpz_t a, b, c, d;

	mpz_inits(a, b, c, d, NULL);

	printf("Please input the first integer: ");
	mpz_inp_str(a, NULL, 10);

	printf("Please input the second integer: ");
	mpz_inp_str(b, NULL, 10);

	printf("You typed in ");
	mpz_out_str(NULL, 10, a);
	printf(" and ");
	mpz_out_str(NULL, 10, b);
	printf("\n");

	mpz_add(c, a, b);
	mpz_set_str(d, "2", 10);
	mpz_tdiv_qr(c, d, c, d);

	printf("The average is ");
	mpz_out_str(NULL, 10, c);
	mpz_set_str(d, "1", 10);
	if (mpz_cmp(c, d)) {
		printf(".");
		mpz_set_str(d, "5", 10);
		mpz_out_str(NULL, 10, d);
	}
	printf("\n");

	mpz_out_str(NULL, 10, a);
	printf(" is ");
	switch (prime(a)) {
	case 0: printf("not"); break;
	case 1: printf("probably"); break;
	case 2: printf("certainly"); break;
	default: break;
	}
	printf(" a prime number\n");

	mpz_out_str(NULL, 10, b);
	printf(" is ");
	switch (prime(b)) {
	case 0: printf("not"); break;
	case 1: printf("probably"); break;
	case 2: printf("certainly"); break;
	default: break;
	}
	printf(" a prime number\n");

	gcd(c, a, b);
	mpz_set_str(d, "1", 10);

	mpz_out_str(NULL, 10, a);
	printf(" and ");
	mpz_out_str(NULL, 10, b);
	printf(" are ");
	if (mpz_cmp(c, d))
		printf("not ");
	printf("co-prime\n");

	mpz_clears(a, b, c, d, NULL);
	return 0;
}
