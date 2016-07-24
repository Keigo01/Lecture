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
MAX:        DS 1
LIST:       DS 8


; code section
MyCode:     SECTION
main:
_Startup:
            LDHX   #__SEG_END_SSTACK ; initialize the stack pointer
            TXS
            CLI                     ; enable interrupts

            LDA #00000000b
            STA PTADD
            
            LDA #11101111b
            STA PTBDD
            
            LDA #00001111b
            STA PTBD
            
            CLRH
            CLRX

mainLoop:
            ; Insert your code here
            NOP
            feed_watchdog
            LDA PTBD
            AND #00010000b
            BNE PUSH
            BRA mainLoop
            
PUSH:       feed_watchdog
            LDA PTBD
            AND #00010000b
            BEQ LEAVE
            BRA PUSH
            
LEAVE:      feed_watchdog
            LDA PTAD
            AND #00001111b
            CMP #00001111b
            BEQ OUTPUT_SET
            CMP #00001010b
            BGE mainLoop
            STA PTBD
            STA LIST,x
            INCX
            BRA mainLoop
                
OUTPUT_SET:  feed_watchdog
             STX MAX
             CLRX

OUTPUT:      CPX MAX
             BEQ FIN
             LDA LIST,x
             STA PTBD
             INCX
             BRA WAIT
             
WAIT:       feed_watchdog
            LDA PTBD
            AND #00010000b
            BNE PUSH2
            BRA WAIT
        
PUSH2:      feed_watchdog
            LDA PTBD
            AND #00010000b
            BEQ LEAVE2
            BRA PUSH2
            
LEAVE2:     feed_watchdog
            LDA PTAD
            AND #00001111b
            CMP #00001111b
            BEQ OUTPUT
            BRA LEAVE2
                         
FIN:        LDA #00001111b
            STA PTBD
            CLRX
            BRA mainLoop
     
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
