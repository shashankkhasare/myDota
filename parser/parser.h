#ifndef PARSER_H
#define PARSER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MANY_PTS	0
#define SINGLE_PT	1
char **newstrtok(int num, char *delim);
void freestrtok(char **lines, int num);

typedef struct line{
	char **line;
	int capacity;
}line_t;

typedef struct para_t{
	int capacity;
	line_t *lines;
}para_t;

para_t read_paragraph(int num, FILE *fp, int type, int size, char *delim, char *current_line);
void print_paragraph(para_t *p);
void free_para(para_t *p);

#endif 
