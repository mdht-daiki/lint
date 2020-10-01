#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100

/* 文字列をint型配列に変換する */
int *string_to_lint(char* S) {
  int N = strlen(S);
  int *lint = (int *)malloc(sizeof(int)*(N+1));
  for(int i = 0; i < N; i++) {
    lint[i] = S[N - i - 1] - '0';
  }
  lint[N] = -1;
  return lint;
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

int main(void) {
  int *lint;
  char buf[MAX_LENGTH], digit[MAX_LENGTH];
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%s", digit);

  if(check_string(digit)) {
    lint = string_to_lint(digit);
    for(int i = 0; lint[i] != -1; i++)
    printf("lint[%d] = %d\n", i, lint[i]);
  }

  return 0;
}