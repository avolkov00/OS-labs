#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int lsL(struct dirent * curdir, char * bufpath){
	struct stat stats;
	stat(bufpath, &stats);

	switch (stats.st_mode & S_IFMT) {
		case S_IFBLK:  printf("b");     break;
		case S_IFCHR:  printf("c");     break;
                case S_IFDIR:  printf("d");     break;
                case S_IFIFO:  printf("p");     break;
                case S_IFLNK:  printf("l");     break;
                case S_IFSOCK: printf("s");     break;
                default:       printf("-");     break;
	}
	
	printf((stats.st_mode & S_IRUSR) ? "r" : "-");
	printf((stats.st_mode & S_IWUSR) ? "w" : "-");
	printf((stats.st_mode & S_IXUSR) ? "x" : "-");
	printf((stats.st_mode & S_IRGRP) ? "r" : "-");
	printf((stats.st_mode & S_IWGRP) ? "w" : "-");
	printf((stats.st_mode & S_IXGRP) ? "x" : "-");
	printf((stats.st_mode & S_IROTH) ? "r" : "-");
	printf((stats.st_mode & S_IWOTH) ? "w" : "-");
	printf((stats.st_mode & S_IXOTH) ? "x" : "-");

	printf(" %d", stats.st_nlink);
	
	struct passwd *owner;
	struct group *group;
	owner = getpwuid(stats.st_uid);
	group = getgrgid(stats.st_gid);
	
	printf(" %s %s",owner->pw_name,group->gr_name);
	
	printf(" %ld", stats.st_size); 
	
	char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	printf(" %s", month[localtime(&stats.st_ctime)->tm_mon]);
	printf(" %d ", localtime(&stats.st_ctime)->tm_mday);

	time_t rawtime;
    	time(&rawtime);
	if ( localtime(&rawtime)->tm_year != localtime(&stats.st_ctime)->tm_year)
		printf("%d ", localtime(&stats.st_ctime)->tm_year + 1900);
	else
	{
		printf("%d:", localtime(&stats.st_ctime)->tm_hour);
		printf("%02d ", localtime(&stats.st_ctime)->tm_min);
	}
	printf("%s\n", curdir->d_name);
}

int main(int argc, char *argv[]){
	int rez=0;
	unsigned short Lflag = 0;
	unsigned short DIRflag = 0;
	while ((rez = getopt(argc,argv,"l")) != -1){
		switch (rez){
			case 'l': Lflag = 1; break;
			case '?': printf("Error found! Invalid argument.\n");break;
		}
	}
	DIR *dir;
	//printf("%d\n",optind);
	//printf("%s\n",argv[optind]);
	if ((argc > 2) || ((argc == 2) && (Lflag != 1))){
		dir = opendir(argv[optind]);	
		DIRflag = 1;
	}
	else
		dir = opendir(".");
	struct dirent *curdir;
	if (Lflag == 0){
		while(curdir = readdir(dir)){
			if (curdir->d_name[0]!='.') printf("%s ",curdir->d_name);
		}
		printf("\n");
	}
	else {
		struct stat DIRstats;
		(DIRflag) ? stat(argv[optind], &DIRstats) : stat(".", &DIRstats);
		printf("total %ld \n", DIRstats.st_blocks);
		while(curdir = readdir(dir)){
			if (curdir->d_name[0]!='.'){
				//printf("%s \n",curdir->d_name);
				char * bufpath;
				//printf(curdir->d_name);
				//printf("\n");
				if (DIRflag == 1){
					bufpath = malloc(strlen(argv[optind]) + strlen(curdir->d_name) + 1);
					memcpy(bufpath,argv[optind],strlen(argv[optind]));
					memcpy(bufpath+strlen(argv[optind]),curdir->d_name,strlen(curdir->d_name)+1);
				}
				else{
					bufpath = malloc(strlen(curdir->d_name) +1);
					memcpy(bufpath,curdir->d_name,strlen(curdir->d_name));
				}
				//printf(bufpath);
				lsL(curdir,bufpath);		
				free(bufpath);		
			}
		}
	}
	closedir(dir);
};
