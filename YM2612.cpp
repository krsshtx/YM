#include "YM2612.h"
#define NOP __asm__ __volatile__ ("nop\n\t")

YM2612::YM2612(Bus * bus)
{
    _bus = bus;
    GPIOA->regs->CRH = (GPIOA->regs->CRH & 0x0FF00FFF) | 0x30033000; //PA11-YM_A0, PA12-YM_WR, PA15-YM_RD OUTPUT
    GPIOB->regs->CRL = (GPIOB->regs->CRL & 0xFFF00FF0) | 0x00033003; //PB0-YM_A1, PB3-YM_CS, PB4-YM_IC OUTPUT

    GPIOB->regs->BSRR = (1U << 3) << (16 * 0); //_CS HIGH
    GPIOA->regs->BSRR = (1U << 15) << (16 * 0); //_RD HIGH
    GPIOA->regs->BSRR = (1U << 12) << (16 * 0); //_WR HIGH
    GPIOB->regs->BSRR = (1U << 4) << (16 * 0); //_IC HIGH
    GPIOA->regs->BSRR = (1U << 11) << (16 * 1); //_A0 LOW
    GPIOB->regs->BSRR = (1U << 0) << (16 * 1); //_A1 LOW
}

void YM2612::Reset()
{
    GPIOB->regs->BSRR = (1U << 4) << (16 * 0); //_IC HIGH
    delayMicroseconds(5);
    GPIOB->regs->BSRR = (1U << 4) << (16 * 1); //_IC LOW
    delayMicroseconds(5);
    GPIOB->regs->BSRR = (1U << 4) << (16 * 0); //_IC HIGH
    delayMicroseconds(5);
  //  GPIOB->regs->BSRR = (1U << 0) << (16 * 0); //_A1 HI
  //  delayMicroseconds(1);
  //  GPIOB->regs->BSRR = (1U << 4) << (16 * 0); //_IC HIGH
  //  delayMicroseconds(1);
  //  GPIOB->regs->BSRR = (1U << 4) << (16 * 1); //_IC LOW
  //  delayMicroseconds(1);
  //  GPIOB->regs->BSRR = (1U << 4) << (16 * 0); //_IC HIGH
    delayMicroseconds(100);
    


}
unsigned char YM2612::Read()
  {
    uint32_t rdata;
    uint32_t ymread;

    GPIOB->regs->ODR &= ~(0x0808); //_CS LOW
  //GPIOB->regs->BSRR = 0b0000000000000001 << 16; //_A1 PB0
    GPIOB->regs->ODR &= ~(0x1);
    GPIOA->regs->ODR &= ~(0x0800); //_A0 LOW
    NOP;
 // GPIOB->regs->ODR &= ~(0x0808); //_CS LOW
    GPIOA->regs->ODR &= ~(0x8000); //_RD LOW
 
    NOP;

    GPIOA->regs->CRL = (GPIOA->regs->CRL & 0xFFFFF000) | 0x00000833; //PA0, PA1, PA2 OUTPUT
    GPIOA->regs->ODR |= 0x4 ;

    ymread = 1;
    while (ymread > 0 )
      {

      rdata= GPIOA->regs->IDR;

      ymread = ((rdata & 0x4) >>2);

        //ymread=ym2612.Read();
        //if (itr>1) {
        // Serial.print("itr ");Serial.print(itr);
        // Serial.print(" ymread ");Serial.println(ymread);
        //}
       if (ymread > 0) {
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;

        }
      }

    GPIOA->regs->CRL = (GPIOA->regs->CRL & 0xFFFFF000) | 0x00000333; //PA0, PA1, PA2 OUTPUT
    GPIOA->regs->ODR |= 0x8000;    //_RD HIGH
   
  //GPIOB->regs->ODR |= 0x0808;    //_CS HIGH
    GPIOA->regs->ODR |= 0x0800;    //_A0 HIGH
    GPIOB->regs->ODR |= 0x0808;    //_CS HIGH
  //  Serial.print((rdata>>7)&1);Serial.print((rdata>>6)&1);Serial.print((rdata>>5)&1);Serial.print((rdata>>4)&1);Serial.print((rdata>>3)&1);Serial.print((rdata>>2)&1);Serial.print((rdata>>1)&1);Serial.println(rdata&1);
    return ((rdata & 0x4) >>2);
   
  }
void YM2612::Send(unsigned char addr, unsigned char data, bool setA1) //0x52 = A1 LOW, 0x53 = A1 HIGH
{ 
  if (addr == 0x21) //Rocket Knight Adventures
  return;
  GPIOB->regs->ODR &= ~(0x0808); //_CS LOW
  switch(setA1)
  {
    case 0:
  //GPIOB->regs->BSRR = 0b0000000000000001 << 16; //_A1 PB0
    GPIOB->regs->ODR &= ~(0x1);
    break;
    case 1:
  //GPIOB->regs->BSRR = 0b0000000000000001 ; //_A1 PB0
    GPIOB->regs->ODR |= 0x1;
    break;
  }

    GPIOA->regs->ODR &= ~(0x0800); //_A0 LOW
    NOP;
  //GPIOB->regs->ODR &= ~(0x0808); //_CS LOW
        _bus->Write(addr);
    GPIOA->regs->ODR &= ~(0x1000); //_WR LOW
    NOP;

    GPIOA->regs->ODR |= 0x1000;    //_WR HIGH
//  GPIOB->regs->ODR |= 0x0808;    //_CS HIGH
    GPIOA->regs->ODR |= 0x0800;    //_A0 HIGH
 // GPIOB->regs->ODR &= ~(0x0808); //_CS LOW
        _bus->Write(data);

    GPIOA->regs->ODR &= ~(0x1000); //_WR LOW
    NOP;

    GPIOA->regs->ODR |= 0x1000;    //_WR HIGH
    GPIOB->regs->ODR |= 0x0808;    //_CS HIGH
   
}

