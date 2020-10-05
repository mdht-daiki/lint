#ifndef LINT_FBAO_H
#define LINT_FBAO_H
#include "lint_subfunc.h"

Lint addition(Lint a, Lint b);        /* 足し算 */
Lint subtraction(Lint a, Lint b);     /* 引き算 */
Lint multiplication(Lint a, Lint b);  /* 掛け算 */
Lint division(Lint a, Lint b);        /* 割り算 */
Lint power(Lint a, int i);
Lint add_all(Lint *l_list, int n);
#endif