/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26Q10
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 procedures
#define RA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define RA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define RA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define RA0_GetValue()              PORTAbits.RA0
#define RA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define RA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define RA0_SetPullup()             do { WPUAbits.WPUA0 = 1; } while(0)
#define RA0_ResetPullup()           do { WPUAbits.WPUA0 = 0; } while(0)
#define RA0_SetAnalogMode()         do { ANSELAbits.ANSELA0 = 1; } while(0)
#define RA0_SetDigitalMode()        do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RA1 procedures
#define RA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define RA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define RA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define RA1_GetValue()              PORTAbits.RA1
#define RA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define RA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define RA1_SetPullup()             do { WPUAbits.WPUA1 = 1; } while(0)
#define RA1_ResetPullup()           do { WPUAbits.WPUA1 = 0; } while(0)
#define RA1_SetAnalogMode()         do { ANSELAbits.ANSELA1 = 1; } while(0)
#define RA1_SetDigitalMode()        do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set RA2 procedures
#define RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RA2_GetValue()              PORTAbits.RA2
#define RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RA2_SetPullup()             do { WPUAbits.WPUA2 = 1; } while(0)
#define RA2_ResetPullup()           do { WPUAbits.WPUA2 = 0; } while(0)
#define RA2_SetAnalogMode()         do { ANSELAbits.ANSELA2 = 1; } while(0)
#define RA2_SetDigitalMode()        do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 procedures
#define RA3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define RA3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define RA3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define RA3_GetValue()              PORTAbits.RA3
#define RA3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define RA3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define RA3_SetPullup()             do { WPUAbits.WPUA3 = 1; } while(0)
#define RA3_ResetPullup()           do { WPUAbits.WPUA3 = 0; } while(0)
#define RA3_SetAnalogMode()         do { ANSELAbits.ANSELA3 = 1; } while(0)
#define RA3_SetDigitalMode()        do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set ADDR1 aliases
#define ADDR1_TRIS                 TRISAbits.TRISA4
#define ADDR1_LAT                  LATAbits.LATA4
#define ADDR1_PORT                 PORTAbits.RA4
#define ADDR1_WPU                  WPUAbits.WPUA4
#define ADDR1_OD                   ODCONAbits.ODCA4
#define ADDR1_ANS                  ANSELAbits.ANSELA4
#define ADDR1_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define ADDR1_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define ADDR1_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define ADDR1_GetValue()           PORTAbits.RA4
#define ADDR1_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define ADDR1_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define ADDR1_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define ADDR1_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define ADDR1_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define ADDR1_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define ADDR1_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define ADDR1_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set ADDR2 aliases
#define ADDR2_TRIS                 TRISAbits.TRISA5
#define ADDR2_LAT                  LATAbits.LATA5
#define ADDR2_PORT                 PORTAbits.RA5
#define ADDR2_WPU                  WPUAbits.WPUA5
#define ADDR2_OD                   ODCONAbits.ODCA5
#define ADDR2_ANS                  ANSELAbits.ANSELA5
#define ADDR2_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define ADDR2_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define ADDR2_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define ADDR2_GetValue()           PORTAbits.RA5
#define ADDR2_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define ADDR2_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define ADDR2_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define ADDR2_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define ADDR2_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define ADDR2_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define ADDR2_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define ADDR2_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set ADDR3 aliases
#define ADDR3_TRIS                 TRISAbits.TRISA6
#define ADDR3_LAT                  LATAbits.LATA6
#define ADDR3_PORT                 PORTAbits.RA6
#define ADDR3_WPU                  WPUAbits.WPUA6
#define ADDR3_OD                   ODCONAbits.ODCA6
#define ADDR3_ANS                  ANSELAbits.ANSELA6
#define ADDR3_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define ADDR3_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define ADDR3_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define ADDR3_GetValue()           PORTAbits.RA6
#define ADDR3_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define ADDR3_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define ADDR3_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define ADDR3_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define ADDR3_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define ADDR3_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define ADDR3_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define ADDR3_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set SW1 aliases
#define SW1_TRIS                 TRISAbits.TRISA7
#define SW1_LAT                  LATAbits.LATA7
#define SW1_PORT                 PORTAbits.RA7
#define SW1_WPU                  WPUAbits.WPUA7
#define SW1_OD                   ODCONAbits.ODCA7
#define SW1_ANS                  ANSELAbits.ANSELA7
#define SW1_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define SW1_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define SW1_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define SW1_GetValue()           PORTAbits.RA7
#define SW1_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define SW1_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define SW1_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define SW1_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define SW1_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define SW1_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define SW1_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define SW1_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSELB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set LED5 aliases
#define LED5_TRIS                 TRISBbits.TRISB2
#define LED5_LAT                  LATBbits.LATB2
#define LED5_PORT                 PORTBbits.RB2
#define LED5_WPU                  WPUBbits.WPUB2
#define LED5_OD                   ODCONBbits.ODCB2
#define LED5_ANS                  ANSELBbits.ANSELB2
#define LED5_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define LED5_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define LED5_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define LED5_GetValue()           PORTBbits.RB2
#define LED5_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define LED5_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define LED5_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define LED5_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define LED5_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define LED5_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define LED5_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define LED5_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set LED6 aliases
#define LED6_TRIS                 TRISBbits.TRISB3
#define LED6_LAT                  LATBbits.LATB3
#define LED6_PORT                 PORTBbits.RB3
#define LED6_WPU                  WPUBbits.WPUB3
#define LED6_OD                   ODCONBbits.ODCB3
#define LED6_ANS                  ANSELBbits.ANSELB3
#define LED6_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define LED6_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define LED6_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define LED6_GetValue()           PORTBbits.RB3
#define LED6_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define LED6_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define LED6_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define LED6_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define LED6_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define LED6_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define LED6_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define LED6_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set I2CINT aliases
#define I2CINT_TRIS                 TRISBbits.TRISB4
#define I2CINT_LAT                  LATBbits.LATB4
#define I2CINT_PORT                 PORTBbits.RB4
#define I2CINT_WPU                  WPUBbits.WPUB4
#define I2CINT_OD                   ODCONBbits.ODCB4
#define I2CINT_ANS                  ANSELBbits.ANSELB4
#define I2CINT_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define I2CINT_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define I2CINT_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define I2CINT_GetValue()           PORTBbits.RB4
#define I2CINT_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define I2CINT_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define I2CINT_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define I2CINT_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define I2CINT_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define I2CINT_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define I2CINT_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define I2CINT_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set SW2 aliases
#define SW2_TRIS                 TRISBbits.TRISB5
#define SW2_LAT                  LATBbits.LATB5
#define SW2_PORT                 PORTBbits.RB5
#define SW2_WPU                  WPUBbits.WPUB5
#define SW2_OD                   ODCONBbits.ODCB5
#define SW2_ANS                  ANSELBbits.ANSELB5
#define SW2_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SW2_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SW2_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SW2_GetValue()           PORTBbits.RB5
#define SW2_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SW2_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SW2_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SW2_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define SW2_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define SW2_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define SW2_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define SW2_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set LED1 aliases
#define LED1_TRIS                 TRISCbits.TRISC0
#define LED1_LAT                  LATCbits.LATC0
#define LED1_PORT                 PORTCbits.RC0
#define LED1_WPU                  WPUCbits.WPUC0
#define LED1_OD                   ODCONCbits.ODCC0
#define LED1_ANS                  ANSELCbits.ANSELC0
#define LED1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define LED1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define LED1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define LED1_GetValue()           PORTCbits.RC0
#define LED1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define LED1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define LED1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define LED1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define LED1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define LED1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define LED1_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define LED1_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set LED2 aliases
#define LED2_TRIS                 TRISCbits.TRISC1
#define LED2_LAT                  LATCbits.LATC1
#define LED2_PORT                 PORTCbits.RC1
#define LED2_WPU                  WPUCbits.WPUC1
#define LED2_OD                   ODCONCbits.ODCC1
#define LED2_ANS                  ANSELCbits.ANSELC1
#define LED2_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define LED2_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define LED2_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define LED2_GetValue()           PORTCbits.RC1
#define LED2_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define LED2_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define LED2_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define LED2_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define LED2_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define LED2_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define LED2_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define LED2_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set LED3 aliases
#define LED3_TRIS                 TRISCbits.TRISC2
#define LED3_LAT                  LATCbits.LATC2
#define LED3_PORT                 PORTCbits.RC2
#define LED3_WPU                  WPUCbits.WPUC2
#define LED3_OD                   ODCONCbits.ODCC2
#define LED3_ANS                  ANSELCbits.ANSELC2
#define LED3_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LED3_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LED3_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LED3_GetValue()           PORTCbits.RC2
#define LED3_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LED3_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LED3_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define LED3_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define LED3_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define LED3_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define LED3_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define LED3_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set LED4 aliases
#define LED4_TRIS                 TRISCbits.TRISC3
#define LED4_LAT                  LATCbits.LATC3
#define LED4_PORT                 PORTCbits.RC3
#define LED4_WPU                  WPUCbits.WPUC3
#define LED4_OD                   ODCONCbits.ODCC3
#define LED4_ANS                  ANSELCbits.ANSELC3
#define LED4_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define LED4_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define LED4_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define LED4_GetValue()           PORTCbits.RC3
#define LED4_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define LED4_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define LED4_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define LED4_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define LED4_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define LED4_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define LED4_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define LED4_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSELC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCAF7 pin functionality
 * @Example
    IOCAF7_ISR();
 */
void IOCAF7_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCAF7 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCAF7 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF7_SetInterruptHandler(MyInterruptHandler);

*/
void IOCAF7_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCAF7 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCAF7_SetInterruptHandler() method.
    This handler is called every time the IOCAF7 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF7_SetInterruptHandler(IOCAF7_InterruptHandler);

*/
extern void (*IOCAF7_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCAF7 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCAF7_SetInterruptHandler() method.
    This handler is called every time the IOCAF7 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF7_SetInterruptHandler(IOCAF7_DefaultInterruptHandler);

*/
void IOCAF7_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF5 pin functionality
 * @Example
    IOCBF5_ISR();
 */
void IOCBF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF5_SetInterruptHandler() method.
    This handler is called every time the IOCBF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(IOCBF5_InterruptHandler);

*/
extern void (*IOCBF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF5_SetInterruptHandler() method.
    This handler is called every time the IOCBF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(IOCBF5_DefaultInterruptHandler);

*/
void IOCBF5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/