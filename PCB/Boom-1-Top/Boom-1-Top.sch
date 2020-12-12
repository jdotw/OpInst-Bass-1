EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A0 46811 33110
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PIC18F26Q43:PIC18F26Q43 U2
U 1 1 5FD37D72
P 16350 4450
F 0 "U2" H 16075 4365 50  0000 C CNN
F 1 "PIC18F26Q43" H 16075 4274 50  0000 C CNN
F 2 "" H 16350 4450 50  0001 C CNN
F 3 "" H 16350 4450 50  0001 C CNN
	1    16350 4450
	1    0    0    -1  
$EndComp
$Comp
L MCU_ST_STM32F4:STM32F407VGTx U1
U 1 1 5FD3AAB9
P 8950 6000
F 0 "U1" H 8950 3111 50  0000 C CNN
F 1 "STM32F407VGTx" H 8950 3020 50  0000 C CNN
F 2 "Package_QFP:LQFP-100_14x14mm_P0.5mm" H 8250 3400 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00037051.pdf" H 8950 6000 50  0001 C CNN
	1    8950 6000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J37
U 1 1 606F9E0C
P 3500 13900
F 0 "J37" H 3550 14517 50  0000 C CNN
F 1 "Osc1Conn" H 3550 14426 50  0000 C CNN
F 2 "" H 3500 13900 50  0001 C CNN
F 3 "~" H 3500 13900 50  0001 C CNN
	1    3500 13900
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J38
U 1 1 606FB0DA
P 3500 15450
F 0 "J38" H 3550 16067 50  0000 C CNN
F 1 "Osc2Conn" H 3550 15976 50  0000 C CNN
F 2 "" H 3500 15450 50  0001 C CNN
F 3 "~" H 3500 15450 50  0001 C CNN
	1    3500 15450
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x06_Odd_Even J39
U 1 1 607010E7
P 4650 16700
F 0 "J39" V 4654 16312 50  0000 R CNN
F 1 "SubNoiseConn" V 4745 16312 50  0000 R CNN
F 2 "" H 4650 16700 50  0001 C CNN
F 3 "~" H 4650 16700 50  0001 C CNN
	1    4650 16700
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Odd_Even J40
U 1 1 607053C9
P 6850 16700
F 0 "J40" V 6854 16012 50  0000 R CNN
F 1 "FiltConn2" V 6945 16012 50  0000 R CNN
F 2 "" H 6850 16700 50  0001 C CNN
F 3 "~" H 6850 16700 50  0001 C CNN
	1    6850 16700
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J43
U 1 1 60708219
P 9300 16700
F 0 "J43" V 9304 16212 50  0000 R CNN
F 1 "FXConn1" V 9395 16212 50  0000 R CNN
F 2 "" H 9300 16700 50  0001 C CNN
F 3 "~" H 9300 16700 50  0001 C CNN
	1    9300 16700
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J44
U 1 1 6070D0C0
P 6450 13350
F 0 "J44" V 6454 13062 50  0000 R CNN
F 1 "FiltConn1" V 6545 13062 50  0000 R CNN
F 2 "" H 6450 13350 50  0001 C CNN
F 3 "~" H 6450 13350 50  0001 C CNN
	1    6450 13350
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J45
U 1 1 6070E3D6
P 9200 13350
F 0 "J45" V 9204 13062 50  0000 R CNN
F 1 "FXConn2" V 9295 13062 50  0000 R CNN
F 2 "" H 9200 13350 50  0001 C CNN
F 3 "~" H 9200 13350 50  0001 C CNN
	1    9200 13350
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J42
U 1 1 6070FDF5
P 11450 13900
F 0 "J42" H 11500 14517 50  0000 C CNN
F 1 "EnvGenConn1" H 11500 14426 50  0000 C CNN
F 2 "" H 11450 13900 50  0001 C CNN
F 3 "~" H 11450 13900 50  0001 C CNN
	1    11450 13900
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J41
U 1 1 60710CA3
P 11450 15200
F 0 "J41" H 11500 15817 50  0000 C CNN
F 1 "EnvGenConn2" H 11500 15726 50  0000 C CNN
F 2 "" H 11450 15200 50  0001 C CNN
F 3 "~" H 11450 15200 50  0001 C CNN
	1    11450 15200
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J46
U 1 1 60711C12
P 11450 16500
F 0 "J46" H 11500 17117 50  0000 C CNN
F 1 "EnvGenConn3" H 11500 17026 50  0000 C CNN
F 2 "" H 11450 16500 50  0001 C CNN
F 3 "~" H 11450 16500 50  0001 C CNN
	1    11450 16500
	-1   0    0    -1  
