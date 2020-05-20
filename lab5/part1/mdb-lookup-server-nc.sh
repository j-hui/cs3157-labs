#!/bin/sh
set -e
tail -n $(expr "$(sed -n '$=' "$0")" - "$(sed -n '/^exec /=' "$0")") "$0" \
    > "$0-$$.c"
gcc -g -Wall -c "$0-$$.c"
gcc -o "$0-$$" "$0-$$.o"
rm -f "$0-$$.o" "$0-$$.c"
exec "$0-$$" $1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MDB_LOOKUP_PATH "/home/jae/cs3157-pub/bin/mdb-lookup-cs3157"
#define NC_COMMAND "nc"

volatile int sigint_flag = 0;

void sigint_handler(int signum)
{
	sigint_flag = 1;
}

int main(int argc, char **argv)
{
	char *port_num, mypipe_name[16];
	pid_t nc_pid, mdb_pid;
	int mypipe[2], yopipe[2];
	struct sigaction sa_int;

    remove(argv[0]);
    for (port_num = argv[0] + strlen(argv[0]); port_num > argv[0]; port_num--) {
        if (*port_num == '-') {
            *port_num = '\0';
            break;
        }
    }

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(2);
	}
	port_num = argv[1];

	snprintf(mypipe_name, sizeof(mypipe_name), "mypipe-%d", getpid());

	if (mkfifo(mypipe_name, 0644) < 0) {
		perror("mkfifo");
		goto mkfifo_fail;
	}
	if ((mypipe[0] = open(mypipe_name, O_RDONLY | O_NONBLOCK)) < 0) {
		perror("open mypipe[0]");
		goto open_mypipe0_fail;
	}
	if ((mypipe[1] = open(mypipe_name, O_WRONLY | O_NONBLOCK)) < 0) {
		perror("open mypipe[1]");
		goto open_mypipe1_fail;
	}

	if (pipe(yopipe)) {
		perror("yopipe");
		goto yopipe_fail;
	}

	signal(SIGINT, SIG_IGN); // TODO: print "Ignoring Ctrl-C"

	if ((nc_pid = fork()) < 0) {
		perror("fork nc");
		goto fork_nc_fail;
	}
	if (!nc_pid) {
		dup2(mypipe[0], STDIN_FILENO);
		close(mypipe[0]);
		close(mypipe[1]);

		dup2(yopipe[1], STDOUT_FILENO);
		close(yopipe[0]);
		close(yopipe[1]);

		execlp(NC_COMMAND, NC_COMMAND, "-l", port_num, (char *) NULL);
		perror("exec nc");
		goto exec_nc_fail;
	}

	if ((mdb_pid = fork()) < 0) {
		perror("fork mdb");
		goto fork_mdb_fail;
	}
	if (!mdb_pid) {
		dup2(yopipe[0], STDIN_FILENO);
		close(yopipe[0]);
		close(yopipe[1]);

		dup2(mypipe[1], STDOUT_FILENO);
		close(mypipe[0]);
		close(mypipe[1]);

		execl(MDB_LOOKUP_PATH, MDB_LOOKUP_PATH, (char *) NULL);
		perror("exec mdb");
		goto exec_mdb_fail;
	}

	close(mypipe[0]);
	close(mypipe[1]);
	close(yopipe[0]);
	close(yopipe[1]);

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
	do {
		if (sigint_flag) {
			printf("Ignoring Ctrl-C\n");
			sigint_flag = 0;
		}
	} while (waitpid(-1, NULL, 0) > 0 || errno == EINTR);

	remove(mypipe_name);

	return 0;

exec_mdb_fail:
	kill(mdb_pid, SIGTERM);
fork_mdb_fail:
exec_nc_fail:
	kill(nc_pid, SIGTERM);
fork_nc_fail:
	close(yopipe[1]);
	close(yopipe[0]);
yopipe_fail:
	close(mypipe[1]);
open_mypipe1_fail:
	close(mypipe[0]);
open_mypipe0_fail:
	remove(mypipe_name);
mkfifo_fail:
	return 1;
}
