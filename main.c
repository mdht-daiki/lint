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

  if(length == 2) {
    int *add = addition(lint[0], lint[1]);
    lint_to_string(add, buf);
    printf("lint[0] + lint[1] = %s\n", buf);
    free(add);
    int *sub = subtraction(lint[0], lint[1]);
    lint_to_string(sub, buf);
    printf("lint[0] - lint[1] = %s\n", buf);
    free(sub);
  }

  for(i = 0; i < length; i++)
    free(lint[i]);
  free(lint);
  return 0;
}