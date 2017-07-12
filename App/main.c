#include "common.h"
#include "include.h"
#include "init.h"
#include "PID.h"
#include "deal_data.h"
#include "function.h"
#include "SEEKFREE_OLED.h"


/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 PIT 计时实验
 */
void  main()
{
  Init_All();
  while(1)
  {  
     Search();
  }
}





