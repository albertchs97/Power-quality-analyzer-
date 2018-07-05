
#include "io430.h"


char buffer[1000]; // buffer for AD
int temp;
unsigned int j = 0; // buffer[CHANNEL-j]
unsigned int k = 0; // buffer[k], k<2
unsigned int ready =0; // ready for transfer AD to pc from MSP430

void ADC_init()
{
    P6SEL |= BIT0 ;     //P6.0 模拟输入
    
    ADC12CTL1 |= SHS0 ; //timer 触发
    ADC12IE |= BIT0;    //P6.0 使能
    
  ADC12CTL0 |= ADC12ON + ENC; //给内核供电，允许转换
 //ADC12CTL0 |= ADC12IE; //允许ADCMEM中断
}

void Timer0_init(){
    
    //P1.6 output of timer in compare mode 
    P1SEL |= BIT2;
    P1DIR |= BIT2;
    TACCR0 = 15;  //PERIOD= 1/50000 < (TACCR0 +1 )*(1/32767) = 1/50000
    TACCR1 = 11;   //dUTY RATIO = 25% => TA1CCR1 = 7 * (1-0.25)
    TACCTL1 |= OUTMOD_2;   // out1_mode => toggle / reset
    
    TACTL = TACLR + TASSEL0 + MC0 ;  //clr TAR , ACLK be the clock of timer , up_mode
    TACTL &= ~TAIFG;
    //TACCTL1 |= CCIE;   //允许定时器CCR1中断 
  
}

void USCIA0_init(){
    //P3.6 => TXD1 funtion
    //P3.7 => RXD1 funtion
    // P3OUT &= ~(BIT6 + BIT7);
    P3DIR   |= BIT6;
    P3SEL   |= BIT6  ;   //activate USC1_A1 TXC/RXD
    ME2     |= UTXE1 ;   //Module Enable RXD1 & TXD1
    U1CTL   |= CHAR;            //chosse for 8-digit character length
    U1TCTL  |= SSEL0;           //CHOOSE ACLK AS CLK SOURCE CLK
    U1BR0   =  0x03;            //BODE RATE AS 9600
    U1BR1   =  0x00;            //
    U1MCTL  =  0x4A;            //BODE RATE MODULATION
    U1CTL   &= ~SWRST;          //RESET RELEASED FOR OPERATION
 //   IE2 |= UTXIE1;    
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Timer0_init();        //定时器初始化
  ADC_init();           //AD转换初始化
  USCIA0_init();        //串口通信初始化
  __enable_interrupt(); //开总控位
  P2DIR |= BIT7;
  while(1){
    if (j == 1000)
    {
      ADC12IE &= ~BIT0;
      for (int i = 0; i != 1000; ++i)
      {
        while (IFG2 & BIT5 == 0) ;
          U1TXBUF = buffer[i];
      }
      j = 0;
      ADC12IE |= BIT0;
    }
  }
  return 0;
}


// ADMEM Interrupt
#pragma vector = ADC12_VECTOR
__interrupt void ADC10_isr(){
  if (j != 1000)
  {
    temp = ADC12MEM0;
    buffer[j]= (char)temp;
    buffer[j + 1] = (char)(temp >> 8);
    j += 2;
    ADC12CTL0 &= ~EN
    ADC12CTL0 |= ENC;
  }
}
