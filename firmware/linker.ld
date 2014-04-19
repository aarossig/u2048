/*
 * Linker Script for the STM32F4
 */

/* Memory Sections
 */
MEMORY {
    flash (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00100000
    sram  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00010000
}

/* Memory Sections
 */
SECTIONS {
    /* Executable code goes in flash
     */
    .text : {
        *(.interruptVectorTable*);
        *(.text*);
        *(.rodata*);
    } > flash

    .ARM.exidx : {
        *(.ARM.exidx*);
    } > flash
    
    /* Initialized global and static variables are stored after .text
     */
    __flashData = ALIGN(4);
    .data : AT(__flashData) {
        __dataStart = ALIGN(4);
        *(.data*);
        __dataEnd = ALIGN(4);
    } > sram
    
    /* Uninitialized global and static variables are initialized to zero. Do
     * not store the zeros in flash.
     */
    .bss (NOLOAD) : {
        __bssStart = ALIGN(4);
        *(.bss*);
        __bssEnd = ALIGN(4);
    } > sram
}

/* Initial Stack Pointer
 */
__stackPointer = ALIGN(ORIGIN(sram) + LENGTH(sram), 8);

