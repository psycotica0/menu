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

/* This function takes the newline off a string, if it exists */
void chomp (char * inp) {
	char * end;
	end = strchr(inp,'\n');
	if(end != 0) {
		*end='\0';
	}
}
int main (int argc, char ** argv ) {

	/* manymany decls */
	char (*result)[CURRENT_MAX];
	char inp[INPUT_MAX];
	char * end;
	char * iterator;
	/* This variable is used to hold the character that will start a line*/
	/* Used in the ',' causing newline and '+' causing space case */
	char lineStarter=0;
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

	/* Initialize the iterator */
	iterator=inp;
	/* This is what I seem to get on a blank line */
	if(*iterator != (char)10) {
		while(*iterator != '\0') {
			/* Try to convert user input to int */
			selection=strtol(iterator,&end,10);

			if(end == iterator) {
				/* Not an int */

				/* Consume White Space */
				while(*end == ' ') end++;

				/* Commands Go Here */
				if(*end == '\0' || *end == (char)10) {
					/* We're Done Here */
					break;
					/* I love the null command */
				} else if(*end == 'a' && lineStarter==0) {
					/* The 'a' command outputs every line*/
					/* 'a' is only valid if it's the first, thus the lineStarter check*/
					for(i=0;i<max; i++){
						printf("%s",result[i]);
					}
					/* After a, no more input is accepted */
					break;
				} else if (*end == ',') {
					/* The ',' command is one of those things that I love */
					/* At first it doesn't seem like a command, but really it is. */
					lineStarter='\n';
					/* Set iterator to the next char */
					iterator=end+1;
				} else {
					/* If I don't recognize it, end */
					fprintf(stderr,"Unrecognized Command %c\n", *end);
					break;
				}
			} else {
				/* Selection is the index the user chose
					 check if the index is valid. */
				if(selection < max && selection >= 0) {
					/* chomp */
					chomp(result[selection]);
					/* print selected item to STDOUT */
					if (lineStarter!=0) {
						putchar(lineStarter);
					}
					printf(result[selection]);
				} else {
					/* User selected a bad index */
					fprintf(stderr, "There is no item with the index %d\n", selection);
				}
				iterator=end;
			}
		}
	}

	/* Boilerplate */
	return 0;
}
