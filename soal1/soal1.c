#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wait.h>

int toInt(char c[]) {
	if (c[0] == '*') return -1;
	int x = 0;
	for (int i = 0; i < strlen(c); i++) {
		if (c[i] < '0' || c[i] > '9') return -2;
		x = (x << 3) + (x << 1);
		x += c[i] - '0';
	}
	return x;
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		printf("Argumen salah!\n");
		exit(EXIT_FAILURE);
	}
	int s, m, h;
	s = toInt(argv[1]);
	m = toInt(argv[2]);
	h = toInt(argv[3]);
	if (s == -2 || m == -2 || h == -2) {
		printf("Argumen salah!\n");
		exit(EXIT_FAILURE);
	}
	if (s != -1 && (s > 59 || s < 0)) {
		printf("Argumen salah!\n");
		exit(EXIT_FAILURE);
	}
	if (m != -1 && (m > 59 || m < 0)) {
		printf("Argumen salah!\n");
		exit(EXIT_FAILURE);
	}
	if (h != -1 && (h > 23 || h < 0)) {
		printf("Argumen salah!\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) exit(EXIT_FAILURE);
	if (pid > 0) exit(EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if (sid < 0) exit(EXIT_FAILURE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	while (1) {
		time_t t;
		time(&t);
		struct tm* lt = localtime(&t);
		if ((h == -1 || lt->tm_hour == h) && (m == -1 || lt->tm_min == m) && (s == -1 || lt->tm_sec == s)) {
			pid_t new = fork();
			if (new == 0) execl("/bin/sh", "bash", argv[4], NULL);
			else {
				int status;
				while (wait(&status) > 0);
			}
		}
		sleep(1);
	}
}
