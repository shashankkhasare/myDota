#ifndef PARSER_H
#define PARSER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MANY_PTS	0
#define SINGLE_PT	1

char **newstrtok(int num, char *delim){
	char **ans;
	ans = (char **) malloc(sizeof(char *)*num);
	int i = 0;
	for(i = 0; i<num; i++)
		ans[i] = (char *) malloc(sizeof(char)*10);
		
	for(i = 0; i<num; i++)
		strcpy(ans[i], strtok(NULL, delim));

	return ans;
}
void freestrtok(char **lines, int num){
	int i = 0;
	for(i = 0; i<num; i++)
		free(lines[i]);
	free(lines);	
}

typedef struct line{
	char **line;
	int capacity;
}line_t;

typedef struct para_t{
	int capacity;
	line_t *lines;
}para_t;

para_t read_paragraph(int num, FILE *fp, int type, int size, char *delim, char *current_line){
	para_t para;
	para.capacity = num;
	para.lines = (line_t *) malloc(num*sizeof(line_t));

	char **line, str[200];
	int how_many = 1, i, j;

	strtok(current_line, delim);
	for(j = 0; j < num; j++){
		if(type == MANY_PTS){
			line = newstrtok(1, delim);
			how_many = atoi(line[0]);
			freestrtok(line, 1);
		}

		line = newstrtok(how_many*size, delim);
		para.lines[j].capacity = how_many*size;
		para.lines[j].line = (char **) malloc(sizeof(char *)*how_many*size);
		for(i = 0; i<how_many*size; i++){
			para.lines[j].line[i] = line[i];
		}
		
		if(j < num-1){
			fgets(str, 200, fp);
			strtok(str, delim);
		}
	}
	
	return para;
}

void print_paragraph(para_t *p){
	int i, j;
	for(i = 0; i < p->capacity; i++){
		for(j = 0; j < p->lines[i].capacity; j++)
			printf("%s ", p->lines[i].line[j]);
		printf("\n");	
	}
}
void free_para(para_t *p){
	int i, j;
	for(i = 0; i < p->capacity; i++)
		for(j = 0; j < p->lines[i].capacity; j++)
			free(p->lines[i].line[j]);		
			
}
#endif
