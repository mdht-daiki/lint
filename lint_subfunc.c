#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_subfunc.h"

/* Lintのコンストラクタ */
void Lint_constructor(Lint *l_this, int length, int play) {
  l_this->length = length;
  l_this->digit = (int *)malloc(sizeof(int) * (length + play));
  for(int i = 0; i < length; i++) {
    l_this->digit[i] = 0;
  }
  l_this->sign_pm = PLUS;
  l_this->dp = 0;
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
  int cannot_flag = 0, dp_flag = 0;
  for(int i = 0; i < N; i++) {
    if(!(isdigit(S[i]))) {                              /* 数字以外の記号 */
      if(S[i] == '-') {
        if(i != 0) cannot_flag++;                       /* マイナス符号が先頭以外に存在する */
      } else if(S[i] == '.') {
        if(i == 0 || i == N - 1) cannot_flag++;         /* 小数点が先頭または最後尾に存在する */
        else {
          if(dp_flag != 0) cannot_flag++;               /* 小数点が2つ以上ある */
          else {
            dp_flag++;
            if(S[0] == '-') {
              if(i == 1) cannot_flag++;                 /* マイナスの直後に小数点が存在する -.000 */
            }
          }
        }
      } else 
        cannot_flag++;                                  /* マイナス、小数点以外の記号 */
    }
  }
  if(cannot_flag != 0) {
    printf("変換できません。\n");
    return 0;
  }
  return 1;
}

/* l0をl1にコピーする */
void lint_copy(Lint l0, Lint *l1) {
  l1->sign_pm = l0.sign_pm;
  l1->dp = l0.dp;
  l1->length = l0.length;
  for(int i = 0; l0.digit[i] != LINT_END; i++) {
    l1->digit[i] = l0.digit[i];
  }
}

/* 文字列をLintに変換する */
Lint string_to_lint(char* S) {
  Lint l;
  Lint_constructor(&l, strlen(S), 1);

  int s_pos = l.length - 1;
  for(int i = 0; i < l.length; i++) {

    if(i == l.length - 1 && S[s_pos] == '-'){
      l.sign_pm = MINUS;
      l.length--;
      s_pos--;
    } else {

      if(S[s_pos] == '.') {
        l.dp = i;
        l.length--;
        s_pos--;
      }

      l.digit[i] = S[s_pos--] - '0';
    }    
  }
  l.digit[l.length] = LINT_END;   /* 番兵 */
  return l;
}

/* Lintを文字列に変換する */
void lint_to_string(Lint l, char *ans) {
  int ans_pos = 0;
  if(l.sign_pm == MINUS)
    ans[ans_pos++] = '-';
  
  for(int digit_pos = l.length - 1; digit_pos >= 0; digit_pos--) {
    ans[ans_pos++] = l.digit[digit_pos] + '0';
    if(l.dp != 0 && digit_pos == l.dp)
      ans[ans_pos++] = '.';
  }
  ans[ans_pos] = '\0';
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
  lint_copy(l, &ans);

  int ans_whole = ans.length - ans.dp;
  for(int i = 0; i < l.length-1; i++) {
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
  while(ans_whole >= 2 && ans.digit[ans.length - 1] == 0) {
    ans.digit[--ans.length] = LINT_END;
  }
  return ans;
}

/* 比較結果をひっくり返す */
compare invert_compare(compare c) {
  if(c == LEFT) return RIGHT;
  if(c == RIGHT) return LEFT;
  return EQUAL;
}

/* 絶対値の比較 */
compare Lint_abstract_compare(Lint a, Lint b) {
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  if(a_whole > b_whole) return LEFT;
  if(a_whole < b_whole) return RIGHT;

  int length = a.length >= b.length ? a.length : b.length;
  for(int i = length - 1; i >= 0; i--) {
    if(a.digit[i] > b.digit[i]) return LEFT;
    if(a.digit[i] < b.digit[i]) return RIGHT;
  }
  return EQUAL;
}

/* 大小比較 */
compare Lint_compare(Lint a, Lint b) {
  /* 符号が違う場合 */
  if(a.sign_pm == PLUS && b.sign_pm == MINUS) return LEFT;
  if(a.sign_pm == MINUS && b.sign_pm == PLUS) return RIGHT;

  /* 符号が同じ場合 */
  if(a.sign_pm == PLUS && b.sign_pm == PLUS) 
    return Lint_abstract_compare(a, b);

  if(a.sign_pm == MINUS && b.sign_pm == MINUS) {
    return invert_compare(Lint_abstract_compare(a, b));
  }

  return EQUAL;
}

/* 絶対値の和 */
void Lint_abstract_add(Lint *ans, Lint a, Lint b) {
  for(int i = 0; i < ans->length; i++) {
    ans->digit[i] = (i < a.length ? a.digit[i] : 0) + (i < b.length ? b.digit[i] : 0);
  }
  ans->digit[ans->length] = LINT_END;
}

/* 絶対値の差 */
void Lint_abstract_sub(Lint *ans, Lint a, Lint b) {
  if(Lint_abstract_compare(a, b) == LEFT)
    for(int i = 0; i < ans->length; i++) {
      ans->digit[i] = (i < a.length ? a.digit[i] : 0) - (i < b.length ? b.digit[i] : 0);
    }
  else 
    for(int i = 0; i < ans->length; i++) {
      ans->digit[i] = (i < b.length ? b.digit[i] : 0) - (i < a.length ? a.digit[i] : 0);
    }
  ans->digit[ans->length] = LINT_END;
}

/* 10^n倍する */
Lint Lint_zero_fill(Lint l, int n) {
  Lint ans;
  Lint_constructor(&ans, l.length + n, 1);
  ans.dp = l.dp + n;
  ans.sign_pm = l.sign_pm;
  for(int i = 0; i < n; i++)
    ans.digit[i] = 0;
  
  for(int i = 0; i < l.length; i++)
    ans.digit[i + n] = l.digit[i];
  ans.digit[ans.length] = LINT_END;
  char buf[MAX_LENGTH];
  return ans;
}

/* 小数点以下の桁数を揃える */
void arrange_decimal(Lint a, Lint b, Lint *a_fixed, Lint *b_fixed) {
    if(a.dp > b.dp) {
    Lint_constructor(a_fixed, a.length, 0);
    lint_copy(a, a_fixed);
    *b_fixed = Lint_zero_fill(b, a.dp - b.dp);
  } else if(a.dp < b.dp) {
    *a_fixed = Lint_zero_fill(a, b.dp - a.dp);
    Lint_constructor(b_fixed, a.length, 0);
    lint_copy(b, b_fixed);
  } else {
    Lint_constructor(a_fixed, a.length, 0);
    lint_copy(a, a_fixed);
    Lint_constructor(b_fixed, a.length, 0);
    lint_copy(b, b_fixed);
  }
}