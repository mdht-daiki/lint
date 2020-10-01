#include "lint_struct.h"

void Lint_constructor(Lint *l_this, int length, int play);    /* Lintのコンストラクタ */
void trim_nl(char *s);                                        /* 文字列から改行を削除する */
int check_string(char* S);                                    /* 文字列がLintに変換できるものか確認する */
void lint_copy(Lint l0, Lint *l1);                            /* l0をl1にコピーする */
Lint string_to_lint(char* S);                                 /* 文字列をLintに変換する */
void lint_to_string(Lint l, char *ans);                       /* Lintを文字列に変換する */
Lint *input_lint(int *n);                                     /* Lintを入力する */
Lint carry_borrow(Lint l);                                    /* 繰り上がり・繰り下がり処理 */
compare Lint_compare(Lint a, Lint b);                         /* 大小比較 */