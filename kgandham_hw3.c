/* 
Name: Kiran Kumar Gandham
BlazerId: kgandham
Project #: HW3
To compile: gcc -o kgandham_hw3 kgandham_hw3.c
To run: <instructions to run the program> 
*/ 


#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>

#define STR_LEN 10000

char temp1[STR_LEN];

char cmdid[100] = {};

typedef int fncOne(const char* a);


typedef char* fncTwo(char* b);


typedef void fncThree(char* dirname, int count, int size, int depth, char* substring, char* other);

typedef void fncFour(char* dirname, int count, int size, int depth, char* substring, char* other, char* command);

int opfunc(const char* a, fncOne *f) {
return f(a);
}

char* opfunc1(char* b, fncTwo *f) {
return f(b);
}

void opfunc2(char* dirname, int count, int size, int depth, char* substring, char* other,fncThree *f) {
return f(dirname,count,size,depth,substring,other);
}

void opfunc3(char* dirname, int count, int size, int depth, char* substring, char* other,char* command, fncFour *f) {
return f(dirname,count,size,depth,substring,other,command);
}


int isDir(const char* fileName)
{
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

int tempDepth = 0;

char* getFileCreationTime(char *path) {
    struct stat attr;
    stat(path, &attr);
    return ctime(&attr.st_mtime);
}

void printSubdirectories(char* dirname, int count, int size, int depth, char* substring, char* other) {
    struct dirent *files;
    struct stat fileStat;
    DIR *dir = opendir(dirname);
    if (dir == NULL){
        printf("Directory cannot be opened!" );
        return;
    }
    char tempList[100][100];
    while ((files = readdir(dir)) != NULL) {
        if(strcmp(files->d_name, ".") && strcmp(files->d_name, "..")) {
            if(depth == -1) {
                char temp[100] = {};
                strcat(temp, dirname);
                strcat(temp, "/");
                strcat(temp, files->d_name);
	        //printf("%s\n",temp);
                // calculate /
                int tempCount = 0;
                for(int i = 0; i < sizeof temp; i++) 
                    if(temp[i] == '/')
                        tempCount++;
                //printf("%d\n",tempCount);
		//printf("%d\n",count);
		if(size == 0)
                    for(int i = 0; i < tempCount - count; i++)
                        printf("\t");
                // print files and directory name
                if(strcmp(other, "-S") == 0) {
                    char* modifiedTime = getFileCreationTime(temp);
                    if(isDir(temp) == 0) {
                        if(size != 0) 
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
                        printf("%s\n", files->d_name);
                        printSubdirectories(temp, count, size, -1, substring, "-S");
                    }
                    else {
                        struct stat st;
                        stat(temp, &st);
                        off_t size1 = st.st_size;
                        if(size != 0) {
                            if(size1 < size ) {
                                for(int i = 0; i < tempCount - count; i++)
                                    printf("\t");
                                printf("%s\t(%ld)\t", files->d_name, size1);    
                                if (!stat(temp, &fileStat)) {
                                    printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
                                    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                                }
                                printf("\t%s", modifiedTime);
                            }
                        }
                        else {
                            printf("%s\t(%ld)\t", files->d_name, size1);
                            if (!stat(temp, &fileStat)) {
                                printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
                                printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                            }
                            printf("\t%s", modifiedTime);
                        }
                    }
                }
                else if(strcmp(other, "d") == 0) {
                    if(isDir(temp) == 0) {
                        for(int i = 0; i < tempCount - count; i++)
                            printf("\t");
                        printf("%s\n", files->d_name);
                        printSubdirectories(temp, count, size, -1, substring, "d");
                    }
                    else {
                        // printf("%s\n", files->d_name);
                    }
                }
                else if(strcmp(other, "f") == 0) {
                    if(isDir(temp) == 0) {
                        printSubdirectories(temp, count, size, -1, substring, "f");
                    }
                    else {
                        for(int i = 0; i < tempCount - count; i++)
                            printf("\t");
                        printf("%s\n", files->d_name);
                    }
                }
                else if(strcmp(other, "ttt") == 0) {
                    for(int i = 0; i < tempCount - count + 1; i++)
                        printf("\t");
                    if(isDir(temp) == 0) {
                        printf("%s\n", files->d_name);
                        printSubdirectories(temp, count, size, -1, substring, "ttt");
                    }
                    else {
                        printf("%s\n", files->d_name);
                    }
                }
                else {
                    if(size == 0) {
                        if(isDir(temp) == 0) {
                            printf("%s\n", files->d_name);
                            printSubdirectories(temp, count, size, -1, substring, "");
                        }
                        else {
                            printf("%s\n", files->d_name);
                        }
                    }
                    else { // -s size
			//printf("size\n");
                        if(isDir(temp) == 0) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
                            printf("%s\n", files->d_name);
                            printSubdirectories(temp, count, size, -1, substring, "");
                        }
                        else {
                            struct stat st;
                            stat(temp, &st);
                            off_t size1 = st.st_size;
				//printf("file name: %s size: %d\n",files->d_name, size1);
                            if(size1 < size) {
                                for(int i = 0; i < tempCount - count; i++)
                                    printf("\t");
                                printf("%s\n", files->d_name);
                            }
                        }
                    }
                    
                }
            }
            else {
                char temp[100] = {};
                strcat(temp, dirname);
                strcat(temp, "/");
                strcat(temp, files->d_name);
                // calculate /
                int tempCount = 0;
                for(int i = 0; i < strlen(temp); i++) 
                    if(temp[i] == '/')
                        tempCount++;
                
                // print files and directory name
                if(size == 0) {
                    if(tempCount - count <= depth + 1)
                        if(strstr(files->d_name, substring) != NULL && isDir(temp) != 0) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
                            // printf("%s\n", files->d_name);
                            if(strcmp(other, "-S") == 0) {
                                struct stat st;
                                stat(temp, &st);
                                off_t size1 = st.st_size;
                                printf("%s\t(%ld)\t", files->d_name, size1);
                                if (!stat(temp, &fileStat)) {
                                    printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
                                    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                                }
                                char* modifiedTime = getFileCreationTime(temp);
                                printf("\t%s", modifiedTime);
                            }
                            else {
                                printf("%s\n", files->d_name);
                            }
                        }
                }
                else {
                    struct stat st;
                    stat(temp, &st);
                    off_t size1 = st.st_size;
                    if(size1 < size && strstr(files->d_name, substring) != NULL) {
                        if(tempCount - count <= depth + 1) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
                            if(strcmp(other, "-S") == 0) {
                                struct stat st;
                                stat(temp, &st);
                                off_t size1 = st.st_size;
                                printf("%s\t(%ld)\t", files->d_name, size1);
                                if (!stat(temp, &fileStat)) {
                                    printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
                                    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                                }
                                char* modifiedTime = getFileCreationTime(temp);
                                printf("\t%s", modifiedTime);
                            }
                            else {
                                printf("%s\n", files->d_name);
                            }
                        }
                    }
                }
                if(isDir(temp) == 0) {
                    // printf("%d\t%d\n", tempCount - count, depth);
                    if(tempCount - count <= depth + 1) {
                        for(int i = 0; i < tempCount - count; i++)
                            printf("\t");
                        printf("%s\n", files->d_name);
                        printSubdirectories(temp, count, size, depth, substring, other);
                    }
                }   
            }
            // }
        }
    }
    closedir(dir);
}

