 /**
   CLC3 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc3.c
 
   @Summary
     This is the generated driver implementation file for the CLC3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC3.
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
#include "clc3.h"

/**
  Section: CLC3 APIs
*/

void CLC3_Initialize(void)
{
    // Set the CLC3 to the options selected in the User Interface

    // LC3G1POL not_inverted; LC3G2POL not_inverted; LC3G3POL inverted; LC3G4POL not_inverted; LC3POL not_inverted; 
    CLC3POL = 0x04;
    // LC3D1S CLCIN2 (CLCIN2PPS); 
    CLC3SEL0 = 0x02;
    // LC3D2S CLCIN3 (CLCIN3PPS); 
    CLC3SEL1 = 0x03;
    // LC3D3S CLCIN3 (CLCIN3PPS); 
    CLC3SEL2 = 0x03;
    // LC3D4S CLCIN3 (CLCIN3PPS); 
    CLC3SEL3 = 0x03;
    // LC3G1D3N disabled; LC3G1D2N disabled; LC3G1D4N disabled; LC3G1D1T enabled; LC3G1D3T disabled; LC3G1D2T disabled; LC3G1D4T disabled; LC3G1D1N disabled; 
    CLC3GLS0 = 0x02;
    // LC3G2D2N disabled; LC3G2D1N disabled; LC3G2D4N disabled; LC3G2D3N disabled; LC3G2D2T enabled; LC3G2D1T disabled; LC3G2D4T enabled; LC3G2D3T enabled; 
    CLC3GLS1 = 0xA8;
    // LC3G3D1N disabled; LC3G3D2N disabled; LC3G3D3N disabled; LC3G3D4N disabled; LC3G3D1T disabled; LC3G3D2T enabled; LC3G3D3T enabled; LC3G3D4T enabled; 
    CLC3GLS2 = 0xA8;
    // LC3G4D1N disabled; LC3G4D2N disabled; LC3G4D3N disabled; LC3G4D4N disabled; LC3G4D1T disabled; LC3G4D2T enabled; LC3G4D3T enabled; LC3G4D4T enabled; 
    CLC3GLS3 = 0xA8;
    // LC3EN enabled; INTN disabled; INTP enabled; MODE 2-input D flip-flop with R; 
    CLC3CON = 0x95;

    // Clear the CLC interrupt flag
    PIR5bits.CLC3IF = 0;
    // Enabling CLC3 interrupt.
    PIE5bits.CLC3IE = 1;
}

void CLC3_ISR(void)
{
    // Clear the CLC interrupt flag
    PIR5bits.CLC3IF = 0;
}

bool CLC3_OutputStatusGet(void)
{
    return(CLC3CONbits.LC3OUT);
}
/**
 End of File
*/
