/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */ 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_NUM 15
#define CURRENT_MAX 150
#define INPUT_MAX 50

int main (int argc, char ** argv ) {

	/* manymany decls */
	char (*result)[CURRENT_MAX];
	char inp[INPUT_MAX];
	char * end;
	FILE * tty;
	int num=DEFAULT_NUM;
	int max=0;
	int i;
	int selection;

	/* Process command line arguments
	*  we currently only support -##
	*  FIXME: should have usage message.
	*/
	for(i=1; i < argc; i++) {
		if(argv[i][0]=='-'){
			num=atoi(argv[i])*-1;
			if (num==0) {
				fprintf(stderr,"Invalid Input: %s", argv[i]);
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Allocate enough space for the number of list
 	   items we're going to be displaying */
	result = malloc(num * sizeof(*result));

	/* Open the terminal for input/output so we don't
	   mess with stdin/stdout */
	tty = fopen("/dev/tty","r+");
	if(tty == NULL || errno == EINVAL) {
		perror("fopen: /dev/tty");
		exit(EXIT_FAILURE);
	}

	/* Print out all lines from STDIN */
	for(i=0; i < num; i++) {
		fgets(result[i],CURRENT_MAX,stdin);
		if(feof(stdin) != 0) {
			break;
		}
		max++;
		fprintf(tty, "%d: %s",i,result[i]);
	}

	/* Prompt */
	fprintf(tty, "Selection: ");

	/* Get one line of input from the user and put it in inp */
	fgets(inp,INPUT_MAX,tty);
	fclose(tty);

	/* Try to convert user input to int */
	selection=strtol(inp,0,10);
	if(errno == EINVAL) {
		/* Not an int */
		/* Commands Go Here */
		/* The 'a' command outputs every line*/
		for(i=0;i<max; i++){
			printf("%s",result[i]);
		}
	} else {
		/* Selection is the index the user chose
		   check if the index is valid. */
		if(selection < max && selection >= 0) {
			/* chomp */
			end = strchr(result[selection],'\n');
			if(end != 0) {
				*end='\0';
			}
			/* print selected item to STDOUT */
			printf(result[selection]);
		} else {
			/* User selected a bad index */
			fprintf(stderr, "There is no item with the index %d\n", selection);
		}
	}

	/* Boilerplate */
	return 0;
}