$EndComp
Text GLabel 3700 13500 2    50   Input ~ 0
Osc1Scale
Text GLabel 3700 13600 2    50   Input ~ 0
Osc1Scale-5V
Text GLabel 3700 13700 2    50   Input ~ 0
Osc1NoteCV
Text GLabel 3700 13800 2    50   Input ~ 0
Osc1PWMCV
Text GLabel 3700 13900 2    50   Input ~ 0
Osc1HFTrackHigh
Text GLabel 3700 14000 2    50   Input ~ 0
Osc1HFTrackWiper
Text GLabel 3700 14100 2    50   Input ~ 0
Osc1SyncOut
Text GLabel 3700 14200 2    50   Input ~ 0
Osc1HardSyncIn
Text GLabel 3700 14300 2    50   Input ~ 0
Osc1SoftSyncIn
Text GLabel 3200 14300 0    50   Input ~ 0
Osc1TuningOut
Text GLabel 3200 13800 0    50   Input ~ 0
Osc1ToOsc2FiltMixCV
Text GLabel 3200 13700 0    50   Input ~ 0
Osc1SqrMixCV
Text GLabel 3200 13600 0    50   Input ~ 0
Osc1SawMixCV
Text GLabel 3200 13500 0    50   Input ~ 0
Osc1ToOsc1FiltMixCV
Text GLabel 3200 15850 0    50   Input ~ 0
Osc2TuningOut
Text GLabel 3200 15150 0    50   Input ~ 0
Osc2SqrMixCV
Text GLabel 3200 15050 0    50   Input ~ 0
Osc2SawMixCV
Text GLabel 3700 15050 2    50   Input ~ 0
Osc2Scale
Text GLabel 3700 15150 2    50   Input ~ 0
Osc2Scale-5V
Text GLabel 3700 15250 2    50   Input ~ 0
Osc2NoteCV
Text GLabel 3700 15350 2    50   Input ~ 0
Osc2PWMCV
Text GLabel 3700 15450 2    50   Input ~ 0
Osc2HFTrackHigh
Text GLabel 3700 15550 2    50   Input ~ 0
Osc2HFTrackWiper
Text GLabel 3700 15650 2    50   Input ~ 0
Osc2SyncOut
Text GLabel 3700 15750 2    50   Input ~ 0
Osc2HardSyncIn
Text GLabel 3700 15850 2    50   Input ~ 0
Osc2SoftSyncIn
Text GLabel 4850 16500 2    50   Input ~ 0
SubToOsc2FiltMixCV
Text GLabel 4850 16600 2    50   Input ~ 0
SubOct1Out
Text GLabel 4850 16700 2    50   Input ~ 0
SubOctBothOut
Text GLabel 4850 16800 2    50   Input ~ 0
SubOct2Out
Text GLabel 4850 16900 2    50   Input ~ 0
SubToSubFiltMixCV
Text GLabel 4350 16700 0    50   Input ~ 0
PinkNoiseOut
Text GLabel 4350 16800 0    50   Input ~ 0
WhiteNoiseOut
Text GLabel 4350 16900 0    50   Input ~ 0
NoiseToSubFiltMixCV
Text GLabel 4350 17000 0    50   Input ~ 0
NoiseToOsc2FiltMixCV
Text GLabel 6350 16500 1    50   Input ~ 0
Osc2FiltFreqCV
Text GLabel 6450 16500 1    50   Input ~ 0
Osc2FXReturn
Text GLabel 6550 16500 1    50   Input ~ 0
Osc2FiltResCV
Text GLabel 6650 16500 1    50   Input ~ 0
Osc2FXSend
Text GLabel 6750 16500 1    50   Input ~ 0
SubFiltFreqCV
Text GLabel 6850 16500 1    50   Input ~ 0
SubFXReturn
Text GLabel 6950 16500 1    50   Input ~ 0
SubFXSend
Text GLabel 7050 16500 1    50   Input ~ 0
SubFiltResCV
Text GLabel 7150 16500 1    50   Input ~ 0
PreFV1FXSend
Text GLabel 7250 16500 1    50   Input ~ 0
PreFV1FXReturn
Text GLabel 7350 17000 3    50   Input ~ 0
SubAmpAudioBypassSend
Text GLabel 7250 17000 3    50   Input ~ 0
SubAmpAudioBypassReturn
Text GLabel 11150 13500 0    50   Input ~ 0
OscFiltEnv1ReleaseCV
Text GLabel 11150 13600 0    50   Input ~ 0
OscFiltEnv1SustainCV
Text GLabel 11150 13700 0    50   Input ~ 0
OscFiltEnv1DecayCV
Text GLabel 11150 13800 0    50   Input ~ 0
OscFiltEnv1AttackCV
Text GLabel 11150 13900 0    50   Input ~ 0
OscFiltEnv2AmtWiper
Text GLabel 11150 14000 0    50   Input ~ 0
OscFiltEnv2AmtHigh
Text GLabel 11150 14100 0    50   Input ~ 0
OscFiltEnv2Gate
Text GLabel 11150 14200 0    50   Input ~ 0
OscFiltEnv2Trigger
Text GLabel 11650 14200 2    50   Input ~ 0
OscFiltEnv2ReleaseCV
Text GLabel 11650 14100 2    50   Input ~ 0
OscFiltEnv2SustainCV
Text GLabel 11650 14000 2    50   Input ~ 0
OscFiltEnv2DecayCV
Text GLabel 11650 13900 2    50   Input ~ 0
OscFiltEnv2AttackCV
Text GLabel 11650 13800 2    50   Input ~ 0
OscFiltEnv1AmtWiper
Text GLabel 11650 13700 2    50   Input ~ 0
OscFiltEnv1AmtHigh
Text GLabel 11650 13600 2    50   Input ~ 0
OscFiltEnv1Gate
Text GLabel 11650 13500 2    50   Input ~ 0
OscFiltEnv1Trigger
Text GLabel 11150 14800 0    50   Input ~ 0
OscAmpEnvReleaseCV
Text GLabel 11150 14900 0    50   Input ~ 0
OscAmpEnvSustainCV
Text GLabel 11150 15000 0    50   Input ~ 0
OscAmpEnvDecayCV
Text GLabel 11150 15100 0    50   Input ~ 0
OscAmpEnvAttackCV
Text GLabel 11150 15200 0    50   Input ~ 0
SubAmpEnvAmtWiper
Text GLabel 11150 15300 0    50   Input ~ 0
SubAmpEnvAmtHigh
Text GLabel 11150 15400 0    50   Input ~ 0
SubAmpEnvGate
Text GLabel 11150 15500 0    50   Input ~ 0
SubAmpEnvTrigger
Text GLabel 11650 15500 2    50   Input ~ 0
SubAmpEnvReleaseCV
Text GLabel 11650 15400 2    50   Input ~ 0
SubAmpEnvSustainCV
Text GLabel 11650 15300 2    50   Input ~ 0
SubAmpEnvDecayCV
Text GLabel 11650 15200 2    50   Input ~ 0
SubAmpEnvAttackCV
Text GLabel 11650 15100 2    50   Input ~ 0
OscAmpEnvAmtWiper
Text GLabel 11650 15000 2    50   Input ~ 0
OscAmpEnvAmtHigh
Text GLabel 11650 14900 2    50   Input ~ 0
OscAmpEnvGate
Text GLabel 11650 14800 2    50   Input ~ 0
OscAmpEnvTrigger
Text GLabel 11150 16100 0    50   Input ~ 0
SubFiltEnv2ReleaseCV
Text GLabel 11150 16200 0    50   Input ~ 0
SubFiltEnv2SustainCV
Text GLabel 11150 16300 0    50   Input ~ 0
SubFiltEnv2DecayCV
Text GLabel 11150 16400 0    50   Input ~ 0
SubFiltEnv2AttackCV
Text GLabel 11150 16500 0    50   Input ~ 0
SubFiltEnv1AmtWiper
Text GLabel 11150 16600 0    50   Input ~ 0
SubFiltEnv1AmtHigh
Text GLabel 11150 16700 0    50   Input ~ 0
SubFiltEnv1Gate
Text GLabel 11150 16800 0    50   Input ~ 0
SubFiltEnv1Trigger
Text GLabel 11650 16800 2    50   Input ~ 0
SubFiltEnv1ReleaseCV
Text GLabel 11650 16700 2    50   Input ~ 0
SubFiltEnv1SustainCV
Text GLabel 11650 16600 2    50   Input ~ 0
SubFiltEnv1DecayCV
Text GLabel 11650 16500 2    50   Input ~ 0
SubFiltEnv1AttackCV
Text GLabel 11650 16300 2    50   Input ~ 0
SubFiltEnv2AmtWiper
Text GLabel 11650 16400 2    50   Input ~ 0
SubFiltEnv2AmtHigh
Text GLabel 11650 16200 2    50   Input ~ 0
SubFiltEnv2Gate
Text GLabel 11650 16100 2    50   Input ~ 0
SubFiltEnv2Trigger
Text GLabel 9000 16500 1    50   Input ~ 0
FV1ControlCommonHigh
Text GLabel 9100 16500 1    50   Input ~ 0
FV1ControlPot1Wiper
Text GLabel 9200 16500 1    50   Input ~ 0
FV1ControlPot2Wiper
Text GLabel 9300 16500 1    50   Input ~ 0
FV1ControlPot3Wiper
Text GLabel 9300 13150 1    50   Input ~ 0
FV1EEpromSDA
Text GLabel 9200 13150 1    50   Input ~ 0
FV1EEpromSCL
Text GLabel 9000 13650 3    50   Input ~ 0
FV1FeedbackCV
Text GLabel 9100 13650 3    50   Input ~ 0
FV1WetDryCV
Text GLabel 9200 13650 3    50   Input ~ 0
MixOutLeftTip
Text GLabel 9300 13650 3    50   Input ~ 0
MixOutRightTip
Text GLabel 6350 13650 3    50   Input ~ 0
Osc1FXSend
Text GLabel 6450 13650 3    50   Input ~ 0
Osc1FiltFreqCV
Text GLabel 6550 13650 3    50   Input ~ 0
Osc1FiltResCV
Text GLabel 6650 13650 3    50   Input ~ 0
Osc1FXReturn
$Comp
L power:GNDA #PWR?
U 1 1 610E7D91
P 6450 13050
F 0 "#PWR?" H 6450 12800 50  0001 C CNN
F 1 "GNDA" H 6455 12877 50  0000 C CNN
F 2 "" H 6450 13050 50  0001 C CNN
F 3 "" H 6450 13050 50  0001 C CNN
	1    6450 13050
	-1   0    0    1   
