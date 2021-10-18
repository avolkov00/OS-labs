#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

void help() {
	printf("./chmod - h // вывести справку по использованию\n");
	printf("./chmod - uaw file1 // добавить право пользователю на запись\n");
	printf("./chmod - grr file1 // убрать право группы на чтение\n");
	printf("./chmod - oarwx file1 // добавить права на запись, чтение и исполнение файла для всех остальных\n");
	printf("Подробнее про флаги:\n");
	printf("Первым идёт флаг группы атрибутов(user / group / other)\n");
	printf("Второй флаг – добавление или удаление(add / remove)\n");
	printf("Далее идут от одного до трёх флагов атрибутов(read / write / execute)\n");
};

int main(int argc, char* argv[]) {
	int rights = 0, rez;
	char* filename;
	unsigned short  int u = 0, g = 0, o = 0, r = 0, w = 0, x = 0, ar = 0;

	if ((argc >= 3) && (argv[argc-1][0] != '-')) {
		filename = argv[argc - 1];
		//printf("%s\n",filename);
		if (access(filename, F_OK)) {
			printf("File doesn't exist\n");
			exit(0);
		}
	}
	else filename = NULL;



	struct stat stats;
	mode_t curmode;
	stat(filename, &stats);
	curmode = stats.st_mode;

	//printf("%d\n%d\n%d\n%s\n", curmode, S_IXUSR, 64);
	while ((rez = getopt(argc, argv, "ugoarwxh")) != -1) {
		switch (rez) {
		case 'u': u = 1; break;
		case 'g': g = 1; break;
		case 'o': o = 1; break;
		case 'r': if (ar == 0) { ar = 1; }
				else { r = 1; } break;
		case 'a': if (ar == 0) { ar = 2; }
				else { printf("Incorrect add/remove mode\n"); exit(0); } break;
		case 'w': w = 1; break;
		case 'x': x = 1; break;
		case 'h': help(); break;
		case '?':  break;
		}
	}
	rights = r * 4 + w * 2 + x;
	int realrights = 0;
	if (((u != 0) | (g != 0) | (o != 0)) && (ar != 0) && (rights != 0)) {
		if (u) realrights += rights * 64;
		if (g) realrights += rights * 8;
		if (o) realrights += rights;
		if (ar == 1) chmod(filename, curmode & (~realrights));
		else if (ar == 2) chmod(filename, curmode | realrights);
	}
	else if (argc != 2) {
		printf("Incorrect flag format\n");
	}
};
