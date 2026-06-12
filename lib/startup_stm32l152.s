.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler

_estack = 0x20008000

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  ldr   sp, =_estack
  bl    main
1:
  b     1b

.section .isr_vector,"a",%progbits
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  