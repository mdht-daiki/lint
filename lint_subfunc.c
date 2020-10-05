#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_fbao.h"
#include "lint_subfunc.h"
#define DEBUG_PRINTF printf

#define FREE(ptr) { \
    free(ptr); \
    ptr = NULL; \
} \

char DEBUG_BUF[MAX_LENGTH];

/* Lintのコンストラクタ */
void Lint_constructor(Lint *l_this, int length, int play) {
  // DEBUG_PRINTF("---lint_constructor---\n");
  l_this->length = length;
  l_this->digit = (int *)malloc(sizeof(int) * (length + play));
  if(l_this->digit == NULL) {
    printf("error: cannot alloc\n");
    exit(EXIT_FAILURE);
  }
  // DEBUG_PRINTF("malloc: %p\n", l_this->digit);
  for(int i = 0; i < length; i++) {
    l_this->digit[i] = 0;
  }
  if(play > 0)
    l_this->digit[length] = LINT_END;
  l_this->sign_pm = PLUS;
  l_this->dp = 0;
  l_this->loop_start = LINT_END;
  l_this->loop_end = LINT_END;
}

/*  */
void Lint_free(Lint a) {
  // DEBUG_PRINTF("free  : %p\n", a.digit);
  FREE(a.digit);
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
  l1->loop_start = l0.loop_start;
  l1->loop_end = l0.loop_end;
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
  
  for(int digit_pos = l.length - 1; digit_pos >= -1; digit_pos--) {
    // DEBUG_PRINTF("digit_pos = %d\n", digit_pos);
    // DEBUG_PRINTF("l.loop_start = %d\n", l.loop_start);
    // DEBUG_PRINTF("l.loop_end = %d\n", l.loop_end);
    if(digit_pos == l.loop_start)
      ans[ans_pos++] = '(';
    if(digit_pos == l.loop_end){
      ans[ans_pos++] = ')';
      break;
    }
    if(digit_pos >= 0) {
      ans[ans_pos++] = l.digit[digit_pos] + '0';
      if(l.dp != 0 && digit_pos == l.dp)
        ans[ans_pos++] = '.';
    }
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

  l_list = (Lint *)malloc(sizeof(Lint) * length);
  if(l_list == NULL) {
    printf("error: cannot alloc\n");
    exit(EXIT_FAILURE);
  }
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
  // DEBUG_PRINTF("---carry_borrow---\n");
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
      int n = (-ans.digit[i] - 1) / 10 + 1;
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
    ans_whole--;
  }

  Lint ans_partial = Lint_delete_zero(ans);
  Lint_free(ans);
  // DEBUG_PRINTF("---carry_borrow--- end\n");
  return ans_partial;
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
    if(a.digit[i] > b.digit[i]) {
      // DEBUG_PRINTF("LEFT\n");
      return LEFT;
    }
    if(a.digit[i] < b.digit[i]) {
      // DEBUG_PRINTF("RIGHT\n");
      return RIGHT;
    }
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

/* 小数点以下に0をn個加える */
Lint Lint_zero_fill(Lint l, int n) {
  // DEBUG_PRINTF("---Lint_zero_fill---");
  Lint ans;
  Lint_constructor(&ans, l.length + n, 1);
  ans.dp = l.dp + n;
  ans.sign_pm = l.sign_pm;
  for(int i = 0; i < n; i++)
    ans.digit[i] = 0;
  
  for(int i = 0; i < l.length; i++)
    ans.digit[i + n] = l.digit[i];
  ans.digit[ans.length] = LINT_END;
  return ans;
}

/* 小数点以下の桁数を揃える */
void arrange_decimal(Lint a, Lint b, Lint *a_fixed, Lint *b_fixed) {
  // DEBUG_PRINTF("---arrange_decimal---\n");
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
  // DEBUG_PRINTF("---arrange_decimal--- end\n");
}

/* aの上位n桁を切り取る */
Lint Lint_partial(Lint a, int n) {
  Lint a_partial;
  Lint_constructor(&a_partial, n, 1);

  int a_whole = a.length - a.dp;
  if(a_whole < n)
    a_partial.dp = n - a_whole;
  
  int a_pos = a.length - n;
  for(int partial_pos = 0; partial_pos < n; partial_pos++) {
    a_partial.digit[partial_pos] = a.digit[a_pos++];
  }
  a_partial.digit[n] = LINT_END;
  return a_partial;
}

/* 一桁の整数を表すLintを返す */
Lint Lint_one_digit(int n) {
  Lint one;
  Lint_constructor(&one, 1, 1);
  one.digit[0] = n;
  one.digit[1] = LINT_END;
  return one;
}

/* 10^n倍する */
Lint Lint_pow_10(Lint l, int n) {
  Lint ans = Lint_zero_fill(l, n);
  ans.dp = l.dp;
  return ans;
}

/* 小数点以下の末尾の0を削除する */
Lint Lint_delete_zero(Lint l) {
  Lint l_partial;
  int zero_after_dp = 0;
  for(int i = 0; i < l.dp; i++) {
    if(l.digit[i] == 0)
      zero_after_dp++;
    else
      break;
  }
  if(zero_after_dp != 0) {
    l_partial = Lint_partial(l, l.length - zero_after_dp);
    if(l.loop_start != LINT_END)
      l_partial.loop_start = l.loop_start - zero_after_dp;
    if(l.loop_end != LINT_END){
      l_partial.loop_end = l.loop_end - zero_after_dp;
      // DEBUG_PRINTF("l.loop_end = %d\n", l.loop_end);
      // DEBUG_PRINTF("zero_after_dp = %d\n", zero_after_dp);
      // DEBUG_PRINTF("l_partial.loop_end = %d\n", l_partial.loop_end);
    }
    return l_partial;
  }
  Lint_constructor(&l_partial, l.length, 1);
  lint_copy(l, &l_partial);
  return l_partial;
}

/* add、subの桁数設定 */
Lint constructor_add_sub(Lint a, Lint b) {
  // DEBUG_PRINTF("---constructor_add_sub---\n");
  // 整数部分、小数部分ともに桁が多い方に合わせる
  Lint ans;
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  int ans_whole = a_whole >= b_whole ? a_whole : b_whole;
  int ans_dp = a.dp >= b.dp ? a.dp : b.dp;
  int length = ans_whole + ans_dp;

  Lint_constructor(&ans, length, 1);                          // freeスタック [add]
  ans.dp = ans_dp;
  return ans;
}

/* addの演算内容 */
void add_calc(Lint *add, Lint a, Lint b) {
  if(a.sign_pm == b.sign_pm) {                                // 符号が同じ場合
    Lint_abstract_add(add, a, b);                               // 絶対値の足し算
    add->sign_pm = a.sign_pm;                                    // 符号を揃える
  } else {                                                    // 符号が違う場合
    Lint_abstract_sub(add, a, b);                               // 絶対値の引き算
    if(a.sign_pm == PLUS) {                                     // PLUS + MINUSのとき
      if(Lint_abstract_compare(a, b) == RIGHT)                    // MINUSの絶対値の方が大きい
        add->sign_pm = MINUS;                                       // PLUS + MINUS < 0
      else                                                        // 絶対値が同じ or PLUSの絶対値の方が大きい
        add->sign_pm = PLUS;                                        // PLUS + MINUS >= 0
    } else {                                                    // MINUS + PLUSのとき
      if(Lint_abstract_compare(a, b) == LEFT)                     // MINUSの絶対値の方が大きい
        add->sign_pm = MINUS;
      else                                                        // 絶対値が同じ or PLUSの絶対値の方が大きい
        add->sign_pm = PLUS;
    }
  }
}

/* subの演算内容 */
void sub_calc(Lint *sub, Lint a, Lint b) {
  if(Lint_compare(a, b) == RIGHT)     // a < bのとき a - b < 0
    sub->sign_pm = MINUS;                          
  if(a.sign_pm != b.sign_pm)                      // 符号が異なる時絶対値の和をとる
    Lint_abstract_add(sub, a, b);
  else                                            // 符号が同じ時絶対値の差をとる
    Lint_abstract_sub(sub, a, b);
}

/* mulのコンストラクタ */
Lint constructor_mul(Lint a, Lint b) {
  // DEBUG_PRINTF("---constructor_mul---\n");
  Lint ans;
  int length = a.length + b.length - 1;
  Lint_constructor(&ans, length, 1);              // freeスタック [mul]
  ans.dp = a.dp + b.dp;

  // 符号が異なる時、結果は負
  if(a.sign_pm != b.sign_pm)
  ans.sign_pm = MINUS;
  // DEBUG_PRINTF("---constructor_mul--- end\n");
  return ans;
}

/* mulの演算内容 */
void mul_calc(Lint *mul, Lint a, Lint b) {
  for(int i = 0; i < a.length; i++) {
    for(int j = 0; j < b.length; j++) {
      mul->digit[i+j] += a.digit[i] * b.digit[j];
    }
  }
  mul->digit[mul->length] = LINT_END;
}

/* bが整数になるように小数点を動かす */
void dp_move(Lint a, Lint b, Lint *a_10n, Lint *b_10n) {
  // DEBUG_PRINTF("---dp_move---\n");
  arrange_decimal(a, b, a_10n, b_10n);
  a_10n->dp -= b_10n->dp;
  b_10n->dp = 0;
  // DEBUG_PRINTF("---dp_move--- end\n");
}

/* remainの長さを設定する */
int set_remain_length(Lint a, Lint b) {
  // DEBUG_PRINTF("---set_remain_length---\n");
  Lint a_partial = Lint_partial(a, b.length);
  int a_whole = a.length - a.dp;
  int remain_length;

  if(a_whole <= b.length) 
    remain_length = a_whole;
  else {
    if(Lint_compare(a_partial, b) == RIGHT)
      remain_length = b.length + 1;
    else 
      remain_length = b.length;
  }
  Lint_free(a_partial);
  // DEBUG_PRINTF("---set_remain_length--- end\n");
  return remain_length;
}

/* remainの長さを設定する */
int set_div_dp(Lint a, Lint b) {
  // DEBUG_PRINTF("---set_div_dp---\n");
  Lint a_partial = Lint_partial(a, b.length);
  int a_whole = a.length - a.dp;
  int div_whole;

  if(a_whole < b.length) 
    div_whole = 1;
  else {
    if(Lint_compare(a_partial, b) == RIGHT)
      div_whole = a_whole - b.length;
    else 
      div_whole = a_whole - b.length + 1;
  }
  Lint_free(a_partial);
  // DEBUG_PRINTF("---set_div_dp--- end\n");
  return MAX_LENGTH - div_whole;
}

void div_calc(Lint *ans, Lint a, Lint b, int a_pos_init, Lint *remain) {
  // DEBUG_PRINTF("---div_calc---\n");
  int record_start_i = 0;
  Lint l_list[10];
  int a_pos = a_pos_init;
  int finish_flag = 0;

  for(int i = 0; i < 10; i++) {
    // DEBUG_PRINTF("l_list[%d]\n", i);
    l_list[i] = Lint_one_digit(i);
  }

  

  for(int i = MAX_LENGTH - 1; i >= 0; i--) {
    ans->digit[i] = 9;
    for(int j = 1; j <= 9; j++) {
      Lint x = multiplication(b, l_list[j]);;
      if(Lint_compare(x, remain[i + 1]) == LEFT) {
        ans->digit[i] = j - 1;
        // DEBUG_PRINTF("b * %d > remain[%d]\nans->digit[%d] = %d\n", j, i + 1, i, ans->digit[i]);
        Lint_free(x);
        break;
      }
      Lint_free(x);
    }

    Lint x_result = multiplication(b, l_list[ans->digit[i]]);
    // lint_to_string(x_result, DEBUG_BUF);
    // DEBUG_PRINTF("x_result = %s\n", DEBUG_BUF);

    if(i >= 1) {
      // DEBUG_PRINTF("i = %d >= 1\n", i);
      Lint x1 = subtraction(remain[i + 1], x_result);
      // lint_to_string(x1, DEBUG_BUF);
      // DEBUG_PRINTF("x1 = %s\n", DEBUG_BUF);
      // DEBUG_PRINTF("a_pos = %d\n", a_pos);
      if(a_pos == 0) {
        if(Lint_compare(x1, l_list[0]) == EQUAL) {
          Lint_free(x1);
          Lint_free(x_result);
          for(int j = 0; j < 10; j++) 
            Lint_free(l_list[j]);
          break;
        }
      }

      if(record_start_i == 0) {
        if(a_pos <= 0 && i <= ans->dp) {
          record_start_i = i - 1;
        }
        // DEBUG_PRINTF("record_start_i = %d\n", record_start_i);
      }


      if(a_pos > 0) {
        Lint x2 = Lint_pow_10(x1, 1);
        remain[i] = addition(x2, l_list[a.digit[a_pos - 1]]);
        a_pos--;
        Lint_free(x2);
        Lint_free(x1);
        Lint_free(x_result);
      } else {
        remain[i] = Lint_pow_10(x1, 1);
        // lint_to_string(remain[i], DEBUG_BUF);
        // DEBUG_PRINTF("remain[%d] = %s\n", i, DEBUG_BUF);
        // DEBUG_PRINTF("finish_flag = %d\n", finish_flag);
        Lint_free(x1);
        Lint_free(x_result);
        // DEBUG_PRINTF("record_start_i = %d\n", record_start_i);
        for(int j = record_start_i; j > i; j--) {
          // DEBUG_PRINTF("j = %d\n", j);
          // DEBUG_PRINTF("finish_flag = %d\n", finish_flag);
          if(Lint_compare(remain[i], remain[j]) == EQUAL) {
            ans->loop_start = j;
            ans->loop_end = i;
            finish_flag++;
            // DEBUG_PRINTF("same remain\n");
            // DEBUG_PRINTF("ans->loop_start = %d\n", ans->loop_start);
            // DEBUG_PRINTF("ans->loop_end = %d\n", ans->loop_end);
          }
        }
        // DEBUG_PRINTF("finish_flag = %d\n", finish_flag);
        if(finish_flag != 0) {
          // DEBUG_PRINTF("finished\n");
          for(int j = 0; j < 10; j++) 
            Lint_free(l_list[j]);
          break;
        }
      }
    } else {
      remain[i] = subtraction(remain[i + 1], x_result);
      Lint_free(x_result);
      for(int j = 0; j < 10; j++) 
        Lint_free(l_list[j]);
    }
  }
}


void DEBUG_PRINT_LINT(char *name, Lint a) {
  lint_to_string(a, DEBUG_BUF);
  printf("%s = %s\n", name, DEBUG_BUF);
}

Lint Lint_10_power_minus(int n) {
  Lint ans;
  Lint_constructor(&ans, n + 1, 1);
  Lint one = Lint_one_digit(1);
  lint_copy(one, &ans);
  ans.length = n + 1;
  ans.dp = n;
  return ans;
}