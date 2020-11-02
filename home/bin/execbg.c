#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	pid_t pid;
	int i;
	argv++;

	if (!*argv) {
		fputs("Nothing to execute.\n", stderr);
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		fputs("Failed to fork().\n", stderr);
		exit(1);
	} else if (pid > 0) {
		wait(&i);
		exit(WEXITSTATUS(i));
	}

	if (setsid() < 0) {
		fputs("Failed to create a new session.\n", stderr);
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		fputs("Failed to fork() for the second time.\n", stderr);
		exit(1);
	} else if (pid > 0) {
		fprintf(stderr, "Forked to PID %d.\n", (int)pid);
		exit(0);
	}
	
	if (isatty(STDIN_FILENO)) {
		i = open("/dev/null", O_WRONLY);
		if (i < 0) {
			fputs("Failed to open /dev/null for writing.\n", stderr);
			return 1;
		}
		if (dup2(i, STDIN_FILENO) < 0) {
			close(i);
			fputs("Failed to redirect stdin.\n", stderr);
			return 1;
		}
		close(i);
	}

	execvp(*argv, argv);
	if (errno == ENOENT) {
		fprintf(stderr, "Command not found: '%s'\n", *argv);
		/* returned to nowhere, but still... */
		return 127;
	} else {
		fputs("Failed to execute:", stderr);
		for (; *argv; argv++)
			fprintf(stderr, " '%s'", *argv);
		putc('\n', stderr);
		return 126;
	}
}
