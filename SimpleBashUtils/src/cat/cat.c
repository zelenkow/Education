#include "cat.h"

int main(int argc, char *argv[]) {
  options opts = {0};
  parsing(argc, argv, &opts);
  reading(argc, argv, &opts);
  return 0;
}

void parsing(int argc, char *argv[], options *opts) {
  if (argc < 2) {
    printf("Not options\n");
    exit(1);
  }
  int opt;
  struct option long_opts[] = {{"number-nonblank", no_argument, NULL, 'b'},
                               {"number", no_argument, NULL, 'n'},
                               {"squeeze-blank", no_argument, NULL, 's'},
                               {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "bnsEvTte", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'b':
        opts->nonblank = 1;
        break;
      case 'n':
        opts->lines = 1;
        break;
      case 's':
        opts->squeeze = 1;
        break;
      case 'E':
        opts->dollar = 1;
        break;
      case 'v':
        opts->nonprint = 1;
        break;
      case 'T':
        opts->tab = 1;
        break;
      case 't':
        opts->tab = 1;
        opts->nonprint = 1;
        break;
      case 'e':
        opts->dollar = 1;
        opts->nonprint = 1;
        break;
    }
  }
}

void reading(int argc, char *argv[], options *opts) {
  int nonblank_line = 1;
  int every_line = 1;
  int last_was_blank = 0;

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      printf("Error file\n");
      exit(1);
    }

    char *buffer = NULL;
    size_t size = 0;
    ssize_t read;

    while ((read = getline(&buffer, &size, file)) != -1) {
      int is_blank = (buffer[0] == '\n');
      if (opts->squeeze && is_blank && last_was_blank) {
        continue;
      }
      if (opts->nonblank && buffer[0] != '\n') {
        printf("%6d\t", nonblank_line++);
      }
      if (opts->lines && !opts->nonblank) {
        printf("%6d\t", every_line++);
      }
      last_was_blank = is_blank;
      printing(buffer, read, opts);
    }
    free(buffer);
    fclose(file);
  }
}

void printing(char *buffer, ssize_t read, options *opts) {
  for (int i = 0; i < read; i++) {
    int tabik = 0;
    if (opts->tab && buffer[i] == '\t') {
      printf("^I");
      tabik = 1;
    }
    if (opts->dollar && buffer[i] == '\n') {
      putchar('$');
    }
    if (opts->nonprint && buffer[i] != '\t' && buffer[i] != '\n') {
      buffer[i] = nonprinting(buffer[i]);
    }
    if (!tabik) {
      putchar(buffer[i]);
    }
  }
}

char nonprinting(char ch) {
  if (ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}