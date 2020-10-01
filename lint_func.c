#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"
#define LINT_CB_BUF 10

/* 文字列をint型配列に変換する */
Lint string_to_lint(char* S) {
  Lint l;
  l.length = strlen(S);
  l.digit = (int *)malloc(sizeof(int)*(l.length + 1));
  for(int i = 0; i < l.length; i++) {
    l.digit[i] = S[l.length - i - 1] - '0';
  }
  l.digit[l.length] = LINT_END;   /* 番兵 */
  return l;
}

/* int型配列を文字列に変換する */
void lint_to_string(Lint l, char *ans) {
  for(int i = 0; i < l.length; i++) {
    ans[i] = l.digit[l.length - i - 1] + '0';
  }
  ans[l.length] = '\0';
}

/* int型配列を入力する */
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
  ans.length = l.length;
  ans.digit = (int *)malloc(sizeof(int) * (ans.length + LINT_CB_BUF));   /* 桁数に余裕を持たせる */
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

/* 足し算 */
Lint addition(Lint a, Lint b) {
  Lint add;
  add.length = a.length >= b.length ? a.length : b.length;
  add.digit = (int *)malloc(sizeof(int) * (add.length + 1));
  for(int i = 0; i < add.length; i++) {
    add.digit[i] = (i < a.length ? a.digit[i] : 0) + (i < b.length ? b.digit[i] : 0);
  }
  add.digit[add.length] = LINT_END;
  Lint ans = carry_borrow(add);
  free(add.digit);
  return ans;
}

/* 引き算 */
Lint subtraction(Lint a, Lint b) {
  Lint sub;
  sub.length = a.length >= b.length ? a.length : b.length;
  sub.digit = (int *)malloc(sizeof(int) * (sub.length + 1));
  for(int i = 0; i < sub.length; i++) {
    sub.digit[i] = (i < a.length ? a.digit[i] : 0) - (i < b.length ? b.digit[i] : 0);
  }
  sub.digit[sub.length] = LINT_END;
  Lint ans = carry_borrow(sub);
  free(sub.digit);
  return ans;
}