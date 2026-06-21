#include "stm32l1xx.h"

#define RTC_WRITE_KEY1        0xCAU
#define RTC_WRITE_KEY2        0x53U
#define RTC_WRITE_LOCK        0xFFU

static uint32_t elapsed_seconds = 0;
static uint32_t elapsed_hours = 0;
static uint32_t elapsed_days = 0;

#define SLEEPING_MODE 0
#define SHOWTIME_MODE 1
#define RESET_MODE 2

void GPIO_Init(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    GPIOA->MODER &= ~3U;
    GPIOB->MODER &= ~(3U << 14);
    GPIOB->MODER |= (1U << 14);
}

void RTC_Init_LSI (void) {

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	PWR->CR |= (1U << 8);
	
	RCC->CSR |= 1U;
	
	while ((RCC->CSR & (1U << 1)) == 0) {}

    RCC->CSR &= ~(3U << 16);
    RCC->CSR |= (2U << 16) | (1U << 22);

	RTC->WPR = RTC_WRITE_KEY1; 
    RTC->WPR = RTC_WRITE_KEY2;

    RTC->ISR |= (1U << 7);
    
    while ((RTC->ISR & (1U << 6)) == 0U) {}
    
    RTC->PRER = (0x007CU << 16) | 0x0127U;

    RTC->ISR &= ~(1U << 7);

    RTC->WPR = RTC_WRITE_LOCK;
}

void Set_PIN_TO_AF11(GPIO_TypeDef *GPIOx, uint32_t pin) {

	GPIOx->MODER &= ~(3U << (pin * 2U));
	GPIOx->MODER |= (2U << (pin * 2U));

	if(pin < 8U) {
	GPIOx->AFRL &= ~(15U << (pin * 4U));
	GPIOx->AFRL |= (11U << (pin * 4U));
	}
	else {
	GPIOx->AFRH &= ~(15U << ((pin-8U) * 4U));
	GPIOx->AFRH |= (11U << ((pin-8U) * 4U));
	}
}

void LCD_GPIO_Init(void) {

	Set_PIN_TO_AF11(GPIOA, 1U); // PA1
	Set_PIN_TO_AF11(GPIOA, 2U); // PA2
	Set_PIN_TO_AF11(GPIOA, 3U); // PA3
	Set_PIN_TO_AF11(GPIOA, 8U); // PA8
	Set_PIN_TO_AF11(GPIOA, 9U); // PA9
	Set_PIN_TO_AF11(GPIOA, 10U); // PA10
	Set_PIN_TO_AF11(GPIOA, 15U); // PA15
	
	Set_PIN_TO_AF11(GPIOB, 3U); // PB3
	Set_PIN_TO_AF11(GPIOB, 4U); // PB4
	Set_PIN_TO_AF11(GPIOB, 5U); // PB5
	Set_PIN_TO_AF11(GPIOB, 8U); // PB8
	Set_PIN_TO_AF11(GPIOB, 9U); // PB9
	Set_PIN_TO_AF11(GPIOB, 10U); // PB10
	Set_PIN_TO_AF11(GPIOB, 11U); // PB11
	Set_PIN_TO_AF11(GPIOB, 12U); // PB12
	Set_PIN_TO_AF11(GPIOB, 13U); // PB13
	Set_PIN_TO_AF11(GPIOB, 14U); // PB14
	Set_PIN_TO_AF11(GPIOB, 15U); // PB15
	
	Set_PIN_TO_AF11(GPIOC, 0U); // PC0
	Set_PIN_TO_AF11(GPIOC, 1U); // PC1
	Set_PIN_TO_AF11(GPIOC, 2U); // PC2
	Set_PIN_TO_AF11(GPIOC, 3U); // PC3
	Set_PIN_TO_AF11(GPIOC, 6U); // PC6
	Set_PIN_TO_AF11(GPIOC, 7U); // PC7
	Set_PIN_TO_AF11(GPIOC, 8U); // PC8
	Set_PIN_TO_AF11(GPIOC, 9U); // PC9
	Set_PIN_TO_AF11(GPIOC, 10U); // PC10
	Set_PIN_TO_AF11(GPIOC, 11U); // PC11
	
}

void LCD_enable(void) {

	    RCC->APB1ENR |= RCC_APB1ENR_LCDEN;
	    
	    LCD->CR &= ~(3U << 5);	// BIAS 1/3
	    LCD->CR |= (2U << 5);
	    
	    LCD->CR &= ~(7U << 2);	// DUTY 1/4
	    LCD->CR |= (3U << 2);
	    
	    LCD->CR &= ~(1U << 1);	// VSEL 0
	    
	    while ((LCD->SR & (1U << 5)) == 0U) {}
	    
	    LCD->FCR &= ~(15U << 22);	// PS /32
	    LCD->FCR |= (5U << 22);
	    
	    LCD->FCR &= ~(15U << 18);	// DIV /16
	    
	    LCD->FCR &= ~(7U << 10);	// CC max
		LCD->FCR |=  (7U << 10);
	    
	    LCD->CR |= 1U;	// LCDEN 1
	    
}

void LCD_Clear(void) {
    for (uint32_t i = 0; i < 16U; i++) {
        LCD->RAM[i] = 0U;
    }
}

void LCD_Update(void) {
    LCD->SR |= (1U << 2);                  // UDR
    while ((LCD->SR & (1U << 3)) == 0U) {} // wait UDD
    LCD->CLR |= (1U << 3);                 // clear UDD
}

void LCD_SetSegment(uint32_t seg, uint32_t com) {

    uint32_t ram_index = com * 2U;

    LCD->RAM[ram_index] |= (1U << seg);
}




int main(void) {

	GPIO_Init();
    RTC_Init_LSI ();
	LCD_GPIO_Init();
	LCD_enable();
	GPIOB->ODR &= ~(1U << 7);
	
    
    uint32_t prev_sec = RTC->TR & 0x7FU;
	uint32_t seg = 0U;
	uint32_t com = 0U;
	uint32_t led_state = 0U;

	LCD_Clear();
	LCD_SetSegment(seg, com);
	LCD_Update();

while (1) {
    uint32_t cur_sec = RTC->TR & 0x7FU;

    if (cur_sec != prev_sec) {
        prev_sec = cur_sec;

        LCD_Clear();
        LCD_SetSegment(seg, com);
        LCD_Update();

        if (led_state == 0U) {
            GPIOB->ODR |= (1U << 7);
            led_state = 1U;
        } else {
            GPIOB->ODR &= ~(1U << 7);
            led_state = 0U;
        }

        seg++;

        if (seg >= 24U) {
            seg = 0U;
            com++;
        }

        if (com >= 4U) {
            com = 0U;
        }
    }
}

    
    return 0;
}