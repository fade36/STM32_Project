#include "stm32l1xx.h"

#define RTC_WRITE_KEY1        0xCAU
#define RTC_WRITE_KEY2        0x53U
#define RTC_WRITE_LOCK        0xFFU

static uint32_t elapsed_seconds = 0;
static uint32_t elapsed_hours = 0;
static uint32_t elapsed_days = 0;
static volatile uint32_t button_pressed_event = 0U;

#define SLEEPING_MODE 0U
#define SHOWTIME_MODE 1U
#define RESET_MODE 2U

void GPIO_Init(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    GPIOA->MODER &= ~3U;
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

void NVIC_EnableIRQ(uint32_t irq_number) {

    NVIC_ISER0 = (1U << irq_number);
}

void Button_EXTI_Init(void) {

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[0] &= ~0xFU;

    EXTI->IMR |= EXTI_LINE_BUTTON;
    EXTI->RTSR |= EXTI_LINE_BUTTON;
    EXTI->FTSR &= ~EXTI_LINE_BUTTON;
    EXTI->PR = EXTI_LINE_BUTTON;

    NVIC_EnableIRQ(NVIC_IRQ_EXTI0);
}

void RTC_Wakeup_Init(void) {

    RTC->WPR = RTC_WRITE_KEY1;
    RTC->WPR = RTC_WRITE_KEY2;

    RTC->CR &= ~RTC_CR_WUTE;

    while ((RTC->ISR & RTC_ISR_WUTWF) == 0U) {}

    RTC->WUTR = 0U;
    RTC->CR &= ~RTC_CR_WUCKSEL_MASK;
    RTC->CR |= RTC_CR_WUCKSEL_1HZ;
    RTC->ISR &= ~RTC_ISR_WUTF;
    RTC->CR |= RTC_CR_WUTIE | RTC_CR_WUTE;

    RTC->WPR = RTC_WRITE_LOCK;

    EXTI->IMR |= EXTI_LINE_RTC_WAKEUP;
    EXTI->RTSR |= EXTI_LINE_RTC_WAKEUP;
    EXTI->PR = EXTI_LINE_RTC_WAKEUP;

    NVIC_EnableIRQ(NVIC_IRQ_RTC_WKUP);
}

void EXTI0_IRQHandler(void) {

    if ((EXTI->PR & EXTI_LINE_BUTTON) != 0U) {
        EXTI->PR = EXTI_LINE_BUTTON;
        button_pressed_event = 1U;
    }
}

void RTC_WKUP_IRQHandler(void) {

    if ((RTC->ISR & RTC_ISR_WUTF) != 0U) {
        RTC->ISR &= ~RTC_ISR_WUTF;
    }

    EXTI->PR = EXTI_LINE_RTC_WAKEUP;
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
	    
	    LCD->CR |= (1U << 7);
	    
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
	//-------------------------------------------------------------
	// taken from official repository STM32L152 Discovery glass LCD
	// https://github.com/STMicroelectronics/STM32CubeL0/blob/master/Projects/NUCLEO-L053R8/Examples/LCD/LCD_SegmentsDrive/Src/stm32l152c_discovery_lcd.c
	
static const uint16_t LCD_NumberMap[10] = {
    0x5F00, // 0
    0x4200, // 1
    0xF500, // 2
    0x6700, // 3
    0xEA00, // 4
    0xAF00, // 5
    0xBF00, // 6
    0x4600, // 7
    0xFF00, // 8
    0xEF00  // 9
};

	#define LCD_CHAR_D 0x4714
	#define LCD_CHAR_H 0xFA00
	#define LCD_CHAR_R 0xFC01
	#define LCD_CHAR_S 0xAF00
	#define LCD_CHAR_T 0x0414

static const uint8_t LCD_PositionSegments[6][4] = {
    {0U,  1U, 28U, 29U}, // position 1
    {2U,  7U, 26U, 27U}, // position 2
    {8U,  9U, 24U, 25U}, // position 3
    {10U, 11U, 20U, 21U}, // position 4
    {12U, 13U, 18U, 19U}, // position 5
    {14U, 15U, 17U, 16U}  // position 6
};

	//-------------------------------------------------------------
	
uint16_t LCD_GetCharMap(char ch){
    if (ch >= '0' && ch <= '9') {
        return LCD_NumberMap[ch - '0'];
    }

    if (ch == 'D' || ch == 'd') return LCD_CHAR_D;
    if (ch == 'H' || ch == 'h') return LCD_CHAR_H;
    if (ch == 'R' || ch == 'r') return LCD_CHAR_R;
    if (ch == 'S' || ch == 's') return LCD_CHAR_S;
    if (ch == 'T' || ch == 't') return LCD_CHAR_T;

    return 0x0000; // space / unknown
}

void LCD_PutChar(uint32_t position, char ch){
    uint16_t map = LCD_GetCharMap(ch);

    if (position >= 6U) {
        return;
    }

    for (uint32_t com = 0U; com < 4U; com++) {
        uint32_t shift = 12U - (com * 4U);
        uint32_t nibble = (map >> shift) & 0xFU;

        for (uint32_t bit = 0U; bit < 4U; bit++) {
            if (nibble & (1U << bit)) {
                LCD_SetSegment(LCD_PositionSegments[position][bit], com);
            }
        }
    }
}

void LCD_Print6(const char *text){
    LCD_Clear();

    for (uint32_t i = 0U; i < 6U; i++) {
        if (text[i] == '\0') {
            break;
        }

        LCD_PutChar(i, text[i]);
    }

    LCD_Update();
}

void LCD_ShowTime(uint32_t days, uint32_t hours){

    char text[7];

    if (days > 99U) {
        days = 99U;
    }

    if (hours > 23U) {
        hours = 23U;
    }

    text[0] = 'D';
    text[1] = (char)('0' + (days / 10U));
    text[2] = (char)('0' + (days % 10U));
    text[3] = 'H';
    text[4] = (char)('0' + (hours / 10U));
    text[5] = (char)('0' + (hours % 10U));
    text[6] = '\0';

    LCD_Print6(text);
}

void LCD_ShowResetQuestion(void){
    LCD_Print6("RST   ");
}

void Delay(volatile uint32_t delay){
    while (delay != 0U) {
        delay--;
    }
}

void ElapsedTime_Tick(void){
    elapsed_seconds++;

    if (elapsed_seconds >= 20U) {
        elapsed_seconds = 0U;
        elapsed_hours++;

        if (elapsed_hours >= 24U) {
            elapsed_hours = 0U;
            elapsed_days++;

            if (elapsed_days > 99U) {
                elapsed_days = 99U;
            }
        }
    }
}

void ElapsedTime_Reset(void){
    elapsed_seconds = 0U;
    elapsed_hours = 0U;
    elapsed_days = 0U;
}

void Enter_Sleep_Mode(void) {

    SCB_SCR &= ~(1U << 2);
    __asm volatile ("wfi");
}

int main(void) {

	GPIO_Init();
    RTC_Init_LSI ();
	LCD_GPIO_Init();
	LCD_enable();
	Button_EXTI_Init();
    RTC_Wakeup_Init();
	
    uint32_t prev_sec = RTC->TR & 0x7FU;
    uint32_t mode = SLEEPING_MODE;
    uint32_t mode_seconds = 0U;

    LCD_Clear();
    LCD_Update();

    while (1) {
        uint32_t cur_sec = RTC->TR & 0x7FU;

        if (cur_sec != prev_sec) {
            uint32_t old_hours = elapsed_hours;
            uint32_t old_days = elapsed_days;

            prev_sec = cur_sec;
            ElapsedTime_Tick();

            if (mode != SLEEPING_MODE) {
                mode_seconds++;

                if (mode_seconds >= 30U) {
                    mode = SLEEPING_MODE;
                    mode_seconds = 0U;
                    LCD_Clear();
                    LCD_Update();
                }
            }

            if ((mode == SHOWTIME_MODE) &&
                ((old_hours != elapsed_hours) || (old_days != elapsed_days))) {
                LCD_ShowTime(elapsed_days, elapsed_hours);
            }
        }

        if (button_pressed_event != 0U) {
        
            button_pressed_event = 0U;

            if (mode == SLEEPING_MODE) {
                mode = SHOWTIME_MODE;
                mode_seconds = 0U;
                LCD_ShowTime(elapsed_days, elapsed_hours);
            } else if (mode == SHOWTIME_MODE) {
                mode = RESET_MODE;
                mode_seconds = 0U;
                LCD_ShowResetQuestion();
            } else {
                ElapsedTime_Reset();
                mode = SLEEPING_MODE;
                mode_seconds = 0U;
                LCD_Clear();
                LCD_Update();
            }
            Delay(30000U);
            button_pressed_event = 0U;
            EXTI->PR = EXTI_LINE_BUTTON;
        }
        
		
        if (mode == SLEEPING_MODE) {
            Enter_Sleep_Mode();
        }
    }
}