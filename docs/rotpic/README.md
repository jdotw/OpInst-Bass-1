# PIC18F26Q10 Firmware For Rotary Encoders and LEDs

# I2C

## Transmit to Rotary PIC

### Data Byte 0

| Bit | Description |
| --- |:-----------:|
| 0   | LED1 On/Off | 
| 1   | LED2 On/Off | 
| 2   | LED3 On/Off | 
| 3   | LED4 On/Off | 
| 4   | LED5 On/Off | 
| 5   | LED6 On/Off | 
| 6   | _Unused_ | 
| 7   | _Unused_ | 

### Data Byte 1

| Bit | Description |
| --- |:-----------:|
| 0   | Encoder 1 Polarity | 
| 1   | Encoder 2 Polarity | 
| 2   | Encoder 3 Polarity | 
| 3   | Encoder 4 Polarity | 
| 4   | _Unused_ | 
| 5   | _Unused_ | 
| 6   | _Unused_ | 
| 7   | _Unused_ | 

## Receive from Rotary PIC

| Data Byte | Type | Description |
| --- | --- |-----------|
| 0   | Signed 8bit-int | Rotary Encoder 1 Delta | 
| 1   | Signed 8bit-int | Rotary Encoder 2 Delta | 
| 2   | Signed 8bit-int | Rotary Encoder 3 Delta | 
| 3   | Signed 8bit-int | Rotary Encoder 4 Delta | 
| 4   | Bit Mask | Switch States | 

All rotary encoder values are reset to zero after being read. 

### Switch State Bits

| Bit | Description |
| --- |-----------|
| 0   | SW1 State (On/Off) | 
| 1   | SW1 Did Change | 
| 2   | SW2 State (On/Off) | 
| 3   | SW2 Did Change | 
| 4   | _Unused_ | 
| 5   | _Unused_ | 
| 6   | _Unused_ | 
| 7   | _Unused_ | 

Switch state (on/off) values are read directly from the current state of the pin. The did change state is reset after being read.