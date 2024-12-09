#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


void help() {
    	
    	printf("Options are available in ls:\n");
    	printf("  -h	Show help\n");
    	printf("  -c    Count files\n");
    	printf("  -s    Sort files alphabetically\n");
    	
}


int comp_name(const void *a, const void *b) {
    	const char **nameA = (const char **)a;
    	const char **nameB = (const char **)b;
    	return strcmp(*nameA, *nameB);
}


int file_count(const char *path) {
    	struct dirent *entry;
    	DIR *dir=opendir(path);
    	int count =0;

    	if(dir == NULL) {
        	perror(" Error : Directory can't open ");
        	return -1;
    	}

    	while((entry=readdir(dir)) !=NULL){
        	if(entry->d_name[0] !='.'){  
            		count++;
        	}
    	}

    	closedir(dir);
    	return count;
}


void file_list(const char *path){
    	struct dirent *entry;
    	DIR *dir = opendir(path);
    	char **name_file =NULL;
    	int file_count=0, cap=10;

    	if(dir == NULL){
        	perror("Error : Directory can't open");
        	return;
    	}

    	name_file =(char **)malloc(cap* sizeof(char *));
    	if(name_file ==NULL){
        	perror(" Allocation of memory failed ");
        	closedir(dir);
        	return;
    	}
    	
    	
    	while((entry =readdir(dir))!= NULL){
        	if(entry->d_name[0]!= '.'){  
            		if(file_count >= cap) {
                		cap *= 2;
                		name_file = (char **)realloc(name_file, cap * sizeof(char *));
                		if(name_file == NULL){
                    			printf("Allocation of memory failed ");
                    			closedir(dir);
                    			return;
                		}
            		}

            		name_file[file_count] =strdup(entry->d_name);
            		if(name_file[file_count] ==NULL){
                		printf("Allocation of memory failed ");
                		closedir(dir);
                		return;
            		}
            		file_count++;
        	}
    	}

    	closedir(dir);

    	qsort(name_file,file_count,sizeof(char *),comp_name);

    	for(int i =0; i<file_count;i++){
        	printf("%s\n",name_file[i]);
        	free(name_file[i]);
    	}

    	free(name_file);
}

int main(int argc, char *argv[]){
    	int count = 0;  
    	int slist = 0;   
    	int help_flag = 0;
    	const char *path = "."; 
    	
    	for(int i = 1; i < argc; i++){
        	if(argv[i][0] == '-'){
            		switch(argv[i][1]){
                		case 'c':
                    			count = 1;
                    			break;
                		case 's':
                    			slist = 1;
                    			break;
               
                		case 'h':
                    			help_flag = 1;
                    			break;
                		default:
                    			printf("%s is unknow option\n", argv[i]);
                    			printf("Use 'ls -h' for list of available commands.\n ");
                    			return 1;
            		}
        	} 
        	else{
            	path = argv[i];  
        	}
    	}

    	if(help_flag){
        	help();
        	return 0;
    	}

    	if(slist){
        	file_list(path);
    	} 
    	else{
        	file_list(path);  
    	}

    	if(count){
        	int cnt_file = file_count(path);
        	printf("File Count: %d\n", cnt_file);
    	}

    	return 0;
}

