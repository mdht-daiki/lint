#ifndef LINT_SUBFUNC_H
#define LINT_SUBFUNC_H
#include "lint_struct.h"

void Lint_constructor(Lint *l_this, int length, int play);            /* Lintのコンストラクタ */
void trim_nl(char *s);                                                /* 文字列から改行を削除する */
int check_string(char* S);                                            /* 文字列がLintに変換できるものか確認する */
void lint_copy(Lint l0, Lint *l1);                                    /* l0をl1にコピーする */
Lint string_to_lint(char* S);                                         /* 文字列をLintに変換する */
void lint_to_string(Lint l, char *ans);                               /* Lintを文字列に変換する */
Lint *input_lint(int *n);                                             /* Lintを入力する */
Lint carry_borrow(Lint l);                                            /* 繰り上がり・繰り下がり処理 */
compare Lint_compare(Lint a, Lint b);                                 /* 大小比較 */
compare invert_compare(compare c);                                    /* 比較結果をひっくり返す */
compare Lint_abstract_compare(Lint a, Lint b);                        /* 絶対値の比較 */
void Lint_abstract_add(Lint *ans, Lint a, Lint b);                    /* 絶対値の和 */
void Lint_abstract_sub(Lint *ans, Lint a, Lint b);                    /* 絶対値の差 */
Lint Lint_zero_fill(Lint l, int n);                                   /* 小数点以下に0をn個加える */
void arrange_decimal(Lint a, Lint b, Lint *a_fixed, Lint *b_fixed);   /* 小数点以下の桁数を揃える */
Lint Lint_partial(Lint a, int n);                                     /* aの上位n桁を切り取る */
Lint Lint_one_digit(int n);                                           /* 一桁の整数を表すLintを返す */
Lint Lint_pow_10(Lint l, int n);                                      /* 10^n倍する */
Lint Lint_delete_zero(Lint l);                                        /* 小数点以下の末尾の0を削除する */

#endif