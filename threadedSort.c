#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include <limits.h>
#include<errno.h>
#include<fcntl.h>
int fileSize(const char* name);
int isRegular(const char * path);
void list_dir(const char * dir_name,char * myfifo);
struct Node{
     int a;
    char* b;
    struct Node *next;
};
struct Node *head;
void insert( int x,char* y){
    struct Node *temp;
    temp = (struct Node*)malloc(sizeof(struct Node));
    temp->a = x;
    temp->b = y;
    temp->next = head;
    head= temp;
}
void print(){
    struct Node *temp;
    temp= head;
    while(temp!=NULL){
        printf("%d\t %s\n",temp->a,temp->b);
        temp=temp->next;
    }
}
void insertionSort(struct Node *head) {
    if(head == NULL || head->next == NULL) {
       //        printf("Fail ");
            return;
    }
    struct Node *t1 = (head)->next;
    while(t1 != NULL) {
	   char* name = t1->b;
        int fSize = t1->a;
        int found = 0;
        struct Node *t2 = head;
        while(t2 != t1) {
            if(t2->a > t1->a && found == 0) {
                fSize = t2->a;
		name = t2->b;
                t2->a = t1->a;
		t2->b = t1->b;
                found = 1;
                t2 = t2->next;
            } else {
                if(found == 1) {
                    char *tmp = name;
		    int temp = fSize;
                    fSize = t2->a;
                    name = t2->b;
		    t2->a = temp;
		    t2->b = tmp;

                }
                t2 = t2->next;
            }
      }
       t2->a = fSize;
       t2->b = name;
       t1 = t1->next;
    }
}


int main(int argc, char *argv[]){
	char * myfifo = argv[2];
	pid_t pid;
	int fd; 

	 if(mkfifo(myfifo, S_IRWXU | S_IWGRP | S_IWOTH) == -1){
                perror("Server failed to creat a FIFO");
                return 1;
        }
	  pid = fork();
        if(pid==0){
    	    list_dir(argv[1],argv[2]);
        }else{
        if((fd = open(myfifo, O_RDONLY)) ==-1){
                perror("Failed to open FIFO");
                return 1;
        }
        int size;
       int len;
	char c;
	int n, i, x;
	char *y = malloc(sizeof(char*));
	struct Node *temp = head;

	while(read(fd, &len, sizeof(int))!=0){
	read(fd,&size,sizeof(int));
	char * buf = (char*) malloc(sizeof(char)*len);
	read(fd, buf, len);
	insert(size,buf);
	}
	insertionSort(head);
	print();
	close(fd);
        unlink(myfifo);

        return 0;
	}

	
}
int isRegular(const char *path)
{
    struct stat path_stat;
    lstat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}
void list_dir (const char * name,char *myfifo)
{
	int fd;
	DIR *dir;
        struct dirent *entry;
 if((fd = open(myfifo, O_WRONLY)) == -1){
                perror("Failed to open FIFO");
        }

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
           char path[1024];
           snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
	   list_dir(path,myfifo);
        } else {
	
	const char * dash = "/";
	 char * dirName = concat(name, dash);
	 char * fName = malloc(sizeof(fName));
	 fName = concat(dirName, entry->d_name);
	 int n; 
	 
	if(isRegular(fName)){
	int size = fileSize(fName);
		n = strlen(fName);
		  if((write(fd, &n, sizeof(int)) == -1)){
                        perror("Writing failed1");
                }

		if((write(fd, &size, sizeof(int)) == -1)){
			perror("Writing failed1");
		}
		if((write(fd,fName , n) == -1)){
			perror("Writing failed");

		}
	}

  }
  }
   closedir(dir);

}
 int fileSize(const char* name){
    FILE* fp = fopen(name, "r");
    fseek(fp, 0L, SEEK_END);
     int res = ftell(fp);
    fseek(fp,0L,SEEK_SET);
    fclose(fp);

    return res;
}

