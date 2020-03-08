/*
*  Syntacore SCR* framework
*  @brief Bare metal tests/benchmarks linker script
*  @author mn-sc
*
* Copyright by Syntacore © 2017. ALL RIGHTS RESERVED.
*
*/

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
  SDRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 8M
}

STACK_SIZE = 2048;

SECTIONS {

  .text.crt ORIGIN(SDRAM) : {
    *(.text.crt*)
  } >SDRAM

  .text : {
    PROVIDE(__TEXT_START__ = .);
    *(.text .text.*)
     PROVIDE(__TEXT_END__ = .);
  } >SDRAM

  .rodata : {
    _gp = . + 0x800;
    __global_pointer$ = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
  } >SDRAM

  /* data segment */
  .sdata : {
    PROVIDE(__DATA_START__ = .);
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } >SDRAM

  .data : {
    *(.data .data.*)
    . = ALIGN(4);
    PROVIDE(__DATA_END__ = .);
  } >SDRAM

  /* bss segment */
  .sbss : {
    PROVIDE(__BSS_START__ = .);
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >SDRAM

  .bss : {
    *(.bss .bss.*)
    . = ALIGN(4);
    PROVIDE(__BSS_END__ = .);
  } >SDRAM

  . = ALIGN(16);

  _end = .;
  PROVIDE(__end = .);

  /* End of uninitalized data segement */

  .stack ORIGIN(SDRAM) + LENGTH(SDRAM) - STACK_SIZE : {
    PROVIDE(__STACK_START__ = .);
    . += STACK_SIZE;
    PROVIDE(__C_STACK_TOP__ = .);
    PROVIDE(__STACK_END__ = .);
  } >SDRAM

  /DISCARD/ : {
    *(.eh_frame .eh_frame.*)
  }
}
