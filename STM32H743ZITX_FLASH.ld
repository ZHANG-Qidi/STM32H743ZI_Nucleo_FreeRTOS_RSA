/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**
**  Abstract    : Linker script for STM32H7 series
**                4K bytes BACKUPSRAM
**                8K bytes RAM_D3_Z8
**                8K bytes RAM_D3_Z7
**                8K bytes RAM_D3_Z6
**                8K bytes RAM_D3_Z5
**                8K bytes RAM_D3_Z4
**                8K bytes RAM_D3_Z3
**                8K bytes RAM_D3_Z2
**                8K bytes RAM_D3_Z1
**                64K bytes SRAM4
**                32K bytes SRAM3
**                128K bytes SRAM2
**                128K bytes SRAM1
**                512K bytes AXISRAM
**                128K bytes DTCMRAM
**                32K bytes SRAM3_ALIAS_M4
**                128K bytes SRAM2_ALIAS_M4
**                128K bytes SRAM1_ALIAS_M4
**                1024K bytes FLASH2
**                1024K bytes FLASH1
**                64K bytes ITCMRAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2019 STMicroelectronics.
** All rights reserved.
**
** This software component is licensed by ST under BSD 3-Clause license,
** the "License"; You may not use this file except in compliance with the
** License. You may obtain a copy of the License at:
**                        opensource.org/licenses/BSD-3-Clause
**
****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);	/* end of "DTCMRAM" Ram type memory */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200 ;      /* required amount of heap  */
_Min_Stack_Size = 0x400 ;     /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  RAM_D3      (xrw)    : ORIGIN = 0x38000000,   LENGTH = 64K
  RAM_D2      (xrw)    : ORIGIN = 0x30000000,   LENGTH = 288K
  RAM_D1      (xrw)    : ORIGIN = 0x24000000,   LENGTH = 512K
  FLASH       (rx)     : ORIGIN = 0x08000000,   LENGTH = 2048K
  BACKUPSRAM  (xrw)    : ORIGIN = 0x38800000,   LENGTH = 4K
  RAM_D3_Z8   (xrw)    : ORIGIN = 0x3800E000,   LENGTH = 8K
  RAM_D3_Z7   (xrw)    : ORIGIN = 0x3800C000,   LENGTH = 8K
  RAM_D3_Z6   (xrw)    : ORIGIN = 0x3800A000,   LENGTH = 8K
  RAM_D3_Z5   (xrw)    : ORIGIN = 0x38008000,   LENGTH = 8K
  RAM_D3_Z4   (xrw)    : ORIGIN = 0x38006000,   LENGTH = 8K
  RAM_D3_Z3   (xrw)    : ORIGIN = 0x38004000,   LENGTH = 8K
  RAM_D3_Z2   (xrw)    : ORIGIN = 0x38002000,   LENGTH = 8K
  RAM_D3_Z1   (xrw)    : ORIGIN = 0x38000000,   LENGTH = 8K
  SRAM4       (xrw)    : ORIGIN = 0x38000000,   LENGTH = 64K
  SRAM3       (xrw)    : ORIGIN = 0x30040000,   LENGTH = 32K
  SRAM2       (xrw)    : ORIGIN = 0x30020000,   LENGTH = 128K
  SRAM1       (xrw)    : ORIGIN = 0x30000000,   LENGTH = 128K
  AXISRAM     (xrw)    : ORIGIN = 0x24000000,   LENGTH = 512K
  DTCMRAM     (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
  SRAM3_ALIAS_M4 (xrw) : ORIGIN = 0x10040000,   LENGTH = 32K
  SRAM2_ALIAS_M4 (xrw) : ORIGIN = 0x10020000,   LENGTH = 128K
  SRAM1_ALIAS_M4 (xrw) : ORIGIN = 0x10000000,   LENGTH = 128K
  FLASH2      (rx)     : ORIGIN = 0x08100000,   LENGTH = 1024K
  FLASH1      (rx)     : ORIGIN = 0x08000000,   LENGTH = 1024K
  ITCMRAM     (xrw)    : ORIGIN = 0x00000000,   LENGTH = 64K
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH1 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH1

  /* The program code and other data goes into FLASH1 */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH1

  /* Constant data goes into FLASH1 */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH1

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH1
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH1

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH1

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH1

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH1

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);
  /* Initialized data sections goes into DTCMRAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> FLASH1
  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCMRAM

  /* User_heap_stack section, used to check that there is enough DTCMRAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCMRAM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .dma_buffer : /* Space before ':' is critical */
  {
    . = ALIGN(4);
    *(.dma_buffer)
    *(.dma_buffer.*)

    . = ALIGN(4);
  } >SRAM4

  .bdma_buffer (NOLOAD) : 
  {
    . = ALIGN(4);
 	*(.bdma_buffer)
 	*(.bdma_buffer.*)

    . = ALIGN(4);
  } >SRAM4 AT> FLASH1
  
  .dtcm :
  {
    . = ALIGN(4);
    *(.dtcm)
    *(.dtcm.*)

    . = ALIGN(4);
  } >DTCMRAM

  _siitcmram = LOADADDR(.itcm);
  .itcm :
  {
    . = ALIGN(4);
    _sitcmram = .;
    *(.itcm)
    *(.itcm.*)

    . = ALIGN(4);
    _eitcmram = .;
  } >ITCMRAM AT> FLASH1

  .lwip_sec (NOLOAD) : 
  {
    . = ALIGN(4);
    . = ABSOLUTE(0x30040000);
    *(.RxDecripSection) 
    
    . = ABSOLUTE(0x30040060);
    *(.TxDecripSection)
    
    . = ABSOLUTE(0x30040200);
    *(.RxArraySection) 

    . = ALIGN(4);
  } >RAM_D2 AT> FLASH1

  /* used by the startup to initialize data */
  _siRAM_D3_Z1 = LOADADDR(.RAM_D3_Z1);
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z1 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z1 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z1)           /* .data sections */
    *(.RAM_D3_Z1*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z1 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z1 AT> FLASH1
  
  /* used by the startup to initialize data */
  _siRAM_D3_Z2 = LOADADDR(.RAM_D3_Z2);
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z2 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z2 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z2)           /* .data sections */
    *(.RAM_D3_Z2*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z2 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z2 AT> FLASH1

  /* used by the startup to initialize data */
  _siRAM_D3_Z3 = LOADADDR(.RAM_D3_Z3);
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z3 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z3 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z3)           /* .data sections */
    *(.RAM_D3_Z3*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z3 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z3 AT> FLASH1

  /* used by the startup to initialize data */
  _siRAM_D3_Z4 = LOADADDR(.RAM_D3_Z4);
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z4 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z4 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z4)           /* .data sections */
    *(.RAM_D3_Z4*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z4 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z4 AT> FLASH1

  /* used by the startup to initialize data */
  _siRAM_D3_Z5 = LOADADDR(.RAM_D3_Z5); 
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z5 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z5 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z5)           /* .data sections */
    *(.RAM_D3_Z5*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z5 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z5 AT> FLASH1

  /* used by the startup to initialize data */
  _siRAM_D3_Z6 = LOADADDR(.RAM_D3_Z6); 
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z6 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z6 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z6)           /* .data sections */
    *(.RAM_D3_Z6*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z6 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z6 AT> FLASH1
  
  /* used by the startup to initialize data */
  _siRAM_D3_Z7 = LOADADDR(.RAM_D3_Z7); 
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z7 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z7 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z7)           /* .data sections */
    *(.RAM_D3_Z7*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z7 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z7 AT> FLASH1
  
  /* used by the startup to initialize data */
  _siRAM_D3_Z8 = LOADADDR(.RAM_D3_Z8); 
  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3_Z8 : 
  {
    . = ALIGN(4);
    _sRAM_D3_Z8 = .;        /* create a global symbol at data start */
    *(.RAM_D3_Z8)           /* .data sections */
    *(.RAM_D3_Z8*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3_Z8 = .;        /* define a global symbol at data end */
  } >RAM_D3_Z8 AT> FLASH1
  
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
