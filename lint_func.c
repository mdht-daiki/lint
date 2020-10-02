#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"

/* 足し算 */
Lint addition(Lint a, Lint b) {
  Lint add;
  int length = a.length >= b.length ? a.length : b.length;
  Lint_constructor(&add, length, 1);
  if(a.sign_pm == b.sign_pm) {
    Lint_abstract_add(&add, a, b);
    add.sign_pm = a.sign_pm;
  } else {
    Lint_abstract_sub(&add, a, b);
    if(a.sign_pm == PLUS) {
      if(Lint_abstract_compare(a, b) == LEFT)
        add.sign_pm = PLUS;
      else
        add.sign_pm = MINUS;
    } else {
      if(Lint_abstract_compare(a, b) == LEFT)
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
  int length = a.length >= b.length ? a.length : b.length;
  Lint_constructor(&sub, length, 1);

  if(Lint_compare(a, b) == RIGHT)
    sub.sign_pm = MINUS;
  if(a.sign_pm != b.sign_pm)
    Lint_abstract_add(&sub, a, b);
  else
    Lint_abstract_sub(&sub, a, b);
  
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