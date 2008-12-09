#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_NUM 15
#define DEFAULT_COMMAND "vim -p"

int main (int argc, char ** argv ) {
	char * command=DEFAULT_COMMAND;
	int num=DEFAULT_NUM;
	int i;
	for(i=1; i < argc; i++) {
		if(argv[i][0]=='-'){
			num=atoi(argv[i])*-1;
			if (num==0) {
				printf("Invalid Input: %s", argv[i]);
				exit(EXIT_FAILURE);
			}
		} else {
			command = argv[i];
		}
	}

	char result[num][150];
	int max=0;

	for(i=0;i < num; i++){
		fgets(result[i],150,stdin);
		max++;
		if( feof(stdin) != 0) {
			break;
		}
		printf("%d: %s",i,result[i]);
	}

	printf("Selection: ");

	FILE * secondIn = fopen("/dev/tty","r");
	if (secondIn == 0){
		exit(EXIT_FAILURE);
	}
	char inp[50];
	fgets(inp,50,secondIn);

	int selection=atoi(inp);
	if(selection==0){
	}

	return 0;
}
