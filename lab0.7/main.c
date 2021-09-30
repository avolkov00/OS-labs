#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>



void inputArch(char * archname,char *  filename){
	FILE *f,*arch, * input;	
	int filesCount=0;
	if ((arch = fopen(archname,"r")) != NULL){
		fscanf(arch,"%d",&filesCount);
		fgetc(arch);
	}	
	int sizeOfFile[filesCount];
	int sizeOfName[filesCount];
	char * nameOfFiles[filesCount];
	char * files[filesCount];
	if (filesCount != 0){
		for (int i = 0; i < filesCount; i++){
			fscanf(arch,"%d",&sizeOfName[i]);
                        fgetc(arch);
			fscanf(arch,"%d",&sizeOfFile[i]);
			fgetc(arch);	
			nameOfFiles[i] = malloc(sizeOfName[i]);
                        fread(nameOfFiles[i],1, sizeOfName[i],arch);
			fgetc(arch);
			files[i] = malloc(sizeOfFile[i]);
			fread(files[i],1, sizeOfFile[i],arch);
			fgetc(arch);
		}
		fclose(arch);
	}
	for (int i = 0; i < filesCount; i++){
		if (strcmp(nameOfFiles[i],filename)==0){
			printf("Error! Such file already exists in archieve\n");
			for (int i = 0; i < filesCount; i++){
				free(files[i]);
				free(nameOfFiles[i]);
			}
			exit(1);
		}
	}	
	char * buffer = 0;
	long length = 0;	
	if ((f = fopen(filename,"r")) != NULL){
		fseek (f, 0, SEEK_END);
		length = ftell(f)-1;
		fseek (f, 0, SEEK_SET);
		buffer = malloc (length);
		fread (buffer, 1, length, f);
		fclose (f);	
	}
	else 
	{
		printf("No such file\n");	
		exit(1);
	}
	if ((arch = fopen(archname,"w")) != NULL){
		filesCount+=1;
		fprintf(arch,"%d\n",filesCount);
		if (filesCount != 1)
		{
			for (int i = 0; i < filesCount-1; i++){
				fprintf(arch,"%d\n",sizeOfName[i]);
				fprintf(arch,"%d\n",sizeOfFile[i]);
				fwrite(nameOfFiles[i],1,sizeOfName[i],arch);
                                fputc('\n',arch);
				fwrite(files[i],1,sizeOfFile[i],arch);
				fputc('\n',arch);
			}
		}
		fprintf(arch,"%d\n",strlen(filename));
		fprintf(arch,"%d\n",length);
		fprintf(arch,"%s\n",filename);
                fwrite(buffer,1,length,arch);
		fputc('\n',arch);
	}
	fclose(arch);
	for (int i = 0; i < filesCount-1; i++){
		free(files[i]);
		free(nameOfFiles[i]);
	}
	free(buffer);	
}	

