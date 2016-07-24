;*******************************************************************
;* This stationery serves as the framework for a user application. *
;* For a more comprehensive program that demonstrates the more     *
;* advanced functionality of this processor, please see the        *
;* demonstration applications, located in the examples             *
;* subdirectory of the "Freescale CodeWarrior for HC08" program    *
;* directory.                                                      *
;*******************************************************************

; Include derivative-specific definitions
            INCLUDE 'derivative.inc'

; export symbols
            XDEF _Startup, main
            ; we export both '_Startup' and 'main' as symbols. Either can
            ; be referenced in the linker .prm file or from C/C++ later on

            XREF __SEG_END_SSTACK   ; symbol defined by the linker for the end of the stack


; variable/data section
MY_ZEROPAGE: SECTION  SHORT         ; Insert here your data definition

; code section
MyCode:     SECTION
main:
_Startup:
            LDHX   #__SEG_END_SSTACK ; initialize the stack pointer
            TXS
            CLI                     ; enable interrupts

            LDA #00000000b
            STA PTADD
            
            LDA #11111111b
            STA PTBDD

mainLoop:
            ; Insert your code here
            NOP
            feed_watchdog
            LDA PTAD
            AND #00000001
            BEQ ZERO
            LDA #00000001b
            STA PTBD
            BRA mainLoop
            
ZERO:       LDA #00000010b
            STA PTBD
            BRA mainLoop   
         
            BRA    mainLoop
