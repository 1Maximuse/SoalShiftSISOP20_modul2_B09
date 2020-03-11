#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
	pid_t child = fork();
	if (child == 0) {
		execl("/bin/mkdir", "mkdir", "/home/noel/modul2/indomie", NULL);
	}
	child = fork();
	if (child == 0) {
		execl("/bin/mkdir", "mkdir", "/home/noel/modul2/sedaap", NULL);
	}
	int status;
	while(wait(&status) > 0);
	child = fork();
	if (child == 0) {
		execl("/usr/bin/unzip", "unzip", "jpg.zip", "-d", "jpg", NULL);
	}
	while(wait(&status) > 0);
	DIR* directory = opendir("jpg");
	struct dirent* item = readdir(directory);
	while (item != NULL) {
		if (item->d_type == (unsigned char)4 && (strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0)) {
			item = readdir(directory);
			continue;
		}
		char filename[300];
		sprintf(filename, "jpg/%s", item->d_name);
		child = fork();
		if (child == 0) {
			if (item->d_type == (unsigned char)4) {
				child = fork();
				if (child == 0) execl("/bin/mv", "mv", filename, "/home/noel/modul2/indomie/", NULL);
				while (wait(&status) > 0);
				char kosong1[300], kosong2[300];
				sprintf(kosong1, "/home/noel/modul2/indomie/%s/coba1.txt", item->d_name);
				sprintf(kosong2, "/home/noel/modul2/indomie/%s/coba2.txt", item->d_name);
				child = fork();
				if (child == 0) execl("/usr/bin/touch", "touch", kosong1, NULL);
				sleep(3);
				execl("/usr/bin/touch", "touch", kosong2, NULL);
			} else if (item->d_type == (unsigned char)8) {
				execl("/bin/mv", "mv", filename, "/home/noel/modul2/sedaap/", NULL);
			}
		}
		item = readdir(directory);
	}
	while (wait(&status) > 0);
}
