#include "common.h"
#include "include.h"
#include "init.h"
#include "PID.h"
#include "deal_data.h"
#include "function.h"
#include "SEEKFREE_OLED.h"


/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� PIT ��ʱʵ��
 */
void  main()
{
  Init_All();
  while(1)
  {  
     Search();
  }
}





