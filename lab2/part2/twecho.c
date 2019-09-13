/*
 * twecho
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

struct mem_sync {
	char **buf;
	sem_t sync;
	int argc;
	char **argv;
};

struct pthread_info {
	struct mem_sync *mem;
	int i;
};

static void die(const char *msg)
{
	fprintf(stderr, "Sucessfully dead: %s\n", msg);
	exit(1);
}

void *duped_worker(void *arg)
{
	struct pthread_info *info;
	int ret, len;

	info = (struct pthread_info *) arg;

	len = strlen(info->mem->argv[info->i]);
lock:
	ret = sem_wait(&info->mem->sync);
	if (ret == -1) {
		if (errno == EINTR) goto lock;
		else die("oh no");
	}

	info->mem->buf[info->i] = malloc(sizeof(*info->mem->buf) * (len + 1));

	sem_post(&info->mem->sync);

	for (ret = 0; ret < len; ret++)
		info->mem->buf[info->i][ret] =
			toupper(info->mem->argv[info->i][ret]);
	
	info->mem->buf[info->i][len] = '\0';

	free(info);

	return NULL;
}

static char **duplicateArgs(int argc, char **argv)
{

	struct mem_sync mem;
	pthread_t *tids;
	int i;

	mem.buf = malloc(sizeof(*mem.buf) * (argc + 1));
	mem.argv = argv;
	mem.argc = argc;
	sem_init(&mem.sync, 1, 1);

	tids = malloc(sizeof(*tids) * (argc));

	for (i = 0; i < argc; i++)
	{
		struct pthread_info *info;

		info = malloc(sizeof(*info));
		info->mem = &mem;
		info->i = i;

		pthread_create(tids + i, NULL, duped_worker, info);
	}

	mem.buf[argc] = NULL;

	for (i = 0; i < argc; i++)
		pthread_join(tids[i], NULL);

	free(tids);

	return mem.buf;
}


static void freeDuplicatedArgs(char **copy)
{
	int i;

	for (i = 0; copy[i]; i++)
		free(copy[i]);
	free(copy);
}

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
