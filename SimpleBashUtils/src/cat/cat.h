#ifndef CAT_H
#define CAT_H

#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int lines;
  int nonblank;
  int squeeze;
  int dollar;
  int nonprint;
  int tab;
} options;

void parsing(int argc, char *argv[], options *opts);
void printing(char *buffer, ssize_t read, options *opts);
char nonprinting(char ch);
void reading(int argc, char *argv[], options *opts);

#endif