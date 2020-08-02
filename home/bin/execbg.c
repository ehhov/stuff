#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	pid_t pid;
	int fd;
	argv++;

	if (!*argv) {
		fprintf(stderr, "Nothing to execute.\n");
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Failed to fork().\n");
		exit(1);
	} else if (pid > 0) {
		exit(0);
	}

	if (setsid() < 0) {
		fprintf(stderr, "Failed to create a new session.\n");
		exit(1);
	}

	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Failed to fork() for the second time.\n");
		exit(1);
	} else if (pid > 0) {
		fprintf(stderr, "Forked to PID %d.\n", (int)pid);
		exit(0);
	}
	
	if (isatty(STDIN_FILENO)) {
		fd = open("/dev/null", O_WRONLY);
		if (fd < 0) {
			fprintf(stderr, "Failed to open /dev/null for writing.\n");
			return 1;
		}
		if (dup2(fd, STDIN_FILENO) < 0) {
			close(fd);
			fprintf(stderr, "Failed to redirect stdin.\n");
			return 1;
		}
		close(fd);
	}

	execvp(*argv, argv);
	if (errno == ENOENT) {
		fprintf(stderr, "Command not found:\n");
		for (; *argv; argv++)
			fprintf(stderr, "'%s' ", *argv);
		fprintf(stderr, "\b\n");
		return 127;
	} else {
		fprintf(stderr, "Command failed to execute:\n");
		for (; *argv; argv++)
			fprintf(stderr, "'%s' ", *argv);
		fprintf(stderr, "\b\n");
		return 126;
	}
}
