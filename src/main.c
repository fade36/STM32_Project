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

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

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

int main(void) {

	GPIO_Init();
    RTC_Init_LSI ();
    
    GPIOB->ODR &= ~(1U << 7);
    uint32_t prev_sec = RTC->TR & 0x7FU;
    uint32_t led_state = 0;
    uint32_t mode = SLEEPING_MODE;
    uint32_t prev_button = GPIOA->IDR & 1U;
    uint32_t check30 = 0;
    
    
    while (1) {
    	uint32_t cur_button = GPIOA->IDR & 1U;
    	uint32_t cur_sec = RTC->TR & 0x7FU;
    	
    	if(prev_sec != cur_sec) {
    			if(mode != SLEEPING_MODE) {
    				check30++;
    			}
    			elapsed_seconds++;
    			prev_sec = cur_sec;
    			
    	if (mode == SHOWTIME_MODE) {
    				if(led_state == 0) {
    			GPIOB->ODR |= (1U << 7);
    			led_state = 1;
    		}
    	else {
    			GPIOB->ODR &= ~(1U << 7);
    			led_state = 0;
    		}
    			}   			  
    	if(elapsed_seconds >= 3600) {
    		elapsed_hours++;
    		elapsed_seconds = 0;
    	}
    	if(elapsed_hours >= 24) {
    		elapsed_days++;
    		elapsed_hours = 0;
    	}
    	}
    	
    	if(mode != SLEEPING_MODE && check30 > 30) {
    		led_state = 0;
    		GPIOB->ODR &= ~(1U << 7);
    		mode = SLEEPING_MODE;
    		check30 = 0;
    	}
    	
    	if ((prev_button == 0) && (cur_button != 0)) {
    	
    		if(mode == SLEEPING_MODE) {
    			check30 = 0;
    			mode = SHOWTIME_MODE;
    		}
    		
    		else if(mode == SHOWTIME_MODE && check30 <= 30) {
    		check30 = 0;
    		mode = RESET_MODE;
    		GPIOB->ODR |= (1U << 7);
    		}
    		
    		else if (mode == RESET_MODE && check30 <= 30) {
    		check30 = 0;
    		mode = SLEEPING_MODE;
    		elapsed_seconds = 0;
    		elapsed_hours = 0;
    		elapsed_days = 0;
    		led_state = 0;
    		GPIOB->ODR &= ~(1U << 7); 
    		}
    		
    	}
    	prev_button = cur_button;
    	
    }
    
    
    return 0;
}