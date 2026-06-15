#ifndef STM32L152_MINI_H
#define STM32L152_MINI_H

#define __IO volatile

#define PERIPH_BASE           ((unsigned int)0x40000000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000)
#define IOPPERIPH_BASE        (PERIPH_BASE + 0x00020000)

#define RCC_BASE              (AHBPERIPH_BASE + 0x00003800)
#define GPIOA_BASE            (IOPPERIPH_BASE + 0x00000000)
#define GPIOB_BASE            (IOPPERIPH_BASE + 0x00000400)
#define SysTick_BASE          ((unsigned int)0xE000E010)
#define PWR_BASE              ((unsigned int)0x40007000)
#define RTC_BASE              ((unsigned int)0x40002400)

typedef struct {
    __IO unsigned int CTRL;
    __IO unsigned int LOAD;
    __IO unsigned int VAL;
    __IO unsigned int CALIB;
} SysTick_TypeDef;

typedef struct {
    __IO unsigned int MODER;
    __IO unsigned int OTYPER;
    __IO unsigned int OSPEEDR;
    __IO unsigned int PUPDR;
    __IO unsigned int IDR;
    __IO unsigned int ODR;
    __IO unsigned int BSRR;
} GPIO_TypeDef;

typedef struct {
    __IO unsigned int CR;
    __IO unsigned int ICSCR;
    __IO unsigned int CFGR;
    __IO unsigned int CIR;
    __IO unsigned int AHBRSTR;
    __IO unsigned int APB2RSTR;
    __IO unsigned int APB1RSTR;
    __IO unsigned int AHBENR;
    __IO unsigned int APB2ENR;
    __IO unsigned int APB1ENR;
    __IO unsigned int AHBLPENR;
    __IO unsigned int APB2LPENR;
    __IO unsigned int APB1LPENR;
    __IO unsigned int CSR;
} RCC_TypeDef;

typedef struct {
    __IO unsigned int CR; 
    __IO unsigned int CSR; 
} PWR_TypeDef;

typedef struct {
    __IO unsigned int TR; 
    __IO unsigned int DR;
    __IO unsigned int CR;
    __IO unsigned int ISR;
    __IO unsigned int PRER;
    __IO unsigned int WUTR;
    __IO unsigned int CALIBR;
    __IO unsigned int ALRMAR;
    __IO unsigned int ALRMBR;
    __IO unsigned int WPR;
} RTC_TypeDef;

#define SysTick               ((SysTick_TypeDef *) SysTick_BASE)
#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define PWR                   ((PWR_TypeDef *) PWR_BASE)
#define RTC                   ((RTC_TypeDef *) RTC_BASE)

#define RCC_AHBENR_GPIOAEN    ((unsigned int)0x00000001)
#define RCC_AHBENR_GPIOBEN    ((unsigned int)0x00000002)

#endif