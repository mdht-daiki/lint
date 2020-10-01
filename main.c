#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100

/* 文字列から改行を削除する */
void trim_nl(char *s) {
  char *p;
  p = strchr(s, '\n');
  if(p != NULL) {
    *p = '\0';
  }
}

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
  int i, j;
  int n;
  char buf[MAX_LENGTH];
  printf("数値を何個入力しますか：");
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", &n);

  int **lint = malloc(sizeof(int *) * n);
  for(i = 0; i < n; i++) {
    fgets(buf, sizeof(buf), stdin);
    trim_nl(buf);
    if(check_string(buf))
      lint[i] = string_to_lint(buf);
  }

  for(i = 0; i < n; i++) {
      for(j = 0; lint[i][j] != -1; j++)
        printf("lint[%d][%d] = %d\n", i, j, lint[i][j]);
  }

  for(i = 0; i < n; i++)
    free(lint[i]);
  free(lint);
  return 0;
}