$EndComp
$Comp
L power:+12VA #PWR?
U 1 1 610E933A
P 6350 12800
F 0 "#PWR?" H 6350 12650 50  0001 C CNN
F 1 "+12VA" H 6365 12973 50  0000 C CNN
F 2 "" H 6350 12800 50  0001 C CNN
F 3 "" H 6350 12800 50  0001 C CNN
	1    6350 12800
	1    0    0    -1  
$EndComp
$Comp
L power:-12VA #PWR?
U 1 1 610EB0DC
P 6550 12700
F 0 "#PWR?" H 6550 12550 50  0001 C CNN
F 1 "-12VA" H 6565 12873 50  0000 C CNN
F 2 "" H 6550 12700 50  0001 C CNN
F 3 "" H 6550 12700 50  0001 C CNN
	1    6550 12700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 12800 6350 13150
Wire Wire Line
	6450 13050 6450 13150
Wire Wire Line
	6550 12700 6550 13150
$Comp
L power:GNDA #PWR?
U 1 1 610ECAEF
P 6650 13150
F 0 "#PWR?" H 6650 12900 50  0001 C CNN
F 1 "GNDA" V 6655 13022 50  0000 R CNN
F 2 "" H 6650 13150 50  0001 C CNN
F 3 "" H 6650 13150 50  0001 C CNN
	1    6650 13150
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610EDB74
P 3200 14100
F 0 "#PWR?" H 3200 13850 50  0001 C CNN
F 1 "GNDA" V 3205 13973 50  0000 R CNN
F 2 "" H 3200 14100 50  0001 C CNN
F 3 "" H 3200 14100 50  0001 C CNN
	1    3200 14100
	0    1    1    0   
