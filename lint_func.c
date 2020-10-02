#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"

/* 足し算 */
Lint addition(Lint a, Lint b) {
  Lint add;
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  int add_whole = a_whole >= b_whole ? a_whole : b_whole;
  int add_dp = a.dp >= b.dp ? a.dp : b.dp;
  int length = add_whole + add_dp;

  Lint_constructor(&add, length, 1);
  add.dp = add_dp;

  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);

  if(a.sign_pm == b.sign_pm) {
    Lint_abstract_add(&add, a_fixed, b_fixed);
    add.sign_pm = a.sign_pm;
  } else {
    Lint_abstract_sub(&add, a_fixed, b_fixed);
    if(a.sign_pm == PLUS) {
      if(Lint_abstract_compare(a_fixed, b_fixed) == LEFT)
        add.sign_pm = PLUS;
      else
        add.sign_pm = MINUS;
    } else {
      if(Lint_abstract_compare(a_fixed, b_fixed) == LEFT)
        add.sign_pm = MINUS;
      else
        add.sign_pm = PLUS;
    }
  }
  Lint ans = carry_borrow(add);
  free(a_fixed.digit);
  free(b_fixed.digit);
  free(add.digit);
  return ans;
}

/* 引き算 */
Lint subtraction(Lint a, Lint b) {
  Lint sub;
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  int sub_whole = a_whole >= b_whole ? a_whole : b_whole;
  int sub_dp = a.dp >= b.dp ? a.dp : b.dp;
  int length = sub_whole + sub_dp;

  Lint_constructor(&sub, length, 1);
  sub.dp = sub_dp;

  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);

  if(Lint_compare(a_fixed, b_fixed) == RIGHT)
    sub.sign_pm = MINUS;
  if(a.sign_pm != b.sign_pm)
    Lint_abstract_add(&sub, a_fixed, b_fixed);
  else
    Lint_abstract_sub(&sub, a_fixed, b_fixed);
  
  Lint ans = carry_borrow(sub);
  free(a_fixed.digit);
  free(b_fixed.digit);
  free(sub.digit);
  return ans;
}

Lint multiplication(Lint a, Lint b) {
  int length = a.length + b.length - 1;
  Lint mul;
  Lint_constructor(&mul, length, 1);
  mul.dp = a.dp + b.dp;
  if(a.sign_pm != b.sign_pm)
    mul.sign_pm = MINUS;
  for(int i = 0; i < a.length; i++) {
    for(int j = 0; j < b.length; j++) {
      mul.digit[i+j] += a.digit[i] * b.digit[j];
    }
  }
  mul.digit[mul.length] = LINT_END;
  Lint ans = carry_borrow(mul);
  free(mul.digit);
  return ans;
}

Lint division(Lint a, Lint b) {
  Lint div;

  if(a.length < b.length)
    return Lint_one_digit(0);

  int length = a.length - b.length;
  Lint a_partial = Lint_partial(a, b.length);
  if(Lint_compare(a_partial, b) == LEFT)
    length++;
  free(a_partial.digit);
  
  if(length == 0)
    return Lint_one_digit(0);
  Lint *remain = (Lint *)malloc(sizeof(Lint) * (length + 1));
  remain[length] = Lint_partial(a, a.length - length + 1);
  Lint_constructor(&div, length, 1);
  Lint l_list[10];
  for(int i = 0; i < 10; i++) {
    l_list[i] = Lint_one_digit(i);
  }
  for(int i = length - 1; i >= 0; i--) {
    div.digit[i] = 9;
    for(int j = 1; j <= 9; j++) {
      Lint x = multiplication(b, l_list[j]);
      if(Lint_compare(x, remain[i + 1]) == LEFT) {
        div.digit[i] = j - 1;
        break;
      }
      free(x.digit);
    }
    Lint x_result = multiplication(b, l_list[div.digit[i]]);
    remain[i] = subtraction(remain[i + 1], x_result);
    if(i >= 1) {
      Lint x1 = subtraction(remain[i + 1], x_result);
      Lint x2 = Lint_pow_10(x1, 1);
      remain[i] = addition(x2, l_list[a.digit[i - 1]]);
      free(x1.digit);
      free(x2.digit);
    } else {
      remain[i] = subtraction(remain[i + 1], x_result);
    }
    free(x_result.digit);
    free(remain[i + 1].digit);
  }
  free(remain[0].digit);
  free(remain);
  for(int i = 0; i < 10; i++)
    free(l_list[i].digit);
  
  return div;
}