void extractArch(char * archname,char *  filename){
        FILE *f,*arch, *output;
        int filesCount=0;
        if ((arch = fopen(archname,"r")) != NULL){
                fscanf(arch,"%d",&filesCount);
                fgetc(arch);
        }
	else{
		printf("No such archieve\n");
                exit(1);
	}
        int sizeOfFile[filesCount];
        int sizeOfName[filesCount];
        char * nameOfFiles[filesCount];
        char * files[filesCount];
        if (filesCount != 0){
                for (int i = 0; i < filesCount; i++){
                        fscanf(arch,"%d",&sizeOfName[i]);
                        fgetc(arch);
                        fscanf(arch,"%d",&sizeOfFile[i]);
                        fgetc(arch);
                        nameOfFiles[i] = malloc(sizeOfName[i]);
                        fread(nameOfFiles[i],1, sizeOfName[i],arch);
                        fgetc(arch);
                        files[i] = malloc(sizeOfFile[i]);
                        fread(files[i],1, sizeOfFile[i],arch);
                        fgetc(arch);
                }
                fclose(arch);
        }
	int fileNumber = -1;
	for (int i = 0; i < filesCount; i++){
		if (strcmp(nameOfFiles[i],filename) == 0){
			output  = fopen(filename,"w");
			fwrite(files[i],1,sizeOfFile[i],output);
			fclose(output);
			fileNumber = i;
			printf("File successfully extracted\n");
			break;
		}
	}
	if ((fileNumber != -1) && ((arch = fopen(archname,"w")) != NULL)){
                filesCount-=1;
                fprintf(arch,"%d\n",filesCount);
                for (int i = 0; i < filesCount+1; i++){
			if (i != fileNumber){
                                fprintf(arch,"%d\n",sizeOfName[i]);
                                fprintf(arch,"%d\n",sizeOfFile[i]);
                                fwrite(nameOfFiles[i],1,sizeOfName[i],arch);
                                fputc('\n',arch);
                                fwrite(files[i],1,sizeOfFile[i],arch);
                                fputc('\n',arch);
                        }
                }
		fclose(arch);
	}
	if (fileNumber == -1){
		printf("No such file in archieve\n");
	}

	for (int i = 0; i < filesCount; i++){
                free(files[i]);
                free(nameOfFiles[i]);
        }
}

void statArch(char * archname){
        FILE *f,*arch, *output;
        int filesCount=0;
        if ((arch = fopen(archname,"r")) != NULL){
                fscanf(arch,"%d",&filesCount);
                fgetc(arch);
        }
        else{
                printf("No such archieve\n");
                exit(1);
        }
        int sizeOfFile[filesCount];
        int sizeOfName[filesCount];
        char * nameOfFiles[filesCount];
        char * files[filesCount];
        if (filesCount != 0){
                for (int i = 0; i < filesCount; i++){
                        fscanf(arch,"%d",&sizeOfName[i]);
                        fgetc(arch);
                        fscanf(arch,"%d",&sizeOfFile[i]);
                        fgetc(arch);
                        nameOfFiles[i] = malloc(sizeOfName[i]);
                        fread(nameOfFiles[i],1, sizeOfName[i],arch);
                        fgetc(arch);
                        files[i] = malloc(sizeOfFile[i]);
                        fread(files[i],1, sizeOfFile[i],arch);
                        fgetc(arch);
                }
                fclose(arch);
        }
	long length = 0;
	if ((f = fopen(archname,"r")) != NULL){
                fseek (f, 0, SEEK_END);
                length = ftell(f);
                fclose (f);
        }
	printf("Archieve %s size: %d\n",archname,length);
	printf("Names and sizes of files in archieve:\n"); 
	for (int i = 0; i < filesCount; i++){ 
		printf("%s %d\n",nameOfFiles[i],sizeOfFile[i]);
	}
	for (int i = 0; i < filesCount; i++){
                free(files[i]);
                free(nameOfFiles[i]);
        }
}

void help(){
	printf("./archiver arch_name –i file1 // добавить в архив arch_name файл file1\n./archiver arch_name –e file1 // извлечь файл file1 из архива arch_name\n./archiver arch_name –s // вывести текущее состояние архива arch_name\n./archiver –h // вывести справку по работе с архиватором\n");
};

int main(int argc, char *argv[]){
	int rez=0;
	char * filename,* archname;
	if (argv[1][0] != '-')  archname = argv[1];
	else archname = NULL;
	while ((rez = getopt(argc,argv,"i:e:sh")) != -1){
		switch (rez){
			case 'i': if (archname != 0) inputArch(archname,optarg); else printf("Error, invalid archname\n"); break;
			case 'e': if (archname != 0) extractArch(archname,optarg); else printf("Error, invalid archname\n"); break;
			case 's': if (archname != 0) statArch(archname); else printf("Error, invalid archname\n"); break;
			case 'h': help(); break;
			case '?':  break;	
		}
	}
};