$EndComp
Wire Wire Line
	3200 13900 3200 14000
Connection ~ 3200 14000
Wire Wire Line
	3200 14000 3200 14100
Connection ~ 3200 14100
Wire Wire Line
	3200 14100 3200 14200
$Comp
L power:GNDA #PWR?
U 1 1 610EE417
P 3200 15650
F 0 "#PWR?" H 3200 15400 50  0001 C CNN
F 1 "GNDA" V 3205 15523 50  0000 R CNN
F 2 "" H 3200 15650 50  0001 C CNN
F 3 "" H 3200 15650 50  0001 C CNN
	1    3200 15650
	0    1    1    0   
$EndComp
Wire Wire Line
	3200 15250 3200 15350
Connection ~ 3200 15350
Wire Wire Line
	3200 15350 3200 15450
Connection ~ 3200 15450
Wire Wire Line
	3200 15450 3200 15550
Connection ~ 3200 15550
Wire Wire Line
	3200 15550 3200 15650
Connection ~ 3200 15650
Wire Wire Line
	3200 15650 3200 15750
$Comp
L power:GNDA #PWR?
U 1 1 610EEFBC
P 4350 16500
F 0 "#PWR?" H 4350 16250 50  0001 C CNN
F 1 "GNDA" V 4355 16373 50  0000 R CNN
F 2 "" H 4350 16500 50  0001 C CNN
F 3 "" H 4350 16500 50  0001 C CNN
	1    4350 16500
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 16500 4350 16600
Connection ~ 4350 16500
$Comp
L power:GNDA #PWR?
U 1 1 610EFD7A
P 4850 17000
F 0 "#PWR?" H 4850 16750 50  0001 C CNN
F 1 "GNDA" H 4855 16827 50  0000 C CNN
F 2 "" H 4850 17000 50  0001 C CNN
F 3 "" H 4850 17000 50  0001 C CNN
	1    4850 17000
	1    0    0    -1  
