#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_fbao.h"
#include "lint_subfunc.h"
#define DEBUG_PRINTF printf

#define FREE(ptr) { \
    free(ptr); \
    ptr = NULL; \
} \

char DEBUG_BUF[MAX_LENGTH];

/* 足し算 */
Lint addition(Lint a, Lint b) {
  // add : 繰り上がり・繰り下がり前の結果
  Lint add = constructor_add_sub(a, b);                        // freeスタック [add]
  // a_fixed, b_fixed : 小数点以下の桁数を揃えたa, b 
  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);                  // freeスタック [add a_fixed b_fixed]

  add_calc(&add, a_fixed, b_fixed);

  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  Lint ans = carry_borrow(add);                                 // freeスタック [add a_fixed b_fixed]

  
  // メモリ開放
  Lint_free(b_fixed);                                          // freeスタック [add a_fixed]
  Lint_free(a_fixed);                                          // freeスタック [add]
  Lint_free(add);                                              // freeスタック []

  return ans;
}

/* 引き算 */
Lint subtraction(Lint a, Lint b) {
  // DEBUG_PRINTF("---subtraction---\n");
  // sub : 繰り上がり・繰り下がり前の結果
  Lint sub = constructor_add_sub(a, b);           // freeスタック [sub]

  // a_fixed, b_fixed : 小数点以下の桁数を揃えたa, b 
  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);      // freeスタック [sub a_fixed b_fixed]

  sub_calc(&sub, a_fixed, b_fixed);

  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  Lint ans = carry_borrow(sub);                   // freeスタック [sub a_fixed b_fixed]

  // メモリ開放
  Lint_free(b_fixed);                            // freeスタック [sub a_fixed]
  Lint_free(a_fixed);                            // freeスタック [sub]
  Lint_free(sub);                                // freeスタック []
  // DEBUG_PRINTF("---subtraction--- end\n");
  return ans;
}

/* 掛け算（筆算アルゴリズム） */
Lint multiplication(Lint a, Lint b) {
  // DEBUG_PRINTF("---multiplication---\n");
  // mul : 繰り上がり・繰り下がり前の結果
  // DEBUG_PRINTF("mul\n");
  Lint mul = constructor_mul(a, b);               // freeスタック [mul]

  // 計算処理
  mul_calc(&mul, a, b);

  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  // DEBUG_PRINTF("ans\n");
  Lint ans = carry_borrow(mul);                   // freeスタック [mul]

  // メモリ開放
  Lint_free(mul);                                // freeスタック []
  // DEBUG_PRINTF("---multiplication--- end\n");
  return ans;
}

/* 割り算（整数用） */
Lint division(Lint a, Lint b) {
  Lint a_10n, b_10n, ans;
  int remain_length;
  
  // DEBUG_PRINTF("a_10n, b_10n\n");
  dp_move(a, b, &a_10n, &b_10n);
  remain_length = set_remain_length(a_10n, b_10n);
  // DEBUG_PRINTF("ans\n");
  Lint_constructor(&ans, MAX_LENGTH, 1);
  ans.dp = set_div_dp(a_10n, b_10n);
  // DEBUG_PRINTF("remain\n");
  Lint *remain = (Lint *)malloc(sizeof(Lint) * (MAX_LENGTH));
  if(remain == NULL) {
    printf("error:cannot alloc remain\n");
    exit(EXIT_FAILURE);
  }
  // DEBUG_PRINTF("malloc: %p\n", remain);
  remain[MAX_LENGTH] = Lint_partial(a_10n, remain_length);

  int a_pos = a_10n.length - remain_length;
  int record_start_i;

  div_calc(&ans, a, b, a_pos, remain);
  // lint_to_string(ans, DEBUG_BUF);
  // DEBUG_PRINTF("ans = %s\n", DEBUG_BUF);
  Lint ans_zero_deleted = Lint_delete_zero(ans);
  // lint_to_string(ans_zero_deleted, DEBUG_BUF);
  // DEBUG_PRINTF("ans_zero_deleted = %s\n", DEBUG_BUF);
  // DEBUG_PRINTF("ans_zero_deleted.loop_start = %d\n", ans_zero_deleted.loop_start);
  // DEBUG_PRINTF("ans_zero_deleted.loop_end = %d\n", ans_zero_deleted.loop_end);
  for(int i = 0; i < MAX_LENGTH; i++)
    Lint_free(remain[i]);
  FREE(remain);
  Lint_free(a_10n);
  Lint_free(b_10n);
  Lint_free(ans);
  return ans_zero_deleted;
}