/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */ 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_NUM 15
#define CURRENT_MAX 150
#define INPUT_MAX 50

int delim='\n';	/*Delimiter for input/output items*/
char **lines;	/*Lines read from stdin*/
size_t nlines;	/*Number of lines in lines[]*/

/* This function slurps all the data from stdin, and stuffs it into lines[] */
void slurp (void) {
	size_t maxlines;
	char *readbuf;
	size_t maxread,nread;
	int c;

	nlines=maxlines=0;
	readbuf=malloc(maxread=128);
	if (readbuf == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	nread=0;

	while ((c=getchar()) != EOF) {
		if (c == delim) {
			/*End of line*/
			readbuf[nread++]='\0';
			if (nlines >= maxlines) {
				/*Grow lines[]*/
				maxread *= 2;
				lines = realloc(lines,maxread*sizeof *lines);
				if (!lines) {
					perror("realloc");
					exit(EXIT_FAILURE);
				}
			}
			lines[nlines++] = strdup(readbuf);
			nread=0;
		}
		else
		{
			readbuf[nread++] = c;
			if (nread >= maxread) {
				/* Grow after stuffing another character in.
				   This ensures there's room for the '\0'. */
				maxread *= 2;
				readbuf = realloc(readbuf,maxread);
				if (!readbuf) {
					perror("realloc");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	if (ferror(stdin)) {
		perror("<stdin>");
		exit(EXIT_FAILURE);
	}

	if (nread) {
		/* Last item read didn't end with a newline.  Don't lose it. */
		if (nlines >= maxlines) {
			/*Grow lines[]*/
			maxread *= 2;
			lines = realloc(lines,maxread*sizeof *lines);
			if (!lines) {
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		lines[nlines++] = strdup(readbuf);
	}

	free(readbuf);
}

/* This function takes in a string and prints it out, escaping spaces and quotes */
void printWithEscape (char * inp) {
	int i=0;
	int length= strlen(inp);
	for (;i<length;i++){
		if (inp[i]==' ' || inp[i]=='"' || inp[i]=='\'') {
			putchar('\\');
		}
		putchar(inp[i]);
	}
}

/* Print a usage message to stderr */
void usage() {
	fprintf(stderr, "This function is used as a filter in a pipe from stdin to stdout.\n\
 Options:\n\
  -n NUM Specifies the number of entries to display\n\
  -r     Raw output. Doesn't escape any characters\n\
  -0     Use NUL instead of newline as delimiter (implies -r)\n");
}

int main (int argc, char ** argv ) {

	/* manymany decls */
	char inp[INPUT_MAX];
	char * end;
	char * iterator;
	/* This variable is used to hold the character that will start a line*/
	/* Used in the ',' causing newline and '+' causing space case */
	char lineStarter=-1;
	FILE * tty;
	int num=DEFAULT_NUM;
	int i;
	int selection;
	char optFlag;
	int Escape=1;
	int need_newline=0;

	/* Process command line arguments */
	while ((optFlag=getopt(argc, argv,  "n:r0"))!=-1){
		switch(optFlag) {
			case 'n':
				/* Get the number to display */
				num=atoi(optarg);
				if (num==0) {
					fprintf(stderr,"Invalid Input: %s", argv[i]);
					exit(EXIT_FAILURE);
				}
				break;
			case '0':
				/* NUL instead of newline as delimiter */
				delim='\0';
				/* fall through - -0 implies -r */
			case 'r':
				/* Raw ouput */
				Escape=0;
				break;
			case '?':
			default:
				usage();
				return 0;
				break;
		}
	}

	/* Open the terminal for input/output so we don't
	   mess with stdin/stdout */
	tty = fopen("/dev/tty","r+");
	if(tty == NULL || errno == EINVAL) {
		perror("fopen: /dev/tty");
		exit(EXIT_FAILURE);
	}

	/* Slurp all input from stdin */
	slurp();

	if(nlines > num)
		nlines=num;

	/* Print out all lines from STDIN */
	for(i=0; i < nlines; i++) {
		fprintf(tty, "%d: %s\n",i,lines[i]);
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
				} else if(*end == 'a' && lineStarter==-1) {
					/* The 'a' command outputs every line*/
					/* 'a' is only valid if it's the first, thus the lineStarter check*/
					for(i=0;i<nlines; i++){
						if (Escape) {
							printWithEscape(lines[i]);
						} else {
							printf("%s",lines[i]);
						}
						putchar(delim);
					}
					/* After a, no more input is accepted */
					break;
				} else if (*end == ',') {
					/* The ',' command is one of those things that I love */
					/* At first it doesn't seem like a command, but really it is. */
					lineStarter=delim;
					/* Set iterator to the next char */
					iterator=end+1;
				} else if (*end == '+') {
					/* The '+' command I love too. See ',' command. */
					lineStarter=' ';
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
				if(selection < nlines && selection >= 0) {
					/* print selected item to STDOUT */
					if (lineStarter!=-1) {
						putchar(lineStarter);
					}
					if (Escape) {
						printWithEscape(lines[selection]);
					} else {
						printf(lines[selection]);
					}
					need_newline=1;
				} else {
					/* User selected a bad index */
					fprintf(stderr, "There is no item with the index %d\n", selection);
				}
				/*
				This is actually kind of a hack
				If I were to select 1+2, which I get is "1" and "+2", which is parsed as "2".
				So, the default behaviour will actaully be "+", but it's only accessed if you break the numbers up with something that gets consumed without changing the value of the number
				This mostly leaves '+'. Maybe '.', I'm not sure.
				*/
				lineStarter=' ';
				iterator=end;
			}
		}
	}
	if(need_newline)
		putchar(delim);

	/* Boilerplate */
	return 0;
}