$EndComp
$Comp
L power:+12VA #PWR?
U 1 1 610F05D2
P 6350 17300
F 0 "#PWR?" H 6350 17150 50  0001 C CNN
F 1 "+12VA" H 6365 17473 50  0000 C CNN
F 2 "" H 6350 17300 50  0001 C CNN
F 3 "" H 6350 17300 50  0001 C CNN
	1    6350 17300
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610F0EF9
P 6450 17100
F 0 "#PWR?" H 6450 16850 50  0001 C CNN
F 1 "GNDA" H 6455 16927 50  0000 C CNN
F 2 "" H 6450 17100 50  0001 C CNN
F 3 "" H 6450 17100 50  0001 C CNN
	1    6450 17100
	1    0    0    -1  
$EndComp
$Comp
L power:-12VA #PWR?
U 1 1 610F1877
P 6550 17400
F 0 "#PWR?" H 6550 17250 50  0001 C CNN
F 1 "-12VA" H 6565 17573 50  0000 C CNN
F 2 "" H 6550 17400 50  0001 C CNN
F 3 "" H 6550 17400 50  0001 C CNN
	1    6550 17400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6350 17000 6350 17300
Wire Wire Line
	6450 17000 6450 17100
Wire Wire Line
	6550 17000 6550 17400
$Comp
L power:+12VA #PWR?
U 1 1 610F3324
P 6650 17600
F 0 "#PWR?" H 6650 17450 50  0001 C CNN
F 1 "+12VA" H 6665 17773 50  0000 C CNN
F 2 "" H 6650 17600 50  0001 C CNN
F 3 "" H 6650 17600 50  0001 C CNN
	1    6650 17600
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610F333A
P 6750 17100
F 0 "#PWR?" H 6750 16850 50  0001 C CNN
F 1 "GNDA" H 6755 16927 50  0000 C CNN
F 2 "" H 6750 17100 50  0001 C CNN
F 3 "" H 6750 17100 50  0001 C CNN
	1    6750 17100
	1    0    0    -1  
$EndComp
$Comp
L power:-12VA #PWR?
U 1 1 610F334C
P 6850 17400
F 0 "#PWR?" H 6850 17250 50  0001 C CNN
F 1 "-12VA" H 6865 17573 50  0000 C CNN
F 2 "" H 6850 17400 50  0001 C CNN
F 3 "" H 6850 17400 50  0001 C CNN
	1    6850 17400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6650 17000 6650 17600
Wire Wire Line
	6750 17000 6750 17100
Wire Wire Line
	6850 17000 6850 17400
$Comp
L power:+12VA #PWR?
U 1 1 610F57A0
P 6950 17600
F 0 "#PWR?" H 6950 17450 50  0001 C CNN
F 1 "+12VA" H 6965 17773 50  0000 C CNN
F 2 "" H 6950 17600 50  0001 C CNN
F 3 "" H 6950 17600 50  0001 C CNN
	1    6950 17600
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610F57B6
P 7050 17100
F 0 "#PWR?" H 7050 16850 50  0001 C CNN
F 1 "GNDA" H 7055 16927 50  0000 C CNN
F 2 "" H 7050 17100 50  0001 C CNN
F 3 "" H 7050 17100 50  0001 C CNN
	1    7050 17100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 17000 6950 17600
Wire Wire Line
	7050 17000 7050 17100
Wire Wire Line
	7150 17000 7150 17400
