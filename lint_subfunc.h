#ifndef LINT_SUBFUNC_H
#define LINT_SUBFUNC_H
typedef struct {
  int* digit;
  int length;
} Lint;
#endif
#define MAX_LENGTH 1000
#define LINT_END -1000

void trim_nl(char *s);
int check_string(char* S);
void lint_copy(Lint l0, Lint l1);