#include <stdbool.h>
#include <xc.h>

#include "mcc_generated_files/mcc.h"
#include "sw.h"

bool sw_state[2] = { false, false };
bool sw_changed[2] = { false, false };

void sw_1_isr(void){
    // add your IOCAF7 interrupt custom code
    // or set custom function using IOCAF7_SetInterruptHandler()
    sw_state[0] = !PORTAbits.RA7;   // ! because Low = Pressed
    sw_changed[0] = true;
    I2CINT_SetLow();
}

void sw_2_isr(void){
    // add your IOCAF7 interrupt custom code
    // or set custom function using IOCAF7_SetInterruptHandler()
    sw_state[1] = !PORTBbits.RB5;   // ! because Low = Pressed
    sw_changed[1] = true;   
    I2CINT_SetLow();
}

void sw_init(void) {
    IOCAF7_SetInterruptHandler(sw_1_isr);
    IOCBF5_SetInterruptHandler(sw_2_isr);
}