#include "stm32l1xx.h"

int main(void) {
    RCC->AHBENR |= 3U;

    GPIOA->MODER &= ~3U;

    GPIOB->MODER &= ~(3U << 14);
    GPIOB->MODER |= (1U << 14);

    RCC->APB1ENR |= (1U << 28);
    for (volatile int i = 0; i < 2000; i++) {}

    PWR->CR |= (1U << 8);

    RCC->CSR |= 1U;

    while ((RCC->CSR & (1U << 1)) == 0) {}

    RCC->CSR &= ~(3U << 16);
    RCC->CSR |= (2U << 16) | (1U << 22);
    for (volatile int i = 0; i < 2000; i++) {}

    RTC->WPR = 0xCA; 
    RTC->WPR = 0x53;

    RTC->ISR |= (1U << 7); 
    for (volatile int i = 0; i < 2000; i++) {}

	RTC->PRER = (0x007CU << 16) | 0x0127U;

    RTC->ISR &= ~(1U << 7);

    RTC->WPR = 0xFF;
    
    

    while(1) {
        if (GPIOA->IDR & 1U) {
            GPIOB->ODR |= (1U << 7);
        } else {
            GPIOB->ODR &= ~(1U << 7);
        }
    }
    
    return 0;
}