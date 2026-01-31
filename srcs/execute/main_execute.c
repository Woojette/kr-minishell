#include "model.h"
#include <stdio.h>

// echo hello | cat -e > out1 > out2

int main(void)
{
  t_cmd_family* cmd_family = build_echo_cat_family();
  printf("finish cmd family\n");

  // 1. 파이프 작동 시키기
  // 2. 리다이랙션 중 out, append, in 작동시키기
  // 3. heredoc
  
  (void)(cmd_family);
}