char** getArgs(char *str,char* filename){
    char ** result  = NULL;
    char *  temStr  = strtok (str, " \n");
    int num = 0;
    while (temStr) {
        result = realloc (result, sizeof (char*) * ++num);
        if (result == NULL)
            exit (-1);
        result[num-1] = temStr;
        temStr = strtok (NULL, " \n");
    }
    result = realloc (result, sizeof (char*) * (num+1));
    char *fstring = strtok(filename," ");
    while(fstring !=NULL)
    {
	result[num++] = fstring;
	fstring = strtok(NULL, " ");
    }  
    result[num]  = 0;
    return result;
}


//cmdfunc(command,files->d_name);
void cmdfunc(char* command, char* filename,int count)
{
		//printf("%d\n",count);
		    pid_t pid=fork();
		    if (pid==0) { // If it is a child process
                    
                    char **argv = getArgs(command, filename);
                    command = argv[0];
                    execvp(command,argv);
                    exit(-1); 
                }
                else { // Parent process
		    waitpid(pid,0,0);
                }
                    return;	
}

void printdirectories(char* dirname, int count, int size, int depth, char* substring, char* other, char* command) {
    struct dirent *files;
    struct stat fileStat;
    DIR *dir = opendir(dirname);
    char fullpath[1024];
    strcpy(cmdid, other);
	
    if (dir == NULL){
        printf("Directory cannot be opened!" );
        return;
    }
    char tempList[100][100];
    while ((files = readdir(dir)) != NULL) { 
        if(strcmp(files->d_name, ".") && strcmp(files->d_name, "..")) {
	    if(depth == -1) {
		char temp[100] = {};
                strcat(temp, dirname);
                strcat(temp, "/");
                strcat(temp, files->d_name);
                // calculate /
                int tempCount = 0;
                for(int i = 0; i < sizeof temp; i++) 
                    if(temp[i] == '/')
                        tempCount++;
		if(size == 0)
                    for(int i = 0; i < tempCount - count; i++)
                        printf("\t");
                // print files and directory name
                if(strcmp(other, "-e") == 0 || (strcmp(other, "-E")==0)){ 
                    if(size == 0) {
                        if(isDir(temp) == 0) {
                            printf("%s\n", files->d_name);
                            printdirectories(temp, count, size, -1, substring, "", "");
                        }
                        else {
                            printf("%s\n", files->d_name);
                        }
                    }
                    else { // -s size
                        if(isDir(temp) == 0) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
			    //printf("%s\n", files->d_name);
                            printdirectories(temp, count, size, -1, substring, other, command);
			    if(strcmp(other, "-E") == 0)
			    {
				strcpy(fullpath, dirname);
				strcat(fullpath ,"/");
				strcat(fullpath , files->d_name);
				char* tempfile = fullpath;
				strcat(temp1, tempfile);
				strcat(temp1, " ");
			    }		   
                        }
                        else {
                            struct stat st;
                            stat(temp, &st);
                            off_t size1 = st.st_size;
                            if(size1 < size) {
                                for(int i = 0; i < tempCount - count; i++)
                                    printf("\t");
				strcpy(fullpath, dirname);
				strcat(fullpath ,"/");
				strcat(fullpath , files->d_name);
				//printf("%s\n", files->d_name);
				char* tempfile = fullpath;
				//printf("current file: %s\n", tempfile);
				strcat(temp1, tempfile);
				strcat(temp1, " ");
				if(strcmp(other, "-e") == 0)
				{
					cmdfunc(command,fullpath,1);
				}

                            }
                        }
                    }
                    
                }
            }
            else {
                char temp[100] = {};
		strcat(temp, dirname);
                strcat(temp, "/");
                strcat(temp, files->d_name);
                // calculate /
                int tempCount = 0;
                for(int i = 0; i < strlen(temp); i++) 
                    if(temp[i] == '/')
                        tempCount++;
                
                // print files and directory name
                if(size == 0) {
                    if(tempCount - count <= depth + 1)
                        if(strstr(files->d_name, substring) != NULL && isDir(temp)!=0 ) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
		            strcpy(fullpath, dirname);
			    strcat(fullpath ,"/");
			    //printf("command: %s", command);
			    strcat(fullpath , files->d_name);
			    char* tempfile = fullpath;
			    //printf("current file: %s\n", tempfile);
			    strcat(temp1, tempfile);
			    strcat(temp1, " ");
			    //printf("Command here 1\n");
			    if(strcmp(other, "-e") == 0)
				{
					cmdfunc(command,fullpath,2);
				}

                            }
                 }
                 else {
                    struct stat st;
                    stat(temp, &st);
                    off_t size1 = st.st_size;
                    if(size1 < size && strstr(files->d_name, substring) != NULL) {
                        if(tempCount - count <= depth + 1) {
                            for(int i = 0; i < tempCount - count; i++)
                                printf("\t");
                                //printf("%s\n", files->d_name);
				strcpy(fullpath, dirname);
				strcat(fullpath ,"/");
				//printf("command: %s", command);
				strcat(fullpath , files->d_name);
				char* tempfile = fullpath;
				//printf("current file: %s\n", tempfile);
				strcat(temp1, tempfile);
				strcat(temp1, " ");
				if(strcmp(other, "-e") == 0)
				{
					cmdfunc(command,fullpath,3);
				}
                            }
                        }
                    }
                if(isDir(temp) == 0) {
                    // printf("%d\t%d\n", tempCount - count, depth);
                    if(tempCount - count <= depth + 1) {
                        for(int i = 0; i < tempCount - count; i++)
                            printf("\t");
                        //printf("%s\n", files->d_name);

			    if(strcmp(other, "-E") == 0 && strstr(files->d_name, substring))
			    {
				strcpy(fullpath, dirname);
				strcat(fullpath ,"/");
				strcat(fullpath , files->d_name);
				char* tempfile = fullpath;
				if (strstr(temp1,tempfile) == NULL)
				{
					strcat(temp1, tempfile);
					strcat(temp1, " ");
				}
			    }
			printdirectories(temp, count, size, depth, substring, other, command);
                    }
                }   
            }
            // }

        }
		
    }
	
    closedir(dir);
}


