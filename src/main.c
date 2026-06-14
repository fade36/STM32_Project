#include "stm32l1xx.h"

int main(void) {

RCC->AHBENR |= (1U << 1);
GPIOB->MODER &= ~(3U << 14);
GPIOB->MODER |= (1U << 14);
GPIOB->ODR &= ~(1U << 7);
    while(1) {
    }
    return 0;
}
