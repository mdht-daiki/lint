#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"
#define LINT_CB_BUF 10

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

/* int型配列を入力する */
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

/* 繰り上がり・繰り下がり処理 */
int *carry_borrow(int *lint) {
  int length = lint_length(lint);
  int *ans = (int *)malloc(sizeof(int) * (length + LINT_CB_BUF));   /* 桁数に余裕を持たせる */
  lint_copy(lint, ans);

  for(int i = 0; i < length-1; i++) {
    /* 繰り上がり */
    if(ans[i] >= 10) {
      int n = ans[i] / 10;
      ans[i] -= n * 10;
      ans[i + 1] += n;
    }

    /* 繰り下がり */
    if(ans[i] < 0) {
      int n = (ans[i] - 1) / 10 + 1;
      ans[i] += n * 10;
      ans[i + 1] -= n;
    }
  }
  ans[length] = -1;
  /* 最大桁が10以上の時、桁数を増やす */
  while(ans[length - 1] >= 10) {
    int n = ans[length - 1] / 10;
    ans[length - 1] -= n * 10;
    ans[length++] = n;
    ans[length] = -1;
  }

  /* 最大桁が0かつ桁数が1桁でない時、最大桁の0を消す */
  while(length >= 2 && ans[length - 1] == 0) {
    ans[--length] = -1;
  }
  return ans;
}

int *addition(int *lint_a, int *lint_b) {
  int length_a = lint_length(lint_a), length_b = lint_length(lint_b);
  int length = length_a >= length_b ? length_a : length_b;
  int *add = (int *)malloc(sizeof(int) * (length + 1));
  for(int i = 0; i < length; i++) {
    add[i] = (i < length_a ? lint_a[i] : 0) + (i < length_b ? lint_b[i] : 0);
  }
  add[length] = -1;
  int *ans = carry_borrow(add);
  free(add);
  return ans;
}