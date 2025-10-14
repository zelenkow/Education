#ifndef GREP_H
#define GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char pattern[1024];
  int len_pattern;
  int file_count;
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
} options;

void parsing(int argc, char *argv[], options *opts);
void pattering(options *opts, char *pattern);
void reading(int argc, char *argv[], options *opts);
int compiling(char *buffer, char *filename, options *opts, int read,
              int line_count);
void printing(char *buffer, int n, options *opts, char *filename,
              int line_count);

#endif  // GREP_H