#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lint_func.h"
#include "lint_subfunc.h"

/* 足し算 */
Lint addition(Lint a, Lint b) {
  // add : 繰り上がり・繰り下がり前の結果
  Lint add;

  // addの桁数設定
  // 整数部分、小数部分ともに桁が多い方に合わせる
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  int add_whole = a_whole >= b_whole ? a_whole : b_whole;
  int add_dp = a.dp >= b.dp ? a.dp : b.dp;
  int length = add_whole + add_dp;

  Lint_constructor(&add, length, 1);                          // freeスタック [add]
  add.dp = add_dp;


  // a_fixed, b_fixed : 小数点以下の桁数を揃えたa, b 
  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);                  // freeスタック [add a_fixed b_fixed]


  if(a.sign_pm == b.sign_pm) {                                // 符号が同じ場合
    Lint_abstract_add(&add, a_fixed, b_fixed);                  // 絶対値の足し算
    add.sign_pm = a.sign_pm;                                    // 符号を揃える
  } else {                                                    // 符号が違う場合
    Lint_abstract_sub(&add, a_fixed, b_fixed);                  // 絶対値の引き算
    if(a.sign_pm == PLUS) {                                     // PLUS + MINUSのとき
      if(Lint_abstract_compare(a_fixed, b_fixed) == RIGHT)        // MINUSの絶対値の方が大きい
        add.sign_pm = MINUS;                                        // PLUS + MINUS < 0
      else                                                        // 絶対値が同じ or PLUSの絶対値の方が大きい
        add.sign_pm = PLUS;                                         // PLUS + MINUS >= 0
    } else {                                                    // MINUS + PLUSのとき
      if(Lint_abstract_compare(a_fixed, b_fixed) == LEFT)         // MINUSの絶対値の方が大きい
        add.sign_pm = MINUS;
      else                                                        // 絶対値が同じ or PLUSの絶対値の方が大きい
        add.sign_pm = PLUS;
    }
  }
  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  Lint ans = carry_borrow(add);                                 // freeスタック [add a_fixed b_fixed]

  // メモリ開放
  free(b_fixed.digit);                                          // freeスタック [add a_fixed]
  free(a_fixed.digit);                                          // freeスタック [add]
  free(add.digit);                                              // freeスタック []

  return ans;
}

/* 引き算 */
Lint subtraction(Lint a, Lint b) {
  // sub : 繰り上がり・繰り下がり前の結果
  Lint sub;

  // subの桁数設定
  // 整数部分、小数部分ともに桁が多い方に合わせる
  int a_whole = a.length - a.dp;
  int b_whole = b.length - b.dp;
  int sub_whole = a_whole >= b_whole ? a_whole : b_whole;
  int sub_dp = a.dp >= b.dp ? a.dp : b.dp;
  int length = sub_whole + sub_dp;

  Lint_constructor(&sub, length, 1);              // freeスタック [sub]
  sub.dp = sub_dp;

  // a_fixed, b_fixed : 小数点以下の桁数を揃えたa, b 
  Lint a_fixed, b_fixed;
  arrange_decimal(a, b, &a_fixed, &b_fixed);      // freeスタック [sub a_fixed b_fixed]

  if(Lint_compare(a_fixed, b_fixed) == RIGHT)     // a < bのとき a - b < 0
    sub.sign_pm = MINUS;                          
  if(a.sign_pm != b.sign_pm)                      // 符号が異なる時絶対値の和をとる
    Lint_abstract_add(&sub, a_fixed, b_fixed);
  else                                            // 符号が同じ時絶対値の差をとる
    Lint_abstract_sub(&sub, a_fixed, b_fixed);

  // printf("subtraction\n");                        // --------------- FOR DEBUG--------------- 
  // for(int i = 0; i < sub.length; i++)             // --------------- FOR DEBUG--------------- 
  //   printf("%d ", sub.digit[i]);                  // --------------- FOR DEBUG--------------- 
  // printf("\n");                                   // --------------- FOR DEBUG--------------- 

  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  Lint ans = carry_borrow(sub);                   // freeスタック [sub a_fixed b_fixed]

  // メモリ開放
  free(b_fixed.digit);                            // freeスタック [sub a_fixed]
  free(a_fixed.digit);                            // freeスタック [sub]
  free(sub.digit);                                // freeスタック []
  
  return ans;
}

/* 掛け算（筆算アルゴリズム） */
Lint multiplication(Lint a, Lint b) {
  // mul : 繰り上がり・繰り下がり前の結果
  Lint mul;
  // mulの桁数設定
  int length = a.length + b.length - 1;
  Lint_constructor(&mul, length, 1);              // freeスタック [mul]
  mul.dp = a.dp + b.dp;

  // 符号が異なる時、結果は負
  if(a.sign_pm != b.sign_pm)
    mul.sign_pm = MINUS;

  // 計算処理
  for(int i = 0; i < a.length; i++) {
    for(int j = 0; j < b.length; j++) {
      mul.digit[i+j] += a.digit[i] * b.digit[j];
    }
  }
  mul.digit[mul.length] = LINT_END;

  // printf("multiplication\n");                     // --------------- FOR DEBUG--------------- 
  // for(int i = 0; i < mul.length; i++)             // --------------- FOR DEBUG--------------- 
  //   printf("%d ", mul.digit[i]);                  // --------------- FOR DEBUG--------------- 
  // printf("\n");                                   // --------------- FOR DEBUG--------------- 

  // ans : 繰り上がり・繰り下がり後の結果 
  // returnするためfree不要 
  Lint ans = carry_borrow(mul);                   // freeスタック [mul]

  // メモリ開放
  free(mul.digit);                                // freeスタック []
  
  return ans;
}

