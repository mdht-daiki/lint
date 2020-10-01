#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lint_subfunc.h"

/* 文字列から改行を削除する */
void trim_nl(char *s) {
  char *p;
  p = strchr(s, '\n');
  if(p != NULL) {
    *p = '\0';
  }
}

/* 文字列がint型配列に変換できるものか確認する */
int check_string(char* S) {
  int N = strlen(S);
  for(int i = 0; i < N; i++) {
    if(!(isdigit(S[i]))){
      printf("変換できません。\n");
      return 0;
    }
  }
  return 1;
}

void lint_copy(Lint l0, Lint l1) {
  for(int i = 0; l0.digit[i] != LINT_END; i++) {
    l1.digit[i] = l0.digit[i];
  }
}