//Program  : Sinus -- MSP430 PWM Sine
//Engineer : Orhan YILMAZ
//Web      : www.mafgom.com
//
//Örnekler Linux ortamında Octave GNU
//programı ile PWM in %25 ile %75
//arasına denk düşmek üzere oluşturuldu.
//////////////////////////////////////////
 
#include<msp430g2452.h>
 
unsigned char Wave[]=
{
134, 140, 147, 152, 158, 164, 169, 173,
177, 181, 184, 187, 189, 191, 192, 192,
192, 191, 189, 187, 184, 181, 177, 173,
169, 164, 158, 152, 147, 140, 134, 128,
122, 116, 109, 104, 98, 92, 87, 83, 79,
75, 72, 69, 67, 65, 64, 64, 64, 65, 67,
69, 72, 75, 79, 83, 87, 92, 98, 104, 109,
116, 122, 128
};
 
unsigned char T = 0;
unsigned char freq = 0;
 
void main()
{
 
	WDTCTL = WDTPW + WDTHOLD;
 
	//16 Mhz
	DCOCTL = 0x60;
	BCSCTL1 = 0x8F;
 
	//Giriş - Çıkış bitleri
	P1DIR |= 0x01;
	P1OUT = 0x00;
	P1IE |= 0x08; //P1.3 portuna Kesme Aktif et..
 
		//Clk kaynağı,Ön ölçekleyici,Mod seçimi..
	TACCTL0 = CCIE + OUTMOD_6 + OUT;
	TACCTL1 = CCIE + OUTMOD_6 + OUT;
	TACCR1 = Wave[T]; //PWM başlangıç duty cycle değeri
	TACCR0 = 256;
 
	//SMCLK = 16MHz
	TACTL = TASSEL_2 + ID_1 + MC_1 + TAIE + TACLR;
 
	_BIS_SR(GIE);//Kesme izinlerini ver
 
	while(1) //Sonsuz döngü.. (Kesmeleri bekle!)
	;
}
 
#pragma vector = TIMER0_A0_VECTOR
interrupt void ta0kesmesi()
{
	if(TAIV == 10)
	{
		P1OUT |= 0x01;
		TACCR1 = Wave[T];
		T ++;
		T = (T+freq) % 64;
	}
}
 
#pragma vector = TIMER0_A1_VECTOR
interrupt void ta1kesmesi()
{
	if(TAIV == 2)
	{
		P1OUT &= ~0x01;
	}
}
 
#pragma vector = PORT1_VECTOR //Buton kesmesi. Frekansi buton ile değiştirmek için
interrupt void P1kesmesi()
{
	freq ++;
	freq %= 6;
	P1IFG &= ~0x08;
}