$Comp
L power:-12VA #PWR?
U 1 1 610F57C8
P 7150 17400
F 0 "#PWR?" H 7150 17250 50  0001 C CNN
F 1 "-12VA" V 7200 17500 50  0000 C CNN
F 2 "" H 7150 17400 50  0001 C CNN
F 3 "" H 7150 17400 50  0001 C CNN
	1    7150 17400
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610FC8E5
P 7450 17000
F 0 "#PWR?" H 7450 16750 50  0001 C CNN
F 1 "GNDA" V 7455 16872 50  0000 R CNN
F 2 "" H 7450 17000 50  0001 C CNN
F 3 "" H 7450 17000 50  0001 C CNN
	1    7450 17000
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610FCE23
P 7450 16500
F 0 "#PWR?" H 7450 16250 50  0001 C CNN
F 1 "GNDA" V 7455 16372 50  0000 R CNN
F 2 "" H 7450 16500 50  0001 C CNN
F 3 "" H 7450 16500 50  0001 C CNN
	1    7450 16500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 16500 7450 16500
Connection ~ 7450 16500
$Comp
L power:GNDA #PWR?
U 1 1 610FE3C2
P 11150 14300
F 0 "#PWR?" H 11150 14050 50  0001 C CNN
F 1 "GNDA" H 11155 14127 50  0000 C CNN
F 2 "" H 11150 14300 50  0001 C CNN
F 3 "" H 11150 14300 50  0001 C CNN
	1    11150 14300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610FED6F
P 11650 14300
F 0 "#PWR?" H 11650 14050 50  0001 C CNN
F 1 "GNDA" H 11655 14127 50  0000 C CNN
F 2 "" H 11650 14300 50  0001 C CNN
F 3 "" H 11650 14300 50  0001 C CNN
	1    11650 14300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610FF139
P 11150 15600
F 0 "#PWR?" H 11150 15350 50  0001 C CNN
F 1 "GNDA" H 11155 15427 50  0000 C CNN
F 2 "" H 11150 15600 50  0001 C CNN
F 3 "" H 11150 15600 50  0001 C CNN
	1    11150 15600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 610FF14B
P 11650 15600
F 0 "#PWR?" H 11650 15350 50  0001 C CNN
F 1 "GNDA" H 11655 15427 50  0000 C CNN
F 2 "" H 11650 15600 50  0001 C CNN
F 3 "" H 11650 15600 50  0001 C CNN
	1    11650 15600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 61100708
P 11150 16900
F 0 "#PWR?" H 11150 16650 50  0001 C CNN
F 1 "GNDA" H 11155 16727 50  0000 C CNN
F 2 "" H 11150 16900 50  0001 C CNN
F 3 "" H 11150 16900 50  0001 C CNN
	1    11150 16900
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 6110071A
P 11650 16900
F 0 "#PWR?" H 11650 16650 50  0001 C CNN
F 1 "GNDA" H 11655 16727 50  0000 C CNN
F 2 "" H 11650 16900 50  0001 C CNN
F 3 "" H 11650 16900 50  0001 C CNN
	1    11650 16900
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 6110180E
P 9700 17000
F 0 "#PWR?" H 9700 16750 50  0001 C CNN
F 1 "GNDA" V 9705 16872 50  0000 R CNN
F 2 "" H 9700 17000 50  0001 C CNN
F 3 "" H 9700 17000 50  0001 C CNN
	1    9700 17000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9700 17000 9600 17000
Connection ~ 9700 17000
Connection ~ 9100 17000
Wire Wire Line
	9100 17000 9000 17000
Connection ~ 9200 17000
Wire Wire Line
	9200 17000 9100 17000
Connection ~ 9300 17000
Wire Wire Line
	9300 17000 9200 17000
Connection ~ 9400 17000
Wire Wire Line
	9400 17000 9300 17000
Connection ~ 9500 17000
Wire Wire Line
	9500 17000 9400 17000
Connection ~ 9600 17000
Wire Wire Line
	9600 17000 9500 17000
$Comp
L power:GNDA #PWR?
U 1 1 6110349D
P 9700 16500
F 0 "#PWR?" H 9700 16250 50  0001 C CNN
F 1 "GNDA" V 9705 16372 50  0000 R CNN
F 2 "" H 9700 16500 50  0001 C CNN
F 3 "" H 9700 16500 50  0001 C CNN
	1    9700 16500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9400 16500 9500 16500
Connection ~ 9700 16500
Connection ~ 9500 16500
Wire Wire Line
	9500 16500 9600 16500
Connection ~ 9600 16500
Wire Wire Line
	9600 16500 9700 16500
$Comp
L power:GNDA #PWR?
U 1 1 61104B8C
P 9400 13150
F 0 "#PWR?" H 9400 12900 50  0001 C CNN
F 1 "GNDA" V 9405 13022 50  0000 R CNN
F 2 "" H 9400 13150 50  0001 C CNN
F 3 "" H 9400 13150 50  0001 C CNN
	1    9400 13150
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 611051BA
P 9400 13650
F 0 "#PWR?" H 9400 13400 50  0001 C CNN
F 1 "GNDA" V 9405 13522 50  0000 R CNN
F 2 "" H 9400 13650 50  0001 C CNN
F 3 "" H 9400 13650 50  0001 C CNN
	1    9400 13650
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 61105480
P 9000 13150
F 0 "#PWR?" H 9000 12900 50  0001 C CNN
F 1 "GNDA" V 9005 13023 50  0000 R CNN
F 2 "" H 9000 13150 50  0001 C CNN
F 3 "" H 9000 13150 50  0001 C CNN
	1    9000 13150
	0    1    1    0   
$EndComp
Wire Wire Line
	9100 13150 9000 13150
Connection ~ 9000 13150
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 611074E9
P 3300 12150
F 0 "U?" H 3300 12931 50  0000 C CNN
F 1 "DAC7678xPW" H 3300 12840 50  0000 C CNN
F 2 "" H 3300 12150 50  0001 C CNN
F 3 "" H 3300 12150 50  0001 C CNN
	1    3300 12150
	1    0    0    -1  
$EndComp
Text GLabel 3750 11850 2    50   Input ~ 0
Osc1NoteCV
Text GLabel 3750 11950 2    50   Input ~ 0
Osc1PWMCV
Text GLabel 3750 12050 2    50   Input ~ 0
Osc1SawMixCV
Text GLabel 3750 12150 2    50   Input ~ 0
Osc1SqrMixCV
Text GLabel 3750 12250 2    50   Input ~ 0
Osc1ToOsc1FiltMixCV
Text GLabel 3750 12350 2    50   Input ~ 0
Osc1ToOsc2FiltMixCV
Text GLabel 3750 12450 2    50   Input ~ 0
Osc1FiltFreqCV
Text GLabel 3750 12550 2    50   Input ~ 0
Osc1FiltResCV
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 6110BEBD
P 3500 18300
F 0 "U?" H 3500 19081 50  0000 C CNN
F 1 "DAC7678xPW" H 3500 18990 50  0000 C CNN
F 2 "" H 3500 18300 50  0001 C CNN
F 3 "" H 3500 18300 50  0001 C CNN
	1    3500 18300
	1    0    0    -1  
$EndComp
Text GLabel 3950 18000 2    50   Input ~ 0
Osc2NoteCV
Text GLabel 3950 18100 2    50   Input ~ 0
Osc2PWMCV
Text GLabel 3950 18200 2    50   Input ~ 0
Osc2SqrMixCV
Text GLabel 3950 18300 2    50   Input ~ 0
Osc2SawMixCV
Text GLabel 3950 18500 2    50   Input ~ 0
SubToOsc2FiltMixCV
Text GLabel 3950 18400 2    50   Input ~ 0
SubToSubFiltMixCV
Text GLabel 3950 18600 2    50   Input ~ 0
NoiseToSubFiltMixCV
Text GLabel 3950 18700 2    50   Input ~ 0
NoiseToOsc2FiltMixCV
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 611104AA
P 7450 14950
F 0 "U?" H 7450 15731 50  0000 C CNN
F 1 "DAC7678xPW" H 7450 15640 50  0000 C CNN
F 2 "" H 7450 14950 50  0001 C CNN
F 3 "" H 7450 14950 50  0001 C CNN
	1    7450 14950
	1    0    0    -1  
$EndComp
Text GLabel 7900 14650 2    50   Input ~ 0
Osc2FiltFreqCV
Text GLabel 7900 14750 2    50   Input ~ 0
Osc2FiltResCV
Text GLabel 7900 14850 2    50   Input ~ 0
SubFiltFreqCV
Text GLabel 7900 14950 2    50   Input ~ 0
SubFiltResCV
Text GLabel 7900 15150 2    50   Input ~ 0
FV1FeedbackCV
Text GLabel 7900 15050 2    50   Input ~ 0
FV1WetDryCV
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 61111B77
P 13550 13650
F 0 "U?" H 13550 14431 50  0000 C CNN
F 1 "DAC7678xPW" H 13550 14340 50  0000 C CNN
F 2 "" H 13550 13650 50  0001 C CNN
F 3 "" H 13550 13650 50  0001 C CNN
	1    13550 13650
	1    0    0    -1  
