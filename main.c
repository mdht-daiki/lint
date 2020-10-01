#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"

int main(void) {
  int i, j;
  int length;
  char buf[MAX_LENGTH];
  int **lint = input_lint(&length);

  for(i = 0; i < length; i++) {
    lint_to_string(lint[i], buf);
    printf("lint[%d] = %s\n", i, buf);
  }

  for(i = 0; i < length; i++)
    free(lint[i]);
  free(lint);
  return 0;
}