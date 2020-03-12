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

## #2 &ndash; Program Kiwa
>Source code: [soal2.c](https://github.com/1Maximuse/SoalShiftSISOP20_modul2_B09/blob/master/soal2/soal2.c)

Sebelum membentuk daemon, program memastikan bahwa argumen sesuai dengan kode berikut:
```c
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Argumen Salah!\n");
		exit(EXIT_FAILURE);
	}
	int mode;
	if (argv[1][1] == 'a') mode = 1;
	else if (argv[1][1] == 'b') mode = 0;
	...
```

Program menghandle dua mode berbeda, A dan B. Untuk mode A, setiap child process diberi aturan sehingga ia akan *terminate* apabila parentnya *terminate*. Namun pada mode B, child processnya tidak akan diberi peraturan demikian, sehingga akan tetap berjalan sampai child process itu selesai. Ini dilakukan dengan bagian berikut:
```c
if (mode) prctl(PR_SET_PDEATHSIG, SIGHUP);
```

---

Setelah argumen dicek sebagai valid, program akan membentuk suatu daemon. Program ini memiliki susunan proses sebagai berikut:

![](https://github.com/1Maximuse/SoalShiftSISOP20_modul2_B09/blob/master/soal2.png)

Daemon utama (proses 0) menjalankan loop setiap 30 detik. Proses tersebut membuat child process (1) yang akan membuat direktori baru sesuai timestamp. Parent (0) akan menunggu sampai child process `mkdir` (1) selesai, kemudian membuat child process lagi yang akan meng-handle setiap folder (2).

Proses untuk setiap folder (2) akan menjalankan loop setiap 5 detik. Setiap loop nya, ia (2) akan membentuk child process baru (3) yang akan melakukan `wget` untuk mengambil gambar sesuai ukuran yang diminta.

Setelah loop pada proses folder (2) selesai berjalan 20 kali, parent (2) akan memastikan bahwa semua child process `wget` (3) yang ada sudah selesai, kemudian menjalankan child process baru untuk melakukan perintah `zip` (4).

Proses folder selesai dengan melakukan perintah `rm` untuk menghapus foldernya.
