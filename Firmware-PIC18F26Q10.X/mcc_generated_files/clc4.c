 /**
   CLC4 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc4.c
 
   @Summary
     This is the generated driver implementation file for the CLC4 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC4.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
         Device            :  PIC18F26Q10
         Driver Version    :  2.11
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.31 and above or later
         MPLAB             :  MPLAB X 5.45
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
#include "clc4.h"
#include "../encoder.h"

/**
  Section: CLC4 APIs
*/

void CLC4_Initialize(void)
{
    // Set the CLC4 to the options selected in the User Interface

    // LC4G1POL not_inverted; LC4G2POL not_inverted; LC4G3POL inverted; LC4G4POL not_inverted; LC4POL not_inverted; 
    CLC4POL = 0x04;
    // LC4D1S CLCIN3 (CLCIN3PPS); 
    CLC4SEL0 = 0x03;
    // LC4D2S CLCIN2 (CLCIN2PPS); 
    CLC4SEL1 = 0x02;
    // LC4D3S CLCIN2 (CLCIN2PPS); 
    CLC4SEL2 = 0x02;
    // LC4D4S CLCIN2 (CLCIN2PPS); 
    CLC4SEL3 = 0x02;
    // LC4G1D3N disabled; LC4G1D2N disabled; LC4G1D4N disabled; LC4G1D1T enabled; LC4G1D3T disabled; LC4G1D2T disabled; LC4G1D4T disabled; LC4G1D1N disabled; 
    CLC4GLS0 = 0x02;
    // LC4G2D2N disabled; LC4G2D1N disabled; LC4G2D4N disabled; LC4G2D3N disabled; LC4G2D2T enabled; LC4G2D1T disabled; LC4G2D4T enabled; LC4G2D3T enabled; 
    CLC4GLS1 = 0xA8;
    // LC4G3D1N disabled; LC4G3D2N disabled; LC4G3D3N disabled; LC4G3D4N disabled; LC4G3D1T disabled; LC4G3D2T enabled; LC4G3D3T enabled; LC4G3D4T enabled; 
    CLC4GLS2 = 0xA8;
    // LC4G4D1N disabled; LC4G4D2N disabled; LC4G4D3N disabled; LC4G4D4N disabled; LC4G4D1T disabled; LC4G4D2T enabled; LC4G4D3T enabled; LC4G4D4T enabled; 
    CLC4GLS3 = 0xA8;
    // LC4EN enabled; INTN disabled; INTP enabled; MODE 2-input D flip-flop with R; 
    CLC4CON = 0x95;

    // Clear the CLC interrupt flag
    PIR5bits.CLC4IF = 0;
    // Enabling CLC4 interrupt.
    PIE5bits.CLC4IE = 1;
}

void CLC4_ISR(void)
{
    // Clear the CLC interrupt flag
    PIR5bits.CLC4IF = 0;
    encoder_clc_isr(1, -1);
}

bool CLC4_OutputStatusGet(void)
{
    return(CLC4CONbits.LC4OUT);
}
/**
 End of File
*/
