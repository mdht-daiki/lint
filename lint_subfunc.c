#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_subfunc.h"

/* Lintのコンストラクタ */
void Lint_constructor(Lint *l_this, int length, int play) {
  l_this->length = length;
  l_this->digit = (int *)malloc(sizeof(int) * (length + play));
  l_this->sign_pm = PLUS;
}

/* 文字列から改行を削除する */
void trim_nl(char *s) {
  char *p;
  p = strchr(s, '\n');
  if(p != NULL) {
    *p = '\0';
  }
}

/* 文字列がLintに変換できるものか確認する */
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

/* 文字列をLintに変換する */
Lint string_to_lint(char* S) {
  Lint l;
  Lint_constructor(&l, strlen(S), 1);
  for(int i = 0; i < l.length; i++) {
    l.digit[i] = S[l.length - i - 1] - '0';
  }
  l.digit[l.length] = LINT_END;   /* 番兵 */
  return l;
}

/* Lintを文字列に変換する */
void lint_to_string(Lint l, char *ans) {
  for(int i = 0; i < l.length; i++) {
    ans[i] = l.digit[l.length - i - 1] + '0';
  }
  ans[l.length] = '\0';
}

/* Lintを入力する */
Lint *input_lint(int *n) {
  Lint* l_list;
  char buf[MAX_LENGTH];
  printf("数値を何個入力しますか：");
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", n);
  int length = *n;

  l_list = malloc(sizeof(Lint) * length);
  for(int i = 0; i < length; i++) {
    fgets(buf, sizeof(buf), stdin);
    trim_nl(buf);
    if(check_string(buf))
      l_list[i] = string_to_lint(buf);
  }

  return l_list;
}

/* 繰り上がり・繰り下がり処理 */
Lint carry_borrow(Lint l) {
  Lint ans;
  Lint_constructor(&ans, l.length, LINT_CB_BUF);
  lint_copy(l, ans);

  for(int i = 0; i < l.length; i++) {
    /* 繰り上がり */
    if(ans.digit[i] >= 10) {
      int n = ans.digit[i] / 10;
      ans.digit[i] -= n * 10;
      ans.digit[i + 1] += n;
    }

    /* 繰り下がり */
    if(ans.digit[i] < 0) {
      int n = (ans.digit[i] - 1) / 10 + 1;
      ans.digit[i] += n * 10;
      ans.digit[i + 1] -= n;
    }
  }
  ans.digit[ans.length] = LINT_END;
  /* 最大桁が10以上の時、桁数を増やす */
  while(ans.digit[ans.length - 1] >= 10) {
    int n = ans.digit[ans.length - 1] / 10;
    ans.digit[ans.length - 1] -= n * 10;
    ans.digit[ans.length++] = n;
    ans.digit[ans.length] = LINT_END;
  }

  /* 最大桁が0かつ桁数が1桁でない時、最大桁の0を消す */
  while(ans.length >= 2 && ans.digit[ans.length - 1] == 0) {
    ans.digit[--ans.length] = LINT_END;
  }
  return ans;
}