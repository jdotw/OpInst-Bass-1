/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F26Q43
        Driver Version    :  2.03
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above or later
        MPLAB 	          :  MPLAB X 5.40
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "interrupt_manager.h"
#include "mcc.h"

void  INTERRUPT_Initialize (void)
{
    // Disable Interrupt Priority Vectors (16CXXX Compatibility Mode)
    INTCON0bits.IPEN = 0;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(PIE15bits.CLC8IE == 1 && PIR15bits.CLC8IF == 1)
    {
        CLC8_ISR();
    }
    else if(PIE14bits.CLC7IE == 1 && PIR14bits.CLC7IF == 1)
    {
        CLC7_ISR();
    }
    else if(PIE11bits.CLC6IE == 1 && PIR11bits.CLC6IF == 1)
    {
        CLC6_ISR();
    }
    else if(PIE10bits.CLC5IE == 1 && PIR10bits.CLC5IF == 1)
    {
        CLC5_ISR();
    }
    else if(PIE9bits.CLC4IE == 1 && PIR9bits.CLC4IF == 1)
    {
        CLC4_ISR();
    }
    else if(PIE7bits.CLC3IE == 1 && PIR7bits.CLC3IF == 1)
    {
        CLC3_ISR();
    }
    else if(PIE6bits.CLC2IE == 1 && PIR6bits.CLC2IF == 1)
    {
        CLC2_ISR();
    }
    else if(PIE0bits.CLC1IE == 1 && PIR0bits.CLC1IF == 1)
    {
        CLC1_ISR();
    }
    else if(PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
*/
