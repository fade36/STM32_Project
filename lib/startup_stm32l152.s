.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  ldr   sp, =_estack

  ldr   r0, =_sidata
  ldr   r1, =_sdata
  ldr   r2, =_edata
CopyData:
  cmp   r1, r2
  bcs   ZeroBss
  ldr   r3, [r0], #4
  str   r3, [r1], #4
  b     CopyData

ZeroBss:
  ldr   r1, =_sbss
  ldr   r2, =_ebss
  movs  r3, #0
FillBss:
  cmp   r1, r2
  bcs   CallMain
  str   r3, [r1], #4
  b     FillBss

CallMain:
  bl    main
1:
  b     1b
.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler
.type Default_Handler, %function
Default_Handler:
  b     Default_Handler
.size Default_Handler, .-Default_Handler

.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.section .isr_vector,"a",%progbits
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  