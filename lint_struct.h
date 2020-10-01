#ifndef LINT_STRUCT_H
#define LINT_STRUCT_H

/* 整数 */
typedef struct {
  int* digit;
  int length;
} Lint;

/* 小数 */
typedef struct {
  Lint whole;
  Lint dec;
} Ldec;
#endif