$EndComp
Text GLabel 14000 13650 2    50   Input ~ 0
OscFiltEnv1ReleaseCV
Text GLabel 14000 13550 2    50   Input ~ 0
OscFiltEnv1SustainCV
Text GLabel 14000 13450 2    50   Input ~ 0
OscFiltEnv1DecayCV
Text GLabel 14000 13350 2    50   Input ~ 0
OscFiltEnv1AttackCV
Text GLabel 14000 14050 2    50   Input ~ 0
OscFiltEnv2ReleaseCV
Text GLabel 14000 13950 2    50   Input ~ 0
OscFiltEnv2SustainCV
Text GLabel 14000 13850 2    50   Input ~ 0
OscFiltEnv2DecayCV
Text GLabel 14000 13750 2    50   Input ~ 0
OscFiltEnv2AttackCV
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 6111305C
P 13550 15200
F 0 "U?" H 13550 15981 50  0000 C CNN
F 1 "DAC7678xPW" H 13550 15890 50  0000 C CNN
F 2 "" H 13550 15200 50  0001 C CNN
F 3 "" H 13550 15200 50  0001 C CNN
	1    13550 15200
	1    0    0    -1  
$EndComp
Text GLabel 14000 15200 2    50   Input ~ 0
OscAmpEnvReleaseCV
Text GLabel 14000 15100 2    50   Input ~ 0
OscAmpEnvSustainCV
Text GLabel 14000 15000 2    50   Input ~ 0
OscAmpEnvDecayCV
Text GLabel 14000 14900 2    50   Input ~ 0
OscAmpEnvAttackCV
Text GLabel 14000 15600 2    50   Input ~ 0
SubAmpEnvReleaseCV
Text GLabel 14000 15500 2    50   Input ~ 0
SubAmpEnvSustainCV
Text GLabel 14000 15400 2    50   Input ~ 0
SubAmpEnvDecayCV
Text GLabel 14000 15300 2    50   Input ~ 0
SubAmpEnvAttackCV
$Comp
L DAC7678:DAC7678xPW U?
U 1 1 61114278
P 13550 16700
F 0 "U?" H 13550 17481 50  0000 C CNN
F 1 "DAC7678xPW" H 13550 17390 50  0000 C CNN
F 2 "" H 13550 16700 50  0001 C CNN
F 3 "" H 13550 16700 50  0001 C CNN
	1    13550 16700
	1    0    0    -1  
$EndComp
Text GLabel 14000 17100 2    50   Input ~ 0
SubFiltEnv1ReleaseCV
Text GLabel 14000 17000 2    50   Input ~ 0
SubFiltEnv1SustainCV
Text GLabel 14000 16900 2    50   Input ~ 0
SubFiltEnv1DecayCV
Text GLabel 14000 16800 2    50   Input ~ 0
SubFiltEnv1AttackCV
Text GLabel 14000 16700 2    50   Input ~ 0
SubFiltEnv2ReleaseCV
Text GLabel 14000 16600 2    50   Input ~ 0
SubFiltEnv2SustainCV
Text GLabel 14000 16500 2    50   Input ~ 0
SubFiltEnv2DecayCV
Text GLabel 14000 16400 2    50   Input ~ 0
SubFiltEnv2AttackCV
Text GLabel 10250 7000 2    50   Input ~ 0
OscFiltEnv1Gate
Text GLabel 10250 6900 2    50   Input ~ 0
OscFiltEnv1Trigger
Text GLabel 10250 7200 2    50   Input ~ 0
OscFiltEnv2Gate
Text GLabel 10250 7100 2    50   Input ~ 0
OscFiltEnv2Trigger
Text GLabel 10250 7400 2    50   Input ~ 0
SubAmpEnvGate
Text GLabel 10250 7300 2    50   Input ~ 0
SubAmpEnvTrigger
Text GLabel 10250 7600 2    50   Input ~ 0
OscAmpEnvGate
Text GLabel 10250 7500 2    50   Input ~ 0
OscAmpEnvTrigger
Text GLabel 10250 7800 2    50   Input ~ 0
SubFiltEnv1Gate
Text GLabel 10250 7700 2    50   Input ~ 0
SubFiltEnv1Trigger
Text GLabel 10250 8000 2    50   Input ~ 0
SubFiltEnv2Gate
Text GLabel 10250 7900 2    50   Input ~ 0
SubFiltEnv2Trigger
Text GLabel 7350 7650 0    50   Input ~ 0
Osc1TuningOut
Text GLabel 7350 7750 0    50   Input ~ 0
Osc2TuningOut
$EndSCHEMATC