/* 割り算（整数用） */
Lint division(Lint a, Lint b) {
  // char buf1[MAX_LENGTH], buf2[MAX_LENGTH];                     // --------------- FOR DEBUG--------------- 

  // ans : 計算結果
  // returnするためfree不要
  Lint ans;

  // 割る数の方が桁数が多い場合0を返す
  if(a.length < b.length)
    return Lint_one_digit(0);
  
  // ansの桁数設定
  // aの上位b.length桁がbより大きい場合最上位にもう一桁追加できる
  int length = a.length - b.length;
  Lint a_partial = Lint_partial(a, b.length);                     // freeスタック [a_partial]
  if(Lint_compare(a_partial, b) == LEFT || Lint_compare(a_partial, b) == EQUAL)
    length++;
  free(a_partial.digit);                                          // freeスタック []
  
  // ansが0桁のとき0を返す
  if(length == 0)
    return Lint_one_digit(0);

  // l_list[i] : 整数iをLint型で表現したもの
  // 計算の際に必要となるためここで初期化
  Lint l_list[10];
  for(int i = 0; i < 10; i++) {
    l_list[i] = Lint_one_digit(i);
  }                                                               // freeスタック [(l_list[0] - l_list[9])]

  // remain[i] : i桁目を計算した後、次の段に降りてくる値
  // remain[length] : 計算スタート時の計算範囲
  // remain[0] : 割り算のあまり
  Lint *remain = (Lint *)malloc(sizeof(Lint) * (length + 1));     // freeスタック [(l_list[0] - l_list[9]) remain]
  if(remain == NULL) {
    printf("error: cannot alloc remain\n");
    exit(EXIT_FAILURE);
  }
  remain[length] = Lint_partial(a, a.length - length + 1);        // freeスタック [(l_list[0] - l_list[9]) remain remain[length]]
  

  // returnするためfree不要
  Lint_constructor(&ans, length, 1);                              // freeスタック [(l_list[0] - l_list[9]) remain remain[length]]

  // 符号が異なる時、結果は負
  if(a.sign_pm != b.sign_pm)
    ans.sign_pm = MINUS;
  // 計算処理
  // ansの最上位からans[0]まで降りていく
  for(int i = length - 1; i >= 0; i--) {                      // for1
    //                                                               freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1]]

    // ansのi桁目の計算

    // remainがb*9より大きい時9を代入する
    ans.digit[i] = 9;
    
    // b*jがremainより大きくなったら、その直前のj-1を代入する
    for(int j = 1; j <= 9; j++) {                               // for2
      // lint_to_string(b, buf1);                                    // --------------- FOR DEBUG--------------- 
      // lint_to_string(l_list[j], buf2);                            // --------------- FOR DEBUG--------------- 
      // printf("b * l_list[%d]\n", j);                              // --------------- FOR DEBUG--------------- 
      // printf("%s * %s\n", buf1, buf2);                            // --------------- FOR DEBUG--------------- 
      Lint x = multiplication(b, l_list[j]);                      // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x]


      
      if(Lint_compare(x, remain[i + 1]) == LEFT) {
        ans.digit[i] = j - 1;
        // printf("ans.digit[%d] = %d\n", i, ans.digit[i]);          // --------------- FOR DEBUG--------------- 
        break;
      }
      free(x.digit);                                              // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1]]
    }                                                           // endfor2

    // x_result : ansのi桁目 * b
    Lint x_result = multiplication(b, l_list[ans.digit[i]]);      // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result]
    // lint_to_string(x_result, buf1);                               // --------------- FOR DEBUG--------------- 
    // printf("x_result = %s\n", buf1);                              // --------------- FOR DEBUG--------------- 

    if(i >= 1) {                                                // if
      // remainからx_resultを引く
      Lint x1 = subtraction(remain[i + 1], x_result);             // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result x1]
      // lint_to_string(x1, buf1);                                   // --------------- FOR DEBUG--------------- 
      // printf("x1 = %s\n", buf1);                                  // --------------- FOR DEBUG--------------- 

      // 10倍してゼロを一つ増やす
      Lint x2 = Lint_pow_10(x1, 1);                               // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result x1 x2]
      // aを一桁下ろす
      remain[i] = addition(x2, l_list[a.digit[i - 1]]);           // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result x1 x2 remain[i]]
      free(x1.digit);                                             // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result x2 remain[i]]
      free(x2.digit);                                             // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result remain[i]]
    } else {                                                    // else
      // 余りになる
      remain[i] = subtraction(remain[i + 1], x_result);           // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] x_result remain[i]]
    }                                                           // endif
    free(x_result.digit);                                         // freeスタック [(l_list[0] - l_list[9]) remain remain[i + 1] remain[i]]
    free(remain[i + 1].digit);                                    // freeスタック [(l_list[0] - l_list[9]) remain remain[i]]
  }                                                           // endfor1
  free(remain[0].digit);                                          // freeスタック [(l_list[0] - l_list[9]) remain]
  free(remain);                                                   // freeスタック [(l_list[0] - l_list[9])]
  for(int i = 0; i < 10; i++)
    free(l_list[i].digit);                                        // freeスタック []
  
  return ans;
}