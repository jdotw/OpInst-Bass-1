/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26Q10
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB             :  MPLAB X 5.45

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"





void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x0F;

    /**
    TRISx registers
    */
    TRISA = 0xBF;
    TRISB = 0xE3;
    TRISC = 0xF0;

    /**
    ANSELx registers
    */
    ANSELC = 0x0F;
    ANSELB = 0xDC;
    ANSELA = 0x40;

    /**
    WPUx registers
    */
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONA = 0x00;
    ODCONB = 0x10;
    ODCONC = 0x0F;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;

    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;
    INLVLE = 0x08;





   
    
	
    CLCIN7PPS = 0x17;   //RC7->CLC8:CLCIN7;    
    SSPDATPPS = 0x09;   //RB1->MSSP1:SDA1;    
    RB1PPS = 0x10;   //RB1->MSSP1:SDA1;    
    CLCIN0PPS = 0x00;   //RA0->CLC3:CLCIN0;    
    RB0PPS = 0x0F;   //RB0->MSSP1:SCL1;    
    CLCIN3PPS = 0x15;   //RC5->CLC7:CLCIN3;    
    CLCIN6PPS = 0x16;   //RC6->CLC1:CLCIN6;    
    CLCIN4PPS = 0x02;   //RA2->CLC7:CLCIN4;    
    CLCIN5PPS = 0x03;   //RA3->CLC4:CLCIN5;    
    CLCIN2PPS = 0x14;   //RC4->CLC7:CLCIN2;    
    CLCIN1PPS = 0x01;   //RA1->CLC7:CLCIN1;    
    SSPCLKPPS = 0x08;   //RB0->MSSP1:SCL1;    
}
  
void PIN_MANAGER_IOC(void)
{   
}

/**
 End of File
*/