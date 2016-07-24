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
FLAG:       DS 1
MAX:        DS 1
CALI:       DS 1
TMP:        DS 1
LIST:       DS 3


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
            
            LDA #00000000b
            STA FLAG
            STA CALI
            STA MAX
            
            CLRH
            CLRX
            
            STA LIST,x
            
PUSH0:      feed_watchdog
            LDA PTBD
            AND #00010000b
            BNE PUSH0




mainLoop:

           ;Insert your code here
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
            BEQ JOUTPUT_SET
            CMP #00001010b
            BGE OPE
            STA PTBD
            STA LIST,x
            INCX
            CPX #00000100b
            BGE JERROR
            LDA FLAG
            BEQ mainLoop
            BRA CALC
         
JERROR:     JMP FIN

JOUTPUT_SET:    JMP OUTPUT_SET
                
OPE:        STA TMP
            LDA FLAG
            BNE JERROR
            LDA TMP
            STA FLAG
            BRA mainLoop
            
CALC:       feed_watchdog
            CMP #00001011b
            BEQ SUBC
            DECX
            LDA LIST,x
            DECX
            ADD LIST,x
            STA LIST,x
            CMP #00001010b
            BLO mainLoop
            SUB #00001010b
            STA LIST,x
            CPX #00000000B
            BEQ ZERO
            DECX
            LDA LIST,x
            ADD #00000001b
            STA LIST,x
            INCX
            CMP #00001010b
            BLO mainLoop
            DECX
            SUB #00001010b
            STA LIST,x
            LDA #00000001b
            STA CALI
            INCX
            BRA mainLoop
            
ZERO:       LDA #00000001b
            STA CALI
            JMP mainLoop
            
SUBC:       feed_watchdog
            DECX
            DECX
            LDA LIST,x
            INCX
            SUB LIST,x
            DECX
            STA LIST,x
            BMI MIN
            JMP mainLoop

MIN:        feed_watchdog
            
            ADD #00001010b
            STA LIST,x
            DECX
            BMI FIN
            LDA LIST,x
            SUB #00000001b
            STA LIST,x
            INCX
            CMP #00000000b
            BNE Jmain
            LDA LIST,x
            DECX
            CPX #00001010b
            STA LIST,x
            JMP mainLoop
            
Jmain:      JMP mainLoop


            
OUTPUT_SET:  feed_watchdog
             STX MAX
             CLRX
             LDA FLAG
             BEQ FIN

OUTPUT:      feed_watchdog
             CPX MAX
             BGT FIN
             LDA CALI
             BNE CAL
             LDA LIST,x
             STA PTBD
             INCX
             BRA WAIT
             
CAL:        STA PTBD
            LDA #00000000b
            STA CALI
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
            LDA #00000000b
            STA MAX
            STA FLAG
            STA CALI
            STA LIST,x
            JMP mainLoop
            
            
   
            

            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