char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

//Main function
int main(int argc, char *argv[]) {
    char* command = NULL;
    if(argc == 1) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
              printf("%s\n", cwd);
            int tempCount = 0;
            for(int i = 0; i < strlen(cwd); i++) 
                if(cwd[i] == '/')
                    tempCount++;
            printSubdirectories(cwd, tempCount, 0, -1, "", "");
        } else {
            perror("getcwd() error");
            return 1;
        }
    }
    else if(argc == 2) {
        if(strcmp(argv[1], "-S") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printSubdirectories(cwd, tempCount, 0, -1, "", "-S");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
        else {
            char* tempDir = argv[1];
            if(strstr(tempDir, "../") != NULL) {
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printf("%s\n", tempDir);
                    printSubdirectories(tempDir, tempCount, 0, -1, "",  "ttt");
                } else {
                    perror("getcwd() error");
                    return 1;
                }  
            }
            else {
                char* tempDir = argv[1];
                printf("%s\n", tempDir);
                int tempCount = 0;
                for(int i = 0; i < strlen(tempDir); i++) 
                    if(tempDir[i] == '/')
                        tempCount++;
                printSubdirectories(tempDir, tempCount, 0, -1, "", "");
            }
        }
    }
    else if(argc == 3) {
        if(strcmp(argv[1], "-S") == 0) {
            char* tempDir = argv[2];
            if(strstr(tempDir, "../") != NULL) {
                char* substr = tempDir + 3;
                printf("%s\n", substr);
                printSubdirectories(substr, 0, 0, -1, "", "-S");
            }
            else {
                char* tempDir = argv[2];
                printf("%s\n", tempDir);
                int tempCount = 0;
                for(int i = 0; i < strlen(tempDir); i++) 
                    if(tempDir[i] == '/')
                        tempCount++;
                printSubdirectories(tempDir, tempCount, 0, -1, "", "-S");
            }
        }
        else if(strcmp(argv[1], "-s") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                int size;
                sscanf(argv[2], "%d", &size);
                printSubdirectories(cwd, tempCount, size, -1, "", "");
            } else {
                perror("getcwd() error");
                return 1;
            }
            
        }
        else if(strcmp(argv[1], "-t") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                char* tempFile = argv[2];
                printSubdirectories(cwd, tempCount, -1, -1, "", tempFile);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
    }
    else if(argc == 4) {
        if(strcmp(argv[1], "-s") == 0) {
            if(strcmp(argv[3], "-S") == 0) {
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
		    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    int size;
                    sscanf(argv[2], "%d", &size);
                    printSubdirectories(cwd, tempCount, size, -1, "", "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
            else {
                int size;
                sscanf(argv[2], "%d", &size);
                char* tempDir = argv[3];
                if(strstr(tempDir, "../") != NULL) {
                    char* substr = tempDir + 3;
                    printf("%s\n", substr);
                    printSubdirectories(substr, 0, size, -1, "", "");
                }
                else {
                    char* tempDir = argv[3];
                    printf("%s\n", tempDir);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(tempDir); i++) 
                        if(tempDir[i] == '/')
                            tempCount++;
                    printSubdirectories(tempDir, tempCount, size, -1, "", "");
                }
            }
        }
        else if(strcmp(argv[1], "-f") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                // int size;
                // sscanf(argv[2], "%d", &size);
                printSubdirectories(cwd, tempCount, 0, depth, substring, "");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
        else if(strcmp(argv[1], "-S") == 0) {
            if(strcmp(argv[2], "-s") == 0) {
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    int size;
                    sscanf(argv[3], "%d", &size);
                    printSubdirectories(cwd, tempCount, size, -1, "", "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
        }
    }
    else if(argc == 5) {
        if(strcmp(argv[1], "-S") == 0) {
            char* substring = argv[3];
            int depth;
            sscanf(argv[4], "%d", &depth);
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                // int size;
                // sscanf(argv[2], "%d", &size);
                printSubdirectories(cwd, tempCount, 0, depth, substring, "-S");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
        else if(strcmp(argv[1], "-s") == 0) {
	    if(strcmp(argv[3], "-e")==0) {
		printf("%s\n",argv[4]);
             char cwd[PATH_MAX];
             if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                int size;
                sscanf(argv[2], "%d", &size);
		command = argv[4];
                printdirectories(cwd, tempCount, size, -1, "", "-e", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
          } 
	    else if(strcmp(argv[3], "-E")==0) {
		printf("%s\n",argv[4]);
             char cwd[PATH_MAX];
             if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                int size;
                sscanf(argv[2], "%d", &size);
 		command = argv[4];
                printdirectories(cwd, tempCount, size, -1, "", "-E", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
          } 
	}
        else {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                // int size;
                // sscanf(argv[2], "%d", &size);
                printSubdirectories(cwd, tempCount, 0, depth, substring, "-S");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
    }
    else if(argc == 6) {
        if(strcmp(argv[1], "-f") == 0 && strcmp(argv[4], "-s") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            int size;
            sscanf(argv[5], "%d", &size);
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printSubdirectories(cwd, tempCount, size, depth, substring, "");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	else if(strcmp(argv[1], "-f") == 0 && strcmp(argv[4], "-e") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            char cwd[PATH_MAX];
	    command = argv[5];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, 0, depth, substring,  "-e", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	else if(strcmp(argv[1], "-f") == 0 && strcmp(argv[4], "-E") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            char cwd[PATH_MAX];
	    command = argv[5];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, 0, depth, substring,  "-E", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }

        else if(strcmp(argv[3], "-f") == 0 && strcmp(argv[1], "-s") == 0) {
            char* substring = argv[4];
            int depth;
            sscanf(argv[5], "%d", &depth);
            int size;
            sscanf(argv[2], "%d", &size);
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printSubdirectories(cwd, tempCount, size, depth, substring, "");
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
    }
    else if(argc == 7) {
        if(strcmp(argv[1], "-S") == 0) {
            if(strcmp(argv[2], "-f") == 0 && strcmp(argv[5], "-s") == 0) {
                char* substring = argv[3];
                int depth;
                sscanf(argv[4], "%d", &depth);
                int size;
                sscanf(argv[6], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
            else if(strcmp(argv[4], "-f") == 0 && strcmp(argv[2], "-s") == 0) {
                char* substring = argv[5];
                int depth;
                sscanf(argv[6], "%d", &depth);
                int size;
                sscanf(argv[3], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
        }
        if(strcmp(argv[1], "-f") == 0) {
            if(strcmp(argv[4], "-s") == 0) {
                char* substring = argv[2];
                int depth;
                sscanf(argv[3], "%d", &depth);
                int size;
                sscanf(argv[5], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
            else if(strcmp(argv[4], "-S") == 0) {
                char* substring = argv[2];
                int depth;
                sscanf(argv[3], "%d", &depth);
                int size;
                sscanf(argv[6], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
        }
        if(strcmp(argv[1], "-s") == 0) {
            if(strcmp(argv[3], "-S") == 0) {
                char* substring = argv[5];
                int depth;
                sscanf(argv[6], "%d", &depth);
                int size;
                sscanf(argv[2], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
            else if(strcmp(argv[3], "-f") == 0) {
                char* substring = argv[4];
                int depth;
                sscanf(argv[5], "%d", &depth);
                int size;
                sscanf(argv[2], "%d", &size);
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    int tempCount = 0;
                    for(int i = 0; i < strlen(cwd); i++) 
                        if(cwd[i] == '/')
                            tempCount++;
                    printSubdirectories(cwd, tempCount, size, depth, substring, "-S");
                } else {
                    perror("getcwd() error");
                    return 1;
                }
            }
        }
	
    }
    else if(argc == 8) {
	if(strcmp(argv[6], "-e") == 0) {
	    if(strcmp(argv[3], "-f") == 0 && strcmp(argv[1], "-s") == 0) {
            char* substring = argv[4];
            int depth;
            sscanf(argv[5], "%d", &depth);
            int size;
            sscanf(argv[2], "%d", &size);
            char cwd[PATH_MAX];
	    command = argv[7];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, size, depth, substring, "-e", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	    if(strcmp(argv[1], "-f") == 0 && strcmp(argv[4], "-s") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            int size;
            sscanf(argv[5], "%d", &size);
            char cwd[PATH_MAX];
	    command = argv[7];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, size, depth, substring, "-e", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	}
	if(strcmp(argv[6], "-E") == 0) {
	    if(strcmp(argv[3], "-f") == 0 && strcmp(argv[1], "-s") == 0) {
            char* substring = argv[4];
            int depth;
            sscanf(argv[5], "%d", &depth);
            int size;
            sscanf(argv[2], "%d", &size);
            char cwd[PATH_MAX];
	    command = argv[7];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, size, depth, substring, "-E", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	    if(strcmp(argv[1], "-f") == 0 && strcmp(argv[4], "-s") == 0) {
            char* substring = argv[2];
            int depth;
            sscanf(argv[3], "%d", &depth);
            int size;
            sscanf(argv[5], "%d", &size);
            char cwd[PATH_MAX];
	    command = argv[7];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                int tempCount = 0;
                for(int i = 0; i < strlen(cwd); i++) 
                    if(cwd[i] == '/')
                        tempCount++;
                printdirectories(cwd, tempCount, size, depth, substring, "-E", command);
            } else {
                perror("getcwd() error");
                return 1;
            }
        }
	}
	}
    // printf("%d", argc);
	//printf("id: %s",cmdid); 
	if(strcmp(cmdid, "-E") == 0)
	{
		cmdfunc(command,temp1,4);
	}
    return 0;
}