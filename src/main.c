#include "main.h"

int main(void)
{
	init_led();
	init_button();
	SysTick_Config(SystemCoreClock/1000);		// This function initialises the system tick timer and its interrupt and start the system tick timer.
												// Counter is in free running mode to generate periodical interrupts.
												// F_CPU 8MHz

	while (1)
	{
		//LED_toggle();
		//delay(1);
		// передача через USART IDLE
		click();
	}
}

void click(void){
	if ( (GPIOB->IDR & GPIO_IDR_IDR12) == GPIO_IDR_IDR12 )
		delay_with_interrupt(100);			// псевдоизбавление от дребезга, хотел через NVIC_EnableIRQ
											// чет не получилось
	if ( (GPIOB->IDR & GPIO_IDR_IDR12) == GPIO_IDR_IDR12 ) {
		//LED_toggle();						// если все же кнопка на самом деле нажата
		uint32_t start = ticks_delay;		// сохраняем время вхождения для отсчета от 1 - 3 сек
		while(GPIOB->IDR & GPIO_IDR_IDR12){	// пока есть высокий уровень
			if( (ticks_delay-start)>1000 && (ticks_delay-start)<3000 )
				GPIOB->ODR	|=	GPIO_ODR_ODR6;					// тут должна быть передача через UART hold
																// но я такой тормоз который еле еле смог
																// на костылях прерывание создать
			else
				GPIOB->ODR	&=	~GPIO_ODR_ODR6;	// тут должна быть передача через UART click
		}
	}
}

void LED_toggle(void){
	GPIOB->ODR	^=	GPIO_ODR_ODR6;
}

void delay_with_interrupt(const uint32_t ms){
	uint32_t start = ticks_delay;			// сохраняем время вхождения
	while( (ticks_delay-start)<ms );	// находимся в цикле, пока не насчитаем заданное мс
}

//	таймер которого понял как настраивается, но не понял почему время не совпадает.
//	По логике, если я в SysTick->LOAD загружаю SystemCoreClock, которые через define указан
//	как 72 MHz, то если оставлю в LOAD все 72MHz, она же должна ровно 1 секунду выдавать
//	но когда блинкую, получается чаще.
void delay(const uint32_t time) {
	if (time > SYSTICK_MAX_VALUE || time == 0)
		return;
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;				// TICKINT: 0 = counting down to zero does
    														// not assert the SysTick exception request
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;			// CLKSOURCE: 1 - processor clock.

    SysTick->LOAD = (SystemCoreClock-1);					// SYST_RVR
    														// Значение, загружаемое в регистр SYST_CVR,
    														// когда в процессе счёта он достигнет нуля

    SysTick->VAL = 0;										// The SYST_CVR register contains
    														// the current value of the SysTick counter.

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;				// Enables the counter: 1 = counter enabled.
    														// (уменьшение счётчика)

    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));	// COUNTFLAG - Показывает, дошёл ли счётчик до нуля
    														// с момента последнего считывания этого регистра
    														// 1 - если дошел, и выходит из цикла

    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;			// Сбрасываем COUNTFLAG вручную,

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;				// Enables the counter: 0 = counter disabled.
}

void init_led(void) {

    // turn on GPIOA clocking

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // set gpio pin mode

    GPIOB->CRL &= ~GPIO_CRL_MODE6_0; // set as

    GPIOB->CRL |= GPIO_CRL_MODE6_1;  // output with 2 MHz speed

    GPIOB->CRL &= ~GPIO_CRL_CNF6;    // set as push-pull output

}

void init_button(void){
	//RCC->APB2ENR	|=	RCC_APB2ENR_IOPAEN;	// turn on GPIOA clocking

	GPIOB->CRH		&=	~GPIO_CRH_MODE12;	// 00: Input mode (reset state)

	GPIOB->CRH		|=	GPIO_CRH_CNF12_1;	// 10: Input with pull-up / pull-down
	GPIOB->CRH		&=	~GPIO_CRH_CNF12_0;


//	NVIC_EnableIRQ(EXTI9_5_IRQn);			// This function enables a device specific
											// interrupt in the NVIC interrupt controller.
											// External Line[9:5] Interrupts

//	EXTI->IMR  |= EXTI_IMR_MR5;				// IMR (Interrupt mask register)

//	EXTI->RTSR |= EXTI_RTSR_TR5;			// RTSR (Rising trigger selection register)
    										// 1: Rising trigger enabled (for Event and Interrupt) for input line.

//	EXTI->FTSR &= ~EXTI_FTSR_TR5;			// FTSR (Falling trigger selection register)
    										// 0: Falling trigger disabled (for Event and Interrupt) for input line*/
}
