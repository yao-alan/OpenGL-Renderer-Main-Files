#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>

#include "FileTools.h"

void readFile(char** dest, char* src) 
{
	FILE* fin;

	fin = fopen(src, "r");

	fseek(fin, 0, SEEK_END);
	int len = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char* memStart = (char*)calloc(sizeof(char), len);
	*dest = memStart;
	fread(*dest, len, sizeof(char), fin);

	free(memStart);
	fclose(fin);
}
