# SoalShiftSISOP20_modul2_B09

Soal shift Sistem Operasi 2020
Modul 2
Kelompok B09

## #1 &ndash; Crontab tapi pake C

>Source code: [soal1.c](https://github.com/1Maximuse/SoalShiftSISOP20_modul2_B09/blob/master/soal1/soal1.c)
---
Untuk menerima argumen dan memastikan argumen dalam format yang benar, digunakan program sebagai berikut.
```c
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
  ...
}
```
Program tersebut mengecek beberapa hal:
* Memastikan jumlah argumen sesuai.
* Memastikan argumen detik, menit, dan jam berupa angka atau bintang (`*`).
* Memastikan argumen detik, menit, dan jam yang berupa angka berada dalam batasan.

Program akan mengeluarkan error dan akan keluar apabila terjadi salah satu error di atas.

---

Setelah itu, program akan membentuk daemon. Daemon akan melakukan perulangan setiap satu detik, sebagai berikut.

```c
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
```

Program akan mengecek setiap argumen (detik, menit, jam). Apabila ketiganya sesuai (termasuk wildcard), maka program akan melakukan fork dan menjalankan shell script sesuai argumen inputan.
