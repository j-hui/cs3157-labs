#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define c(a, i) (((char *)(&a)) + i)
#define s(a, i) ((short *)c(a, i))

int main(int fd, char **a)
{
    if (fd != 2 || (fd = open(a[1], O_RDONLY)) < 2) exit(1);
begin:
    if (lseek(fd, 0, SEEK_SET) < 0) goto exuent;

    *c(a, 0) = 'l'; if (write(1, c(a, 0), 1) != 1) goto exuent;
    *c(a, 0) = 'o'; if (write(1, c(a, 0), 1) != 1) goto exuent;
    *c(a, 1) = 'k'; if (write(1, c(a, 0), 2) != 2) goto exuent;
    *c(a, 0) = 'u'; if (write(1, c(a, 0), 1) != 1) goto exuent;
    *c(a, 0) = 'p'; if (write(1, c(a, 0), 1) != 1) goto exuent;
    *c(a, 0) = ':'; if (write(1, c(a, 0), 1) != 1) goto exuent;
    *c(a, 0) = ' '; if (write(1, c(a, 0), 1) != 1) goto exuent;

    *c(a, 7) ^= *c(a, 7);

input:
    if (read(0, c(a, *c(a, 7)), 1) != 1) goto exuent;
    if (*c(a, *c(a, 7)) == '\n' || !++(*c(a, 7))) goto search;
    if (*c(a, 7) == 5) goto drain;
    goto input;

drain:
    if (read(0, c(a, 5), 1) != 1) goto exuent;
    if (*c(a, 5) == '\n') goto search;
    goto drain;

search:
    if (read(fd, c(a, 5), 1) != 1) goto begin;
    if (lseek(fd, -1, SEEK_CUR) < 0) goto exuent;
    *c(a, 6) ^= *c(a, 6);
    if (!(*c(a, 7) & ~15)) *c(a, 7) |= *c(a, 7) << 4;
    else *c(a, 7) = (*c(a, 7) & ~15) | (*c(a, 7) & ~15) >> 4;
    if (!*c(a, 7)) goto print;

srch:
    if (read(fd, c(a, 5), 1) != 1) goto exuent;
    if (((++*c(a, 6)) & 64)) if ((*c(a, 6) & 63) == 40) goto search; else;
    else if ((*c(a, 6) & 63) == 16 && (*c(a, 6) = (*c(a, 6) & 127) | 64)) goto srch;
    if ((*c(a, 6) & 128 || !*c(a, 5)) && (*c(a, 6) |= 128)) goto srch;

    if (*c(a, 5) == *c(a, ((*c(a, 7) & ~15) >> 4) - (*c(a, 7) & 15))) --*c(a, 7);
    else *c(a, 7) = (*c(a, 7) & ~15) | (*c(a, 7) & ~15) >> 4;
    if (!(*c(a, 7) & 15)) if (lseek(fd, -(*c(a, 6) & 63), SEEK_CUR) < 0) goto exuent; else goto print; else;
    goto srch;

print:
    if ((*s(a, 5) = lseek(fd, 0, SEEK_CUR) / 40) < 0) goto exuent;
    *c(a, 5) = (*s(a, 5) > 9999) ? '0' + ((*s(a, 5) % 100000) / 10000) : ' ';
    if (write(1, c(a, 5), 1) != 1) goto exuent;

    if ((*s(a, 5) = lseek(fd, 0, SEEK_CUR) / 40) < 0) goto exuent;
    *c(a, 5) = (*s(a, 5) > 999) ? '0' + ((*s(a, 5) % 10000) / 1000) : ' ';
    if (write(1, c(a, 5), 1) != 1) goto exuent;

    if ((*s(a, 5) = lseek(fd, 0, SEEK_CUR) / 40) < 0) goto exuent;
    *c(a, 5) = (*s(a, 5) > 99) ? '0' + ((*s(a, 5) % 1000) / 100) : ' ';
    if (write(1, c(a, 5), 1) != 1) goto exuent;

    if ((*s(a, 5) = lseek(fd, 0, SEEK_CUR) / 40) < 0) goto exuent;
    *c(a, 5) = (*s(a, 5) > 9) ? '0' + ((*s(a, 5) % 100) / 10) : ' ';
    if (write(1, c(a, 5), 1) != 1) goto exuent;

    *c(a, 5) = '0' + ((((lseek(fd, 0, SEEK_CUR) / 40) % 10) + 1) % 10);
    if (write(1, c(a, 5), 1) != 1) goto exuent;

    *c(a, 5) = ':'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = ' '; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = '{'; if (write(1, c(a, 5), 1) != 1) goto exuent;

    *c(a, 6) ^= *c(a, 6);
prnt:
    if (read(fd, c(a, 5), 1) != 1) goto exuent;
    if (((++*c(a, 6)) & 64)) if ((*c(a, 6) & 63) == 40) goto next; else;
    else if ((*c(a, 6) & 63) == 16 && (*c(a, 6) = (*c(a, 6) & 127) | 64)) goto said;
    if ((*c(a, 6) & 128 || !*c(a, 5)) && (*c(a, 6) |= 128)) goto prnt;
    if (write(1, c(a, 5), 1) != 1) goto exuent;
    goto prnt;
said:
    *c(a, 5) = '}'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = ' '; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = 's'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = 'a'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = 'i'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = 'd'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = ' '; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = '{'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    goto prnt;
next:
    *c(a, 5) = '}'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    *c(a, 5) = '\n'; if (write(1, c(a, 5), 1) != 1) goto exuent;
    goto search;

exuent:
    close(fd);
    return 0;
}
