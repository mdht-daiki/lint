#include "lint_struct.h"
#define MAX_LENGTH 1000
#define LINT_END -1000

void Lint_constructor(Lint *l_this, int length, int play);
void trim_nl(char *s);
int check_string(char* S);
void lint_copy(Lint l0, Lint l1);