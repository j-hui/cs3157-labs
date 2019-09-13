#include <gmp.h>
#include "prime.h"

int prime(mpz_t n)
{
	return mpz_probab_prime_p(n, 1000000);
}
