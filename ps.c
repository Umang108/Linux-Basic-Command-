#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct ProInfo{
    	int pid;
    	char name[256];
    	char state;
};

void help(){
    	
    	printf("Options are available in ps:\n");
    	printf("  -p	Show only process IDs\n");
    	printf("  -n 	Show only process names\n");
    	printf("  -s	Show process state\n");
    	printf("  -h    Display this help message\n");
}


struct ProInfo get_info(int pid){
    	struct ProInfo pinfo;
    	pinfo.pid = pid;

    	char filename[256];
    	snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
    	FILE *fp =fopen(filename, "r");
    	if(fp==NULL){
        	perror("Error : Can't open file");
        	pinfo.state = '?';
        	strcpy(pinfo.name,"unknown");
        	return pinfo;
    	}

    	char line[256];
    	while(fgets(line, sizeof(line),fp)){
        	if(strncmp(line, "Name:", 5)==0){
            		sscanf(line, "Name:\t%s", pinfo.name);
        	} 
        	else if(strncmp(line, "State:", 6)==0){
            		sscanf(line, "State:\t%c", &pinfo.state);
            		break;
        	}
    	}

    	fclose(fp);
    	return pinfo;
}
int is_number(const char *str){
    	while(*str) {
        	if(*str <'0' || *str >'9') return 0;
        	str++;
    	}
    	return 1;
}
void pro_list(int p_id, int p_name, int p_state ){
    	struct dirent *entry;
    	DIR *proc_dir;
    	proc_dir= opendir("/proc");
    	if (proc_dir == NULL){
        	perror("Error : Can't open dir");
        	exit(EXIT_FAILURE);
    	}

    	printf("PID\tName\t\t\t\t\tState\n");  
    	while((entry = readdir(proc_dir)) != NULL){
        	if(is_number(entry->d_name)){
            		int pid = atoi(entry->d_name);
            		struct ProInfo pinfo = get_info(pid);

            		if(p_id){
                		printf("%d\n", pinfo.pid);  // Only show PID
            		} 
            		else if(p_name){
                		printf("%-30s\n", pinfo.name);  // Only show Name
            		} 
            		else{
                		if (p_state){
                    			printf("%c\n", pinfo.state);  // Only State 
                	} 
                		else{
                    		printf("%-5d\t%-30s\t\t%c\n", pinfo.pid, pinfo.name, pinfo.state);  // PID, Name, State 
                		}
            		}
        	}
    	}

    	closedir(proc_dir);
}

int main(int argc, char *argv[]){
   	int p_id = 0;
    	int p_name= 0;
    	int p_state = 0;

    	if(argc == 1){
      	  
        	pro_list(p_id, p_name, p_state);
        	return 0;
    	}

    	for(int i = 1; i < argc; i++){
        	if(strcmp(argv[i], "-h") == 0){
            		help();
            		return 0;
        	} 
        	else if(strcmp(argv[i], "-p") == 0){
            		p_id = 1;
        	} 
        	else if(strcmp(argv[i], "-n") == 0){
            		p_name = 1;
        	} 
        	else if(strcmp(argv[i], "-s") == 0){
            		p_state = 1;
        	} 
        	else{
            		fprintf(stderr, "%s is unknow option\n", argv[i]);
            		printf("Use 'help' for list of available commands.\n");
            		return 1;
        	}
    	}

    	pro_list(p_id,p_name,p_state);
    	return 0;
}

