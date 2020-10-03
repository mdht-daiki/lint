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
    free(add.digit);

    // 引き算
    Lint sub = subtraction(l_list[0], l_list[1]);
    lint_to_string(sub, buf);
    printf("lint[0] - lint[1] = %s\n", buf);
    free(sub.digit);

    // 掛け算
    Lint mul = multiplication(l_list[0], l_list[1]);
    lint_to_string(mul, buf);
    printf("lint[0] * lint[1] = %s\n", buf);
    free(mul.digit);

    // 割り算
    Lint div = division(l_list[0], l_list[1]);
    lint_to_string(div, buf);
    printf("lint[0] / lint[1] = %s\n", buf);
    free(div.digit);
  }

  for(i = 0; i < length; i++)
    free(l_list[i].digit);
  free(l_list);
  return 0;
}