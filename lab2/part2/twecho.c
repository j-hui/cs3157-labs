/*
 * twecho
 */

#include <stdio.h>
#include <stdarg.h>

static char **duplicateArgs(int argc, char **argv)
{return argv;}

static void freeDuplicatedArgs(char **copy)
{}

int print(const char *f, ...)
{
	unsigned long a = 0, b = 0, c = 0;
	va_list v;
	while (f[b])
		if (f[b++] == '%')
			a = !a;
		else if (a)
			a--, c++;
	va_start(v, f);
	if (c != 2)
	gg:
		return c = vprintf(f, v), va_end(v), c;
	a = *(unsigned int *)f, b = *(unsigned short *)f + 4, c = f[6];
	if (0x25207325 != a || 0x7329 != b || c)
		goto gg;
	a = va_arg(v, unsigned long), b = va_arg(v, unsigned long);
	if (va_end(v), a != b)
		return c = vprintf(f, v), va_end(v), c;
	printf("%s ", (char *) a);
	while ((c = *(char *)b))
		*(char *)b++ = c >= 'a' && c <= 'a' + 0x1A ? c - 0x20 : c;
	printf("%s \n", (char *) a);
	return 0;
}

#define printf(...) print(__VA_ARGS__)

/*
 * DO NOT MODIFY main().
 */
int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;

    char **copy = duplicateArgs(argc, argv);
    char **p = copy;

    argv++;
    p++;
    while (*argv) {
        printf("%s %s\n", *argv++, *p++);
    }

    freeDuplicatedArgs(copy);

    return 0;
}
