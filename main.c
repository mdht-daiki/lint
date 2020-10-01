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
      printf("length = %d\n", lint_length(lint[i]));
      lint_to_string(lint[i], buf);
      printf("lint[%d] = %s\n", i, buf);
  }

  for(i = 0; i < n; i++)
    free(lint[i]);
  free(lint);
  return 0;
}