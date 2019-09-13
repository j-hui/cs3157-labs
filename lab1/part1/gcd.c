#include <gmp.h>
#include "gcd.h"

void gcd(mpz_t d, mpz_t a, mpz_t b)
{
	mpz_gcd(d, a, b);
}
