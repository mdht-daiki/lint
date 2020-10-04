#ifndef LINT_STRUCT_H
#define LINT_STRUCT_H

#define MAX_LENGTH 1000
#define LINT_END -1000
#define LINT_CB_BUF 10

typedef enum {PLUS, MINUS} pm;
typedef enum {LEFT, EQUAL, RIGHT} compare;

/* 整数 */
typedef struct {
  int* digit;
  int length;
  pm sign_pm: 1;
  int dp;
  int loop_start;
  int loop_end;
} Lint;
#endif