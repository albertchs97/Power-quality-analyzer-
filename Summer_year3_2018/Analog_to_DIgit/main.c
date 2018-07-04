#include "io430.h"

const unsigned char LEDtab[16] ={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
const unsigned char seg[4] ={BIT3,BIT4,BIT5,BIT6};
unsigned int dig[4];
unsigned int num,buffer;



void ADC_init()
{
  P6SEL |= BIT0; //P1.0 模拟信号输入引脚
  //ADC10CTL1 |= INCH_0 + 
  /*RESET : 
    // ADC10CTL1
      P1.0 作为模拟信号输入引脚；
      通道选择 INCH_0 ;
      工作时钟 ADC10OSC 约5MHz ; ==> ADC10CLK = ADC10OSC;
      采样触发源：ADC10SC （软件控制）；
      转换方式：单通道单次 ；
      时钟源分频：/1;
    //ADC10CTL0
      参考电压：VR+ = AVcc , VR- = AVSS ;
      采样时间：4*ADC10CLK;
      最大采样率： 200ksps ;
      中断允许控制： 禁止中断
  */
  ADC12CTL0 |= ADC12ON + ENC; //给内核供电，允许转换
//  ADC10CTL0 |= ADC10IE;
}

void delay()
{
  int j;
  for(j=0;j<0xff;j++);
}
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  int i ; 
  ADC_init();
 // __enable_interrupt();
//  ADC10CTL0 |= ADC10SC  ;
  
    
  while(1)
  {
   ADC12CTL0 |= ADC12SC ; //软件启动采样
   while((BIT0 & ADC12IFG ) == 0 ) ;
    buffer = ADC12MEM0;
  }
  return 0;
}
