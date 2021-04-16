/**
\file
\addtogroup doc_driver_delay_example
\brief This file contains sample source codes to demonstrate the Delay Driver by using it to toggle GPIO pins.

For this example application to work the following needs to be done:
1. In MCC Pin Manager Grid View, select a GPIO pin (preferably linked to an LED) to be configured as output. This can be done by clicking the lock square in the same column as the pin number and same row as the label "GPIO Output." The lock square should turn green once selected.
2. In MCC Project Resources, go to Pin Module. Rename the selected pin as PIN0. Configure it as digital output by unchecking the box under "Analog" and checking the box under "Output." 
3. Click the "Generate" button.
4. Include the pin_manager.h file in the DELAY_example.c 
5. Include the DELAY_example.h in main.c file.
6. Call the function DELAY_MsExample() or DELAY_UsExample() inside the while(1) loop to toggle PIN_0 for 500 ms or 50 ms (50000 us) respectively.

Note: 50 ms may be too fast for the naked eye to see, thus it may look like it is not toggling from its initial state. Use an oscillator or logic analyzer to view the changing states.

\copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
\page License
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
**/

#include "DELAY_example.h"


/**
*  \ingroup doc_driver_delay_example
*  Call this function from main toggle PIN0 every 500 milliseconds
@param none
*/
void DELAY_MsExample(void)
{   
    DELAY_milliseconds(500);

    PIN0_Toggle();
}

/**
*  \ingroup doc_driver_delay_example
*  Call this function from main toggle PIN0 every 50000 microseconds
@param none
*/
void DELAY_UsExample(void)
{
    DELAY_microseconds(50000);

    PIN0_Toggle();
}

/**
 End of File
 */
