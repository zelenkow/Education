#include "grep.h"

int main(int argc, char *argv[]) {
  options opts = {0};

  parsing(argc, argv, &opts);
  reading(argc, argv, &opts);

  return 0;
}

void parsing(int argc, char *argv[], options *opts) {
  if (argc <= 2) {
    printf("Not options\n");
    exit(1);
  }
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhs")) != -1) {
    switch (opt) {
      case 'e':
        opts->e = 1;
        pattering(opts, optarg);
        break;
      case 'i':
        opts->i = REG_ICASE;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'c':
        opts->c = 1;
        break;
      case 'l':
        opts->l = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 'h':
        opts->h = 1;
        break;
      case 's':
        opts->s = 1;
        break;
    }
  }
  if (opts->len_pattern == 0) {
    pattering(opts, argv[optind]);
    optind++;
  }
  opts->file_count = argc - optind;
}

void pattering(options *opts, char *pattern) {
  if (opts->len_pattern + strlen(pattern) + 3 > 1024) {
    printf("Buffer is full\n");
    exit(1);
  }
  if (opts->len_pattern != 0) {
    strcat(opts->pattern + opts->len_pattern, "|");
    opts->len_pattern++;
  }
  opts->len_pattern +=
      sprintf(opts->pattern + opts->len_pattern, "(%s)", pattern);
}

void reading(int argc, char *argv[], options *opts) {
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      if (!opts->s) {
        printf("Error file\n");
      }
      exit(1);
    }
    char *buffer = NULL;
    size_t size = 0;
    int read = 0;
    int line_count = 1;
    int total_count_c = 0;

    while ((read = getline(&buffer, &size, file)) != -1) {
      int count_c = compiling(buffer, argv[i], opts, read, line_count);
      line_count++;
      total_count_c += count_c;
    }
    if (opts->l && total_count_c > 0) {
      printf("%s\n", argv[i]);
    }
    if (opts->c && opts->file_count > 1 && !opts->l && !opts->h) {
      printf("%s:%d\n", argv[i], total_count_c);
    } else if (opts->c && !opts->l) {
      printf("%d\n", total_count_c);
    }
    free(buffer);
    fclose(file);
  }
}

int compiling(char *buffer, char *filename, options *opts, int read,
              int line_count) {
  regex_t regex;
  int error = regcomp(&regex, opts->pattern, REG_EXTENDED | opts->i);
  if (error) {
    printf("Error compiling\n");
    exit(1);
  }
  int match_count_c = 0;
  int result = regexec(&regex, buffer, 0, NULL, 0);
  if ((result == 0 && !opts->v) || (opts->v && result != 0)) {
    if (!opts->c && !opts->l) {
      printing(buffer, read, opts, filename, line_count);
    }
    match_count_c++;
  }
  regfree(&regex);
  return match_count_c;
}

void printing(char *buffer, int n, options *opts, char *filename,
              int line_count) {
  if (opts->file_count > 1 && !opts->h) {
    printf("%s:", filename);
  }
  if (opts->n) {
    printf("%d:", line_count);
  }
  for (int i = 0; i < n; i++) {
    putchar(buffer[i]);
  }
  if (buffer[n - 1] != '\n') {
    putchar('\n');
  }
}