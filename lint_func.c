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
  free(sub.digit);
  return ans;
}

Lint multiplication(Lint a, Lint b) {
  int length = a.length + b.length - 1;
  Lint mul;
  Lint_constructor(&mul, length, 1);
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