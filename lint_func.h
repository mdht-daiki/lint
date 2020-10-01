#include "lint_subfunc.h"

Lint *input_lint(int *n);
Lint string_to_lint(char* S);
void lint_to_string(Lint l, char *ans);
Lint carry_borrow(Lint l);
Lint addition(Lint a, Lint b);
Lint subtraction(Lint a, Lint b);