#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DEFAULT_NUM 15
#define DEFAULT_COMMAND "vim -p"
#define CURRENT_MAX 150
#define INPUT_MAX 50

int main (int argc, char ** argv ) {
	char * command=DEFAULT_COMMAND;
	int num=DEFAULT_NUM;
	int i;
	for(i=1; i < argc; i++) {
		if(argv[i][0]=='-'){
			num=atoi(argv[i])*-1;
			if (num==0) {
				fprintf(stderr,"Invalid Input: %s", argv[i]);
				exit(EXIT_FAILURE);
			}
		} else {
			command = argv[i];
		}
	}

	char result[num][CURRENT_MAX];
	int max=0;

	for(i=0;i < num; i++){
		fgets(result[i],CURRENT_MAX,stdin);
		if( feof(stdin) != 0) {
			break;
		}
		max++;
		printf("%d: %s",i,result[i]);
	}

	printf("Selection: ");

	FILE * secondIn = fopen("/dev/tty","r");
	if (secondIn == 0){
		exit(EXIT_FAILURE);
	}
	char inp[INPUT_MAX];
	fgets(inp,INPUT_MAX,secondIn);

	int selection=strtol(inp,0,10);
	if(errno==EINVAL) {
		/* Commands Go Here */
	} else {
		if(selection < max && selection >= 0) {
			/* Do Stuff */ 
			printf("You Picked: %s",result[selection]);
		} else {
			fprintf(stderr, "There is no item with the index %d\n", selection);
		}
	}

	fclose(secondIn);

	return 0;
}
