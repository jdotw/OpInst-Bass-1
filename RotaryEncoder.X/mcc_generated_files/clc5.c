 /**
   CLC5 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc5.c
 
   @Summary
     This is the generated driver implementation file for the CLC5 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC5.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
         Device            :  PIC18F26Q43
         Driver Version    :  1.0.0
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.30 and above or later
         MPLAB             :  MPLAB X 5.40
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
 
 /**
   Section: Included Files
 */

#include <xc.h>
#include "clc5.h"

/**
  Section: CLC5 APIs
*/

void CLC5_Initialize(void)
{
    // Set the CLC5 to the options selected in the User Interface
    
    // SLCT 4; 
    CLCSELECT = 0x04;
    // G1POL not_inverted; G2POL not_inverted; G3POL inverted; G4POL not_inverted; POL not_inverted; 
    CLCnPOL = 0x04;
    // D1S CLCIN4 (CLCIN4PPS); 
    CLCnSEL0 = 0x04;
    // D2S CLCIN5 (CLCIN5PPS); 
    CLCnSEL1 = 0x05;
    // D3S CLCIN5 (CLCIN5PPS); 
    CLCnSEL2 = 0x05;
    // D4S CLCIN5 (CLCIN5PPS); 
    CLCnSEL3 = 0x05;
    // G1D3N disabled; G1D2N disabled; G1D4N disabled; G1D1T enabled; G1D3T disabled; G1D2T disabled; G1D4T disabled; G1D1N disabled; 
    CLCnGLS0 = 0x02;
    // G2D2N disabled; G2D1N disabled; G2D4N disabled; G2D3N disabled; G2D2T enabled; G2D1T disabled; G2D4T enabled; G2D3T enabled; 
    CLCnGLS1 = 0xA8;
    // G3D1N disabled; G3D2N disabled; G3D3N disabled; G3D4N disabled; G3D1T disabled; G3D2T enabled; G3D3T enabled; G3D4T enabled; 
    CLCnGLS2 = 0xA8;
    // G4D1N disabled; G4D2N disabled; G4D3N disabled; G4D4N disabled; G4D1T disabled; G4D2T enabled; G4D3T enabled; G4D4N enabled; 
    CLCnGLS3 = 0x68;
    // CLC5OUT 0; 
    CLCDATA = 0x00;
    // EN enabled; INTN disabled; INTP enabled; MODE 2-input D flip-flop with R; 
    CLCnCON = 0x95;

    // Clear the CLC interrupt flag
    PIR10bits.CLC5IF = 0;
    // Enabling CLC5 interrupt.
    PIE10bits.CLC5IE = 1;
}

void CLC5_ISR(void)
{
    // Clear the CLC interrupt flag
    PIR10bits.CLC5IF = 0;
}

bool CLC5_OutputStatusGet(void)
{
    return(CLCDATAbits.CLC5OUT);
}
/**
 End of File
*/
