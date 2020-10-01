#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_subfunc.h"

/* 文字列をint型配列に変換する */
int *string_to_lint(char* S) {
  int N = strlen(S);
  int *lint = (int *)malloc(sizeof(int)*(N+1));
  for(int i = 0; i < N; i++) {
    lint[i] = S[N - i - 1] - '0';
  }
  lint[N] = -1;   /* 番兵 */
  return lint;
}

/* int型配列を文字列に変換する */
void lint_to_string(int *lint, char *ans) {
  int length = lint_length(lint);
  for(int i = 0; i < length; i++) {
    ans[i] = lint[length - i - 1] + '0';
  }
  ans[length] = '\0';
}

int **input_lint(int *n) {
  char buf[MAX_LENGTH];
  printf("数値を何個入力しますか：");
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", n);
  int length = *n;

  int **lint = malloc(sizeof(int *) * length);
  for(int i = 0; i < length; i++) {
    fgets(buf, sizeof(buf), stdin);
    trim_nl(buf);
    if(check_string(buf))
      lint[i] = string_to_lint(buf);
  }

  return lint;
}