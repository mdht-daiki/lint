#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_fbao.h"
#include "lint_subfunc.h"
#define DEBUG_PRINTF printf

#define FREE(ptr) { \
    free(ptr); \
    ptr = NULL; \
} \

int main(void) {
  int i, j;
  int length;
  char buf[MAX_LENGTH];
  Lint *l_list = input_lint(&length);

  for(i = 0; i < length; i++) {
    lint_to_string(l_list[i], buf);
    printf("lint[%d] = %s\n", i, buf);
  }

  if(length == 2) {
    // 足し算
    Lint add = addition(l_list[0], l_list[1]);
    lint_to_string(add, buf);
    printf("lint[0] + lint[1] = %s\n", buf);
    Lint_free(add);

    // 引き算
    Lint sub = subtraction(l_list[0], l_list[1]);
    lint_to_string(sub, buf);
    printf("lint[0] - lint[1] = %s\n", buf);
    Lint_free(sub);

    // 掛け算
    Lint mul = multiplication(l_list[0], l_list[1]);
    lint_to_string(mul, buf);
    printf("lint[0] * lint[1] = %s\n", buf);
    Lint_free(mul);

    // 割り算
    Lint div = division(l_list[0], l_list[1]);
    lint_to_string(div, buf);
    printf("lint[0] / lint[1] = %s\n", buf);
    // DEBUG_PRINTF("div.dp = %d\n", div.dp);
    Lint_free(div);
  }

  for(i = 0; i < length; i++)
    Lint_free(l_list[i]);
  FREE(l_list);
  return 0;
}