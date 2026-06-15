#include "stm32l1xx.h"

int main(void) {

RCC->AHBENR |= 3U;

GPIOA->MODER &= ~3U;

GPIOB->MODER &= ~(3U << 14);

GPIOB->MODER |= (1U << 14);

    while(1) {
    if (GPIOA->IDR & 1U) {
		GPIOB->ODR |= (1U << 7);
	}
	else {
	}
	GPIOB->ODR &= ~(1U << 7);
    }
    return 0;
}
