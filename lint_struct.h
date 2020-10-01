#ifndef LINT_STRUCT_H
#define LINT_STRUCT_H

typedef enum {PLUS, MINUS} pm;

/* 整数 */
typedef struct {
  int* digit;
  int length;
  pm sign_pm: 1;
} Lint;

/* 小数 */
typedef struct {
  Lint whole;
  Lint dec;
} Ldec;
#endif