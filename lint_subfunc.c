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

/* int型配列の要素数をカウントする */
int lint_length(int* lint) {
  int length = 0;
  do {
    length++;
  } while(lint[length] != -1);
  return length;
}

void lint_copy(int *lint_0, int *lint_1) {
  for(int i = 0; lint_0[i] != -1; i++) {
    lint_1[i] = lint_0[i];
  }
}