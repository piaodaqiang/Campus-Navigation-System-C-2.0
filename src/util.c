#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "util.h"


void input_line(char *buf, int n) {
  fgets(buf, n, stdin);
  size_t L = strlen(buf);
  if (L>0 && buf[L-1]=='\n') buf[L-1]=0;
}


int read_int(const char *prompt) {
  char s[128];
  printf("%s", prompt);
  input_line(s, sizeof(s));
  return atoi(s);
}


double read_double(const char *prompt) {
  char s[128];
  printf("%s", prompt);
  input_line(s, sizeof(s));
  return atof(s);
}