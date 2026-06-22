#ifndef STM32L152_MINI_H
#define STM32L152_MINI_H

#include <stdint.h>

#define __IO volatile

#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB1PERIPH_BASE		  (PERIPH_BASE + 0x00000000)
#define APB2PERIPH_BASE		  (PERIPH_BASE + 0x00010000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000)
#define IOPPERIPH_BASE        (PERIPH_BASE + 0x00020000)

#define RCC_BASE              (AHBPERIPH_BASE + 0x00003800)
#define GPIOA_BASE            (IOPPERIPH_BASE + 0x00000000)
#define GPIOB_BASE            (IOPPERIPH_BASE + 0x00000400)
#define GPIOC_BASE            (IOPPERIPH_BASE + 0x00000800)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x00000000)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x00000400)
#define PWR_BASE              (APB1PERIPH_BASE + 0x00007000)
#define RTC_BASE              (APB1PERIPH_BASE + 0x00002800)
#define LCD_BASE              (APB1PERIPH_BASE + 0x00002400)
#define NVIC_ISER0            (*((__IO uint32_t *)0xE000E100))
#define SCB_SCR               (*((__IO uint32_t *)0xE000ED10))

#define EXTI_LINE_BUTTON      1U
#define EXTI_LINE_RTC_WAKEUP  (1U << 20)

#define NVIC_IRQ_RTC_WKUP     3U
#define NVIC_IRQ_EXTI0        6U

#define RTC_ISR_WUTWF         (1U << 2)
#define RTC_ISR_WUTF          (1U << 10)
#define RTC_CR_WUTE           (1U << 10)
#define RTC_CR_WUTIE          (1U << 14)
#define RTC_CR_WUCKSEL_MASK   7U
#define RTC_CR_WUCKSEL_1HZ    4U

typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
} GPIO_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t ICSCR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHBRSTR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t AHBLPENR;
    __IO uint32_t APB2LPENR;
    __IO uint32_t APB1LPENR;
    __IO uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    __IO uint32_t CR; 
    __IO uint32_t CSR; 
} PWR_TypeDef;

typedef struct {
    __IO uint32_t TR; 
    __IO uint32_t DR;
    __IO uint32_t CR;
    __IO uint32_t ISR;
    __IO uint32_t PRER;
    __IO uint32_t WUTR;
    __IO uint32_t CALIBR;
    __IO uint32_t ALRMAR;
    __IO uint32_t ALRMBR;
    __IO uint32_t WPR;
} RTC_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t FCR;
    __IO uint32_t SR;
    __IO uint32_t CLR;
    uint32_t RESERVED0; // 0х10 (nothing)
    __IO uint32_t RAM[16]; // 0x14
} LCD_TypeDef;

typedef struct {
    __IO uint32_t MEMRMP;
    __IO uint32_t PMC;
    __IO uint32_t EXTICR[4];
} SYSCFG_TypeDef;

typedef struct {
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
} EXTI_TypeDef;

#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC           	  ((GPIO_TypeDef *) GPIOC_BASE)
#define SYSCFG                ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define EXTI                  ((EXTI_TypeDef *) EXTI_BASE)
#define PWR                   ((PWR_TypeDef *) PWR_BASE)
#define RTC                   ((RTC_TypeDef *) RTC_BASE)
#define LCD                   ((LCD_TypeDef *) LCD_BASE)

#define RCC_AHBENR_GPIOAEN    ((uint32_t)0x00000001)
#define RCC_AHBENR_GPIOBEN    ((uint32_t)0x00000002)
#define RCC_AHBENR_GPIOCEN    ((uint32_t)0x00000004)

#define RCC_APB1ENR_LCDEN     ((uint32_t)0x00000200)
#define RCC_APB1ENR_PWREN     ((uint32_t)0x10000000)
#define RCC_APB2ENR_SYSCFGEN  ((uint32_t)0x00000001)

#define RCC_CSR_LSION         ((uint32_t)0x00000001)
#define RCC_CSR_LSIRDY        ((uint32_t)0x00000002)
#define RCC_CSR_RTCSEL_MASK   ((uint32_t)0x00030000)
#define RCC_CSR_RTCSEL_LSI    ((uint32_t)0x00020000)
#define RCC_CSR_RTCEN         ((uint32_t)0x00400000)

#define PWR_CR_DBP            ((uint32_t)0x00000100)

#define RTC_ISR_INITF         ((uint32_t)0x00000040)
#define RTC_ISR_INIT          ((uint32_t)0x00000080)

#endif