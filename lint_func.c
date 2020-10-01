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
  int length = a.length >= b.length ? a.length : b.length;
  Lint_constructor(&sub, length, 1);
  for(int i = 0; i < sub.length; i++) {
    sub.digit[i] = (i < a.length ? a.digit[i] : 0) - (i < b.length ? b.digit[i] : 0);
  }
  sub.digit[sub.length] = LINT_END;
  Lint ans = carry_borrow(sub);
  free(sub.digit);
  return ans;
}