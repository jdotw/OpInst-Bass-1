#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "i2c.h"
#include "encoder.h"
#include "sw.h"

volatile uint8_t rx_index;
volatile uint8_t tx_index;

static void _i2c_addr_callback() {
    // Called when we see our address on the bus
    
    // Must read the address to clear SSP1BUF
    uint8_t data = SSP1BUF;
    
    // Reset our state
    rx_index = 0;
    tx_index = 0;
}

static void _i2c_rx_led_cmd(uint8_t data) {
    // Note: because the LED pins are operating as a open drain
    // current-sink, low is LED on, high is LED off
    if (data & LED_1_STATE) LED1_SetLow(); else LED1_SetHigh();
    if (data & LED_2_STATE) LED2_SetLow(); else LED2_SetHigh();
    if (data & LED_3_STATE) LED3_SetLow(); else LED3_SetHigh();
    if (data & LED_4_STATE) LED4_SetLow(); else LED4_SetHigh();
    if (data & LED_5_STATE) LED5_SetLow(); else LED5_SetHigh();
    if (data & LED_6_STATE) LED6_SetLow(); else LED6_SetHigh();
}

static void _i2c_rx_callback() {
    // Called when master is sending us data
    uint8_t data = SSP1BUF;
    
    switch(rx_index) {
        case 0:
        _i2c_rx_led_cmd(data);
        break;
    }
    
    rx_index++;
}

static void _i2c_tx_callback() {
    // Called when master wants data from us
    switch (tx_index) {
        case 0:
        case 1:
        case 2:
        case 3:
            SSP1BUF = (uint8_t)enc_value[tx_index];
            enc_value[tx_index] = 0;
            break;
        case 4: {
            uint8_t tx = 0;
            tx |= sw_state[0] << 0;
            tx |= sw_changed[0] << 1;
            tx |= sw_state[1] << 2;
            tx |= sw_changed[1] << 3;            
            SSP1BUF = tx;
            sw_changed[0] = 0;
            sw_changed[1] = 0;
            break;
        }
    }    
    tx_index++;
    
    // Reset interrupt (inactive)
    I2CINT_SetHigh();
}

void i2c_init() {
    // Read address pins
    uint8_t a0 = ADDR1_GetValue();
    uint8_t a1 = ADDR2_GetValue();
    uint8_t a2 = ADDR3_GetValue();
    uint8_t i2c_addr = ((uint8_t)0x50) | a0 | (uint8_t)(a1 << 1) | (uint8_t)(a2 << 2);

    // Start I2C
    I2C1_Open(i2c_addr);
    I2C1_SlaveSetAddrIntHandler(_i2c_addr_callback);    
    I2C1_SlaveSetWriteIntHandler(_i2c_tx_callback);
    I2C1_SlaveSetReadIntHandler(_i2c_rx_callback);    
}

