EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
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
Text GLabel 950  3500 0    50   Input ~ 0
AudioIn
$Comp
L Amplifier_Operational:TL074 U?
U 1 1 5E9CD291
P 2000 1300
F 0 "U?" H 2000 1667 50  0000 C CNN
F 1 "TL074" H 2000 1576 50  0000 C CNN
F 2 "" H 1950 1400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2050 1500 50  0001 C CNN
	1    2000 1300
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 2 1 5E9CE961
P 2000 2800
F 0 "U?" H 2000 3100 50  0000 C CNN
F 1 "TL074" H 2050 3000 50  0000 C CNN
F 2 "" H 1950 2900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2050 3000 50  0001 C CNN
	2    2000 2800
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 3 1 5E9CFBA0
P 2000 4000
F 0 "U?" H 2000 4300 50  0000 C CNN
F 1 "TL074" H 2050 4200 50  0000 C CNN
F 2 "" H 1950 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2050 4200 50  0001 C CNN
	3    2000 4000
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 4 1 5E9CFE51
P 2000 5350
F 0 "U?" H 2050 5600 50  0000 C CNN
F 1 "TL074" H 2100 5500 50  0000 C CNN
F 2 "" H 1950 5450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2050 5550 50  0001 C CNN
	4    2000 5350
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 5 1 5E9D1F91
P 1250 7000
F 0 "U?" H 1208 7046 50  0000 L CNN
F 1 "TL074" H 1208 6955 50  0000 L CNN
F 2 "" H 1200 7100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 1300 7200 50  0001 C CNN
	5    1250 7000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E9D4419
P 1200 3250
F 0 "R?" H 1268 3296 50  0000 L CNN
F 1 "30K" H 1268 3205 50  0000 L CNN
F 2 "" H 1200 3250 50  0001 C CNN
F 3 "~" H 1200 3250 50  0001 C CNN
	1    1200 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E9D5FBC
P 1200 3750
F 0 "R?" H 1268 3796 50  0000 L CNN
F 1 "30K" H 1268 3705 50  0000 L CNN
F 2 "" H 1200 3750 50  0001 C CNN
F 3 "~" H 1200 3750 50  0001 C CNN
	1    1200 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 3150 1200 2900
Wire Wire Line
	1200 2900 1300 2900
Wire Wire Line
	1200 3350 1200 3500
Wire Wire Line
	1200 3650 1200 3500
Connection ~ 1200 3500
Wire Wire Line
	1200 3850 1200 4100
Wire Wire Line
	1200 4100 1300 4100
$Comp
L power:GND #PWR?
U 1 1 5E9DB71D
P 1650 2700
F 0 "#PWR?" H 1650 2450 50  0001 C CNN
F 1 "GND" V 1655 2572 50  0000 R CNN
F 2 "" H 1650 2700 50  0001 C CNN
F 3 "" H 1650 2700 50  0001 C CNN
	1    1650 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 2700 1700 2700
$Comp
L Device:R_Small_US R?
U 1 1 5E9DC267
P 2100 3150
F 0 "R?" V 2305 3150 50  0000 C CNN
F 1 "30K" V 2214 3150 50  0000 C CNN
F 2 "" H 2100 3150 50  0001 C CNN
F 3 "~" H 2100 3150 50  0001 C CNN
	1    2100 3150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2300 3150 2200 3150
Wire Wire Line
	1600 3150 1600 2900
Connection ~ 1600 2900
Wire Wire Line
	1600 2900 1700 2900
$Comp
L power:GND #PWR?
U 1 1 5E9DD3B7
P 1650 3900
F 0 "#PWR?" H 1650 3650 50  0001 C CNN
F 1 "GND" V 1655 3772 50  0000 R CNN
F 2 "" H 1650 3900 50  0001 C CNN
F 3 "" H 1650 3900 50  0001 C CNN
	1    1650 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 3900 1650 3900
$Comp
L Device:R_Small_US R?
U 1 1 5E9E5CF8
P 2100 4350
F 0 "R?" V 1895 4350 50  0000 C CNN
F 1 "30K" V 1986 4350 50  0000 C CNN
F 2 "" H 2100 4350 50  0001 C CNN
F 3 "~" H 2100 4350 50  0001 C CNN
	1    2100 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 4350 2200 4350
Wire Wire Line
	1600 4350 1600 4100
Connection ~ 1600 4100
Wire Wire Line
	1600 4100 1700 4100
Text GLabel 6300 2600 2    50   Input ~ 0
FV1LeftIn
Text GLabel 6300 3800 2    50   Input ~ 0
FV1RightIn
Text GLabel 9300 2700 0    50   Input ~ 0
FV1LeftOut
Text GLabel 9350 3900 0    50   Input ~ 0
FV1RightOut
$Comp
L FV-1-Feedback-Mix-rescue:V2164D-Analog_VCA-FV-1-Feedback-Mix-rescue U?
U 1 1 5E9E8869
P 7550 2100
F 0 "U?" H 7550 1835 50  0000 C CNN
F 1 "V2164D" H 7550 1926 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 8300 1850 50  0001 C CNN
F 3 "" H 7600 2250 50  0001 C CNN
	1    7550 2100
	-1   0    0    1   
$EndComp
$Comp
L FV-1-Feedback-Mix-rescue:V2164D-Analog_VCA-FV-1-Feedback-Mix-rescue U?
U 2 1 5E9EAEF1
P 7550 4850
F 0 "U?" H 7550 4585 50  0000 C CNN
F 1 "V2164D" H 7550 4676 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 8300 4600 50  0001 C CNN
F 3 "" H 7600 5000 50  0001 C CNN
	2    7550 4850
	-1   0    0    1   
$EndComp
$Comp
L FV-1-Feedback-Mix-rescue:V2164D-Analog_VCA-FV-1-Feedback-Mix-rescue U?
U 3 1 5E9EDB34
P 12250 2750
F 0 "U?" H 12250 3025 50  0000 C CNN
F 1 "V2164D" H 12250 2934 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 13000 2500 50  0001 C CNN
F 3 "" H 12300 2900 50  0001 C CNN
	3    12250 2750
	1    0    0    -1  
$EndComp
$Comp
L FV-1-Feedback-Mix-rescue:V2164D-Analog_VCA-FV-1-Feedback-Mix-rescue U?
U 4 1 5E9EE6A7
P 12250 3950
F 0 "U?" H 12250 4225 50  0000 C CNN
F 1 "V2164D" H 12250 4134 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 13000 3700 50  0001 C CNN
F 3 "" H 12300 4100 50  0001 C CNN
	4    12250 3950
	1    0    0    -1  
$EndComp
$Comp
L FV-1-Feedback-Mix-rescue:V2164D-Analog_VCA-FV-1-Feedback-Mix-rescue U?
U 5 1 5E9EF1D6
P 2850 6950
F 0 "U?" H 2808 6971 50  0000 L CNN
F 1 "V2164D" H 2808 6880 50  0000 L CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 3600 6700 50  0001 C CNN
F 3 "" H 2900 7100 50  0001 C CNN
	5    2850 6950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E9F125A
P 8400 2050
F 0 "R?" V 8500 2050 50  0000 C CNN
F 1 "30K" V 8600 2050 50  0000 C CNN
F 2 "" H 8400 2050 50  0001 C CNN
F 3 "~" H 8400 2050 50  0001 C CNN
	1    8400 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1300 2100 1300 2900
Connection ~ 1300 2900
Wire Wire Line
	1300 2900 1600 2900
Wire Wire Line
	8500 2050 11000 2050
Wire Wire Line
	11000 2050 11000 2800
Wire Wire Line
	11000 2800 10500 2800
Wire Wire Line
	7250 2100 1300 2100
Wire Wire Line
	7850 2050 8100 2050
$Comp
L Device:R_Small_US R?
U 1 1 5EA059F3
P 8400 4800
F 0 "R?" V 8500 4800 50  0000 C CNN
F 1 "30K" V 8600 4800 50  0000 C CNN
F 2 "" H 8400 4800 50  0001 C CNN
F 3 "~" H 8400 4800 50  0001 C CNN
	1    8400 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10400 4000 10500 4000
Wire Wire Line
	11000 4000 11000 4800
Wire Wire Line
	11000 4800 8500 4800
Wire Wire Line
	8300 4800 8100 4800
Wire Wire Line
	7250 4850 1300 4850
Wire Wire Line
	1300 4850 1300 4100
Connection ~ 1300 4100
Wire Wire Line
	1300 4100 1600 4100
$Comp
L power:+5V #PWR?
U 1 1 5EA0C1FA
P 7700 3250
F 0 "#PWR?" H 7700 3100 50  0001 C CNN
F 1 "+5V" H 7715 3423 50  0000 C CNN
F 2 "" H 7700 3250 50  0001 C CNN
F 3 "" H 7700 3250 50  0001 C CNN
	1    7700 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA0D372
P 7700 3750
F 0 "#PWR?" H 7700 3500 50  0001 C CNN
F 1 "GND" H 7705 3577 50  0000 C CNN
F 2 "" H 7700 3750 50  0001 C CNN
F 3 "" H 7700 3750 50  0001 C CNN
	1    7700 3750
	1    0    0    -1  
$EndComp
Connection ~ 8100 4800
Wire Wire Line
	8100 4800 7850 4800
$Comp
L Device:C_Small C?
U 1 1 5EA10605
P 8100 4300
F 0 "C?" H 7900 4350 50  0000 L CNN
F 1 "560pF" H 7750 4250 50  0000 L CNN
F 2 "" H 8100 4300 50  0001 C CNN
F 3 "~" H 8100 4300 50  0001 C CNN
	1    8100 4300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA0E749
P 8100 4600
F 0 "R?" H 7950 4650 50  0000 L CNN
F 1 "500R" H 7850 4550 50  0000 L CNN
F 2 "" H 8100 4600 50  0001 C CNN
F 3 "~" H 8100 4600 50  0001 C CNN
	1    8100 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	8100 4500 8100 4400
$Comp
L power:GND #PWR?
U 1 1 5EA13072
P 8100 4100
F 0 "#PWR?" H 8100 3850 50  0001 C CNN
F 1 "GND" H 8105 3927 50  0000 C CNN
F 2 "" H 8100 4100 50  0001 C CNN
F 3 "" H 8100 4100 50  0001 C CNN
	1    8100 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	8100 4200 8100 4100
Wire Wire Line
	8100 2050 8100 2150
$Comp
L Device:C_Small C?
U 1 1 5EA18B2A
P 8100 2550
F 0 "C?" H 8192 2596 50  0000 L CNN
F 1 "560pF" H 8192 2505 50  0000 L CNN
F 2 "" H 8100 2550 50  0001 C CNN
F 3 "~" H 8100 2550 50  0001 C CNN
	1    8100 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA18B30
P 8100 2250
F 0 "R?" H 8168 2296 50  0000 L CNN
F 1 "500R" H 8168 2205 50  0000 L CNN
F 2 "" H 8100 2250 50  0001 C CNN
F 3 "~" H 8100 2250 50  0001 C CNN
	1    8100 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 2350 8100 2450
$Comp
L power:GND #PWR?
U 1 1 5EA18B37
P 8100 2750
F 0 "#PWR?" H 8100 2500 50  0001 C CNN
F 1 "GND" H 8105 2577 50  0000 C CNN
F 2 "" H 8100 2750 50  0001 C CNN
F 3 "" H 8100 2750 50  0001 C CNN
	1    8100 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 2650 8100 2750
Wire Wire Line
	8100 2050 8300 2050
$Comp
L Device:C_Small C?
U 1 1 5EA1B3DD
P 1900 3250
F 0 "C?" V 2150 3250 50  0000 C CNN
F 1 "100pF" V 2050 3250 50  0000 C CNN
F 2 "" H 1900 3250 50  0001 C CNN
F 3 "~" H 1900 3250 50  0001 C CNN
	1    1900 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	1600 3150 1600 3250
Wire Wire Line
	1600 3250 1800 3250
Connection ~ 1600 3150
Wire Wire Line
	2000 3250 2300 3250
Wire Wire Line
	2300 3250 2300 3150
Connection ~ 2300 3150
Wire Wire Line
	2300 2800 2300 3150
Wire Wire Line
	1600 3150 2000 3150
$Comp
L Device:C_Small C?
U 1 1 5EA246C3
P 1900 4450
F 0 "C?" V 2150 4450 50  0000 C CNN
F 1 "100pF" V 2050 4450 50  0000 C CNN
F 2 "" H 1900 4450 50  0001 C CNN
F 3 "~" H 1900 4450 50  0001 C CNN
	1    1900 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 4350 1600 4350
Wire Wire Line
	2300 4000 2300 4350
Wire Wire Line
	1600 4350 1600 4450
Wire Wire Line
	1600 4450 1800 4450
Connection ~ 1600 4350
Wire Wire Line
	2000 4450 2300 4450
Wire Wire Line
	2300 4450 2300 4350
Connection ~ 2300 4350
$Comp
L Device:R_Small_US R?
U 1 1 5EA35176
P 11400 4000
F 0 "R?" V 11600 4000 50  0000 C CNN
F 1 "30K" V 11500 4000 50  0000 C CNN
F 2 "" H 11400 4000 50  0001 C CNN
F 3 "~" H 11400 4000 50  0001 C CNN
	1    11400 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA36204
P 11400 2800
F 0 "R?" V 11600 2800 50  0000 C CNN
F 1 "30K" V 11500 2800 50  0000 C CNN
F 2 "" H 11400 2800 50  0001 C CNN
F 3 "~" H 11400 2800 50  0001 C CNN
	1    11400 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 2800 11700 2800
Wire Wire Line
	11500 4000 11700 4000
Wire Wire Line
	11700 2800 11700 2700
$Comp
L Device:C_Small C?
U 1 1 5EA3D8CB
P 11700 2300
F 0 "C?" H 11792 2346 50  0000 L CNN
F 1 "560pF" H 11792 2255 50  0000 L CNN
F 2 "" H 11700 2300 50  0001 C CNN
F 3 "~" H 11700 2300 50  0001 C CNN
	1    11700 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA3D8D1
P 11700 2600
F 0 "R?" H 11768 2646 50  0000 L CNN
F 1 "500R" H 11768 2555 50  0000 L CNN
F 2 "" H 11700 2600 50  0001 C CNN
F 3 "~" H 11700 2600 50  0001 C CNN
	1    11700 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	11700 2500 11700 2400
$Comp
L power:GND #PWR?
U 1 1 5EA3D8D8
P 11700 2100
F 0 "#PWR?" H 11700 1850 50  0001 C CNN
F 1 "GND" H 11705 1927 50  0000 C CNN
F 2 "" H 11700 2100 50  0001 C CNN
F 3 "" H 11700 2100 50  0001 C CNN
	1    11700 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	11700 2200 11700 2100
Wire Wire Line
	11700 4000 11700 4100
$Comp
L Device:C_Small C?
U 1 1 5EA3F6CF
P 11700 4500
F 0 "C?" H 11792 4546 50  0000 L CNN
F 1 "560pF" H 11792 4455 50  0000 L CNN
F 2 "" H 11700 4500 50  0001 C CNN
F 3 "~" H 11700 4500 50  0001 C CNN
	1    11700 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA3F6D5
P 11700 4200
F 0 "R?" H 11768 4246 50  0000 L CNN
F 1 "500R" H 11768 4155 50  0000 L CNN
F 2 "" H 11700 4200 50  0001 C CNN
F 3 "~" H 11700 4200 50  0001 C CNN
	1    11700 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	11700 4300 11700 4400
$Comp
L power:GND #PWR?
U 1 1 5EA3F6DC
P 11700 4700
F 0 "#PWR?" H 11700 4450 50  0001 C CNN
F 1 "GND" H 11705 4527 50  0000 C CNN
F 2 "" H 11700 4700 50  0001 C CNN
F 3 "" H 11700 4700 50  0001 C CNN
	1    11700 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11700 4600 11700 4700
Wire Wire Line
	11300 2800 11000 2800
Wire Wire Line
	11300 4000 11000 4000
Wire Wire Line
	12550 2750 13250 2750
Wire Wire Line
	12550 3950 13250 3950
Wire Wire Line
	11800 2700 11950 2700
Wire Wire Line
	11800 3900 11950 3900
Wire Wire Line
	7700 3650 7700 3750
Wire Wire Line
	7700 3250 7700 3350
$Comp
L Device:R_POT_US RV?
U 1 1 5EA09894
P 7700 3500
F 0 "RV?" H 7632 3546 50  0000 R CNN
F 1 "100K A" H 7632 3455 50  0000 R CNN
F 2 "" H 7700 3500 50  0001 C CNN
F 3 "~" H 7700 3500 50  0001 C CNN
	1    7700 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA5C939
P 14200 4200
F 0 "R?" V 13995 4200 50  0000 C CNN
F 1 "30K" V 14086 4200 50  0000 C CNN
F 2 "" H 14200 4200 50  0001 C CNN
F 3 "~" H 14200 4200 50  0001 C CNN
	1    14200 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	14400 4200 14300 4200
$Comp
L Device:C_Small C?
U 1 1 5EA5C941
P 14000 4300
F 0 "C?" V 14250 4300 50  0000 C CNN
F 1 "100pF" V 14150 4300 50  0000 C CNN
F 2 "" H 14000 4300 50  0001 C CNN
F 3 "~" H 14000 4300 50  0001 C CNN
	1    14000 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	14100 4200 13700 4200
Wire Wire Line
	14400 3850 14400 4200
Wire Wire Line
	13700 4200 13700 4300
Wire Wire Line
	13700 4300 13900 4300
Wire Wire Line
	14100 4300 14400 4300
Wire Wire Line
	14400 4300 14400 4200
Connection ~ 14400 4200
$Comp
L Device:R_Small_US R?
U 1 1 5EA5EE97
P 14200 3000
F 0 "R?" V 13995 3000 50  0000 C CNN
F 1 "30K" V 14086 3000 50  0000 C CNN
F 2 "" H 14200 3000 50  0001 C CNN
F 3 "~" H 14200 3000 50  0001 C CNN
	1    14200 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	14400 3000 14300 3000
$Comp
L Device:C_Small C?
U 1 1 5EA5EE9F
P 14000 3100
F 0 "C?" V 14250 3100 50  0000 C CNN
F 1 "100pF" V 14150 3100 50  0000 C CNN
F 2 "" H 14000 3100 50  0001 C CNN
F 3 "~" H 14000 3100 50  0001 C CNN
	1    14000 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	14100 3000 13700 3000
Wire Wire Line
	14400 2650 14400 3000
Wire Wire Line
	13700 3000 13700 3100
Wire Wire Line
	13700 3100 13900 3100
Wire Wire Line
	14100 3100 14400 3100
Wire Wire Line
	14400 3100 14400 3000
Connection ~ 14400 3000
$Comp
L power:GND #PWR?
U 1 1 5EA62423
P 13750 2550
F 0 "#PWR?" H 13750 2300 50  0001 C CNN
F 1 "GND" V 13755 2422 50  0000 R CNN
F 2 "" H 13750 2550 50  0001 C CNN
F 3 "" H 13750 2550 50  0001 C CNN
	1    13750 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	13750 2550 13800 2550
$Comp
L power:GND #PWR?
U 1 1 5EA648D1
P 13750 3750
F 0 "#PWR?" H 13750 3500 50  0001 C CNN
F 1 "GND" V 13755 3622 50  0000 R CNN
F 2 "" H 13750 3750 50  0001 C CNN
F 3 "" H 13750 3750 50  0001 C CNN
	1    13750 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	13750 3750 13800 3750
Text GLabel 15100 2650 2    50   Input ~ 0
LeftAudioOut
Text GLabel 15100 3850 2    50   Input ~ 0
RightAudioOut
Wire Wire Line
	14400 2650 15100 2650
Connection ~ 14400 2650
Wire Wire Line
	14400 3850 15100 3850
Connection ~ 14400 3850
Wire Wire Line
	1050 3500 1050 5450
Wire Wire Line
	1050 5450 1300 5450
Wire Wire Line
	1200 3500 1050 3500
Wire Wire Line
	1050 3500 950  3500
Connection ~ 1050 3500
$Comp
L Device:R_Small_US R?
U 1 1 5EA72101
P 13250 4200
F 0 "R?" H 13450 4150 50  0000 R CNN
F 1 "30K" H 13450 4250 50  0000 R CNN
F 2 "" H 13250 4200 50  0001 C CNN
F 3 "~" H 13250 4200 50  0001 C CNN
	1    13250 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	13250 4100 13250 3950
Wire Wire Line
	13250 3950 13700 3950
Connection ~ 13250 3950
Wire Wire Line
	13700 4200 13700 3950
Connection ~ 13700 4200
Connection ~ 13700 3950
Wire Wire Line
	13700 3950 13800 3950
Wire Wire Line
	13700 3000 13700 2750
Connection ~ 13700 3000
Connection ~ 13700 2750
Wire Wire Line
	13700 2750 13800 2750
Wire Wire Line
	1050 3500 1050 1400
Wire Wire Line
	1050 1400 1300 1400
Connection ~ 13250 2750
Wire Wire Line
	13250 2750 13700 2750
$Comp
L Device:R_Small_US R?
U 1 1 5EA87FD7
P 13250 2500
F 0 "R?" H 13450 2450 50  0000 R CNN
F 1 "30K" H 13450 2550 50  0000 R CNN
F 2 "" H 13250 2500 50  0001 C CNN
F 3 "~" H 13250 2500 50  0001 C CNN
	1    13250 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	13250 2600 13250 2750
$Comp
L Amplifier_Operational:TL072 U?
U 1 1 5EA89205
P 14100 2650
F 0 "U?" H 14100 2950 50  0000 C CNN
F 1 "TL072" H 14150 2850 50  0000 C CNN
F 2 "" H 14100 2650 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 14100 2650 50  0001 C CNN
	1    14100 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1300 2400 1300
$Comp
L power:GND #PWR?
U 1 1 5EAAA679
P 1650 1200
F 0 "#PWR?" H 1650 950 50  0001 C CNN
F 1 "GND" V 1655 1072 50  0000 R CNN
F 2 "" H 1650 1200 50  0001 C CNN
F 3 "" H 1650 1200 50  0001 C CNN
	1    1650 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 1200 1700 1200
Connection ~ 11700 2800
Wire Wire Line
	11700 2800 11500 2800
Connection ~ 11700 4000
Wire Wire Line
	11700 4000 11950 4000
Connection ~ 11000 2800
Connection ~ 11000 4000
Connection ~ 8100 2050
Wire Wire Line
	13250 1300 13250 2400
$Comp
L Amplifier_Operational:TL072 U?
U 2 1 5EAF97FA
P 14100 3850
F 0 "U?" H 14100 4150 50  0000 C CNN
F 1 "TL072" H 14150 4050 50  0000 C CNN
F 2 "" H 14100 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 14100 3850 50  0001 C CNN
	2    14100 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 4800 8100 4700
Wire Wire Line
	7850 4900 7900 4900
Wire Wire Line
	7850 2150 7900 2150
$Comp
L Device:R_Small_US R?
U 1 1 5EB51659
P 1400 5450
F 0 "R?" V 1600 5450 50  0000 C CNN
F 1 "30K" V 1500 5450 50  0000 C CNN
F 2 "" H 1400 5450 50  0001 C CNN
F 3 "~" H 1400 5450 50  0001 C CNN
	1    1400 5450
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 5450 1650 5450
$Comp
L power:GND #PWR?
U 1 1 5EB528E8
P 1650 5250
F 0 "#PWR?" H 1650 5000 50  0001 C CNN
F 1 "GND" V 1655 5122 50  0000 R CNN
F 2 "" H 1650 5250 50  0001 C CNN
F 3 "" H 1650 5250 50  0001 C CNN
	1    1650 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 5250 1700 5250
$Comp
L Device:R_Small_US R?
U 1 1 5EB562F0
P 2100 5700
F 0 "R?" V 1900 5700 50  0000 C CNN
F 1 "30K" V 2000 5700 50  0000 C CNN
F 2 "" H 2100 5700 50  0001 C CNN
F 3 "~" H 2100 5700 50  0001 C CNN
	1    2100 5700
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 5350 2300 5700
Wire Wire Line
	2300 5700 2200 5700
Wire Wire Line
	2000 5700 1650 5700
Connection ~ 1650 5450
Wire Wire Line
	1650 5450 1700 5450
Wire Wire Line
	2300 5350 13250 5350
Wire Wire Line
	13250 5350 13250 4300
$Comp
L Device:R_Small_US R?
U 1 1 5EB6ADA6
P 2150 1650
F 0 "R?" V 1945 1650 50  0000 C CNN
F 1 "30K" V 2036 1650 50  0000 C CNN
F 2 "" H 2150 1650 50  0001 C CNN
F 3 "~" H 2150 1650 50  0001 C CNN
	1    2150 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 1400 1650 1650
Wire Wire Line
	1650 1650 2050 1650
Connection ~ 1650 1400
Wire Wire Line
	1650 1400 1700 1400
Wire Wire Line
	2250 1650 2400 1650
Wire Wire Line
	2400 1650 2400 1300
Wire Wire Line
	2400 1300 13250 1300
$Comp
L Amplifier_Operational:TL072 U?
U 3 1 5EB81983
P 3600 7000
F 0 "U?" H 3558 7046 50  0000 L CNN
F 1 "TL072" H 3558 6955 50  0000 L CNN
F 2 "" H 3600 7000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 3600 7000 50  0001 C CNN
	3    3600 7000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EB953F8
P 1400 1400
F 0 "R?" V 1600 1400 50  0000 C CNN
F 1 "30K" V 1500 1400 50  0000 C CNN
F 2 "" H 1400 1400 50  0001 C CNN
F 3 "~" H 1400 1400 50  0001 C CNN
	1    1400 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 1400 1650 1400
$Comp
L Device:C_Small C?
U 1 1 5EB9A19E
P 1900 1750
F 0 "C?" V 2150 1750 50  0000 C CNN
F 1 "100pF" V 2050 1750 50  0000 C CNN
F 2 "" H 1900 1750 50  0001 C CNN
F 3 "~" H 1900 1750 50  0001 C CNN
	1    1900 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 1750 1650 1750
Wire Wire Line
	1650 1750 1650 1650
Connection ~ 1650 1650
Wire Wire Line
	2000 1750 2400 1750
Wire Wire Line
	2400 1750 2400 1650
Connection ~ 2400 1650
$Comp
L Device:C_Small C?
U 1 1 5EBC0167
P 1900 5800
F 0 "C?" V 2150 5800 50  0000 C CNN
F 1 "100pF" V 2050 5800 50  0000 C CNN
F 2 "" H 1900 5800 50  0001 C CNN
F 3 "~" H 1900 5800 50  0001 C CNN
	1    1900 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 5800 1650 5800
Wire Wire Line
	1650 5450 1650 5700
Connection ~ 1650 5700
Wire Wire Line
	1650 5700 1650 5800
Wire Wire Line
	2000 5800 2300 5800
Wire Wire Line
	2300 5800 2300 5700
Connection ~ 2300 5700
Wire Wire Line
	7900 2150 7900 3500
Connection ~ 7900 3500
Wire Wire Line
	7900 3500 7900 4900
$Comp
L Amplifier_Operational:TL072 U?
U 1 1 5EBB217B
P 10100 2800
F 0 "U?" H 10100 3167 50  0000 C CNN
F 1 "TL072" H 10100 3076 50  0000 C CNN
F 2 "" H 10100 2800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 10100 2800 50  0001 C CNN
	1    10100 2800
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL072 U?
U 2 1 5EBB7C00
P 10100 4000
F 0 "U?" H 10100 4367 50  0000 C CNN
F 1 "TL072" H 10100 4276 50  0000 C CNN
F 2 "" H 10100 4000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 10100 4000 50  0001 C CNN
	2    10100 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 3900 9350 3900
Wire Wire Line
	9800 2700 9300 2700
$Comp
L Device:R_Small_US R?
U 1 1 5EBDD5A8
P 9900 3300
F 0 "R?" V 10105 3300 50  0000 C CNN
F 1 "R_Small_US" V 10014 3300 50  0000 C CNN
F 2 "" H 9900 3300 50  0001 C CNN
F 3 "~" H 9900 3300 50  0001 C CNN
	1    9900 3300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EBDE208
P 9900 4500
F 0 "R?" V 10105 4500 50  0000 C CNN
F 1 "R_Small_US" V 10014 4500 50  0000 C CNN
F 2 "" H 9900 4500 50  0001 C CNN
F 3 "~" H 9900 4500 50  0001 C CNN
	1    9900 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10500 4000 10500 4500
Wire Wire Line
	10500 4500 10000 4500
Connection ~ 10500 4000
Wire Wire Line
	10500 4000 11000 4000
Wire Wire Line
	9800 4500 9550 4500
Wire Wire Line
	9550 4500 9550 4100
Wire Wire Line
	9550 4100 9800 4100
Wire Wire Line
	10500 2800 10500 3300
Wire Wire Line
	10500 3300 10000 3300
Connection ~ 10500 2800
Wire Wire Line
	10500 2800 10400 2800
Wire Wire Line
	9800 3300 9550 3300
Wire Wire Line
	9550 3300 9550 2900
Wire Wire Line
	9550 2900 9800 2900
$Comp
L Device:R_Small_US R?
U 1 1 5EBF82A7
P 9300 2900
F 0 "R?" V 9100 2900 50  0000 C CNN
F 1 "10K" V 9200 2900 50  0000 C CNN
F 2 "" H 9300 2900 50  0001 C CNN
F 3 "~" H 9300 2900 50  0001 C CNN
	1    9300 2900
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EBF9175
P 9300 4100
F 0 "R?" V 9100 4100 50  0000 C CNN
F 1 "10K" V 9200 4100 50  0000 C CNN
F 2 "" H 9300 4100 50  0001 C CNN
F 3 "~" H 9300 4100 50  0001 C CNN
	1    9300 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9550 2900 9400 2900
Connection ~ 9550 2900
Wire Wire Line
	9400 4100 9550 4100
Connection ~ 9550 4100
$Comp
L power:GND #PWR?
U 1 1 5EC078D4
P 9000 4100
F 0 "#PWR?" H 9000 3850 50  0001 C CNN
F 1 "GND" V 9005 3972 50  0000 R CNN
F 2 "" H 9000 4100 50  0001 C CNN
F 3 "" H 9000 4100 50  0001 C CNN
	1    9000 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 4100 9000 4100
$Comp
L power:GND #PWR?
U 1 1 5EC14E7A
P 9000 2900
F 0 "#PWR?" H 9000 2650 50  0001 C CNN
F 1 "GND" V 9005 2772 50  0000 R CNN
F 2 "" H 9000 2900 50  0001 C CNN
F 3 "" H 9000 2900 50  0001 C CNN
	1    9000 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	9000 2900 9200 2900
$Comp
L Amplifier_Operational:TL072 U?
U 3 1 5ECA0085
P 4350 7000
F 0 "U?" H 4308 7046 50  0000 L CNN
F 1 "TL072" H 4308 6955 50  0000 L CNN
F 2 "" H 4350 7000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 4350 7000 50  0001 C CNN
	3    4350 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2800 3100 2800
Connection ~ 2300 2800
Wire Wire Line
	2300 4000 3100 4000
Connection ~ 2300 4000
Wire Wire Line
	7850 3500 7900 3500
$Comp
L Device:R_Small_US R?
U 1 1 5ED03C2F
P 3850 3150
F 0 "R?" V 3645 3150 50  0000 C CNN
F 1 "68K" V 3736 3150 50  0000 C CNN
F 2 "" H 3850 3150 50  0001 C CNN
F 3 "~" H 3850 3150 50  0001 C CNN
	1    3850 3150
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5ED1456C
P 3850 4350
F 0 "R?" V 3645 4350 50  0000 C CNN
F 1 "68K" V 3736 4350 50  0000 C CNN
F 2 "" H 3850 4350 50  0001 C CNN
F 3 "~" H 3850 4350 50  0001 C CNN
	1    3850 4350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5ED56F19
P 3450 2600
F 0 "#PWR?" H 3450 2350 50  0001 C CNN
F 1 "GND" V 3455 2472 50  0000 R CNN
F 2 "" H 3450 2600 50  0001 C CNN
F 3 "" H 3450 2600 50  0001 C CNN
	1    3450 2600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5ED68584
P 3450 3800
F 0 "#PWR?" H 3450 3550 50  0001 C CNN
F 1 "GND" V 3455 3672 50  0000 R CNN
F 2 "" H 3450 3800 50  0001 C CNN
F 3 "" H 3450 3800 50  0001 C CNN
	1    3450 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 3800 3550 3800
$Comp
L Amplifier_Operational:TL074 U?
U 1 1 5EDD93A8
P 3850 2700
F 0 "U?" H 3850 3067 50  0000 C CNN
F 1 "TL074" H 3850 2976 50  0000 C CNN
F 2 "" H 3800 2800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 3900 2900 50  0001 C CNN
	1    3850 2700
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 2 1 5EDDD55C
P 5800 2600
F 0 "U?" H 5800 2967 50  0000 C CNN
F 1 "TL074" H 5800 2876 50  0000 C CNN
F 2 "" H 5750 2700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 5850 2800 50  0001 C CNN
	2    5800 2600
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 3 1 5EDE08C6
P 3850 3900
F 0 "U?" H 3850 4267 50  0000 C CNN
F 1 "TL074" H 3850 4176 50  0000 C CNN
F 2 "" H 3800 4000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 3900 4100 50  0001 C CNN
	3    3850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 2600 3550 2600
Wire Wire Line
	6100 2600 6200 2600
$Comp
L Amplifier_Operational:TL074 U?
U 4 1 5EE09E4D
P 5800 3800
F 0 "U?" H 5800 4167 50  0000 C CNN
F 1 "TL074" H 5800 4076 50  0000 C CNN
F 2 "" H 5750 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 5850 4000 50  0001 C CNN
	4    5800 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3900 4300 3900
Wire Wire Line
	4150 2700 4300 2700
Wire Wire Line
	6100 3800 6200 3800
$Comp
L power:GND #PWR?
U 1 1 5EE5FCB3
P 5350 2500
F 0 "#PWR?" H 5350 2250 50  0001 C CNN
F 1 "GND" V 5355 2372 50  0000 R CNN
F 2 "" H 5350 2500 50  0001 C CNN
F 3 "" H 5350 2500 50  0001 C CNN
	1    5350 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	5350 2500 5500 2500
$Comp
L power:GND #PWR?
U 1 1 5EE68882
P 5350 3700
F 0 "#PWR?" H 5350 3450 50  0001 C CNN
F 1 "GND" V 5355 3572 50  0000 R CNN
F 2 "" H 5350 3700 50  0001 C CNN
F 3 "" H 5350 3700 50  0001 C CNN
	1    5350 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	5350 3700 5500 3700
Wire Wire Line
	3500 4000 3500 4350
Wire Wire Line
	3500 4350 3750 4350
Connection ~ 3500 4000
Wire Wire Line
	3500 4000 3550 4000
Wire Wire Line
	3950 4350 4300 4350
Wire Wire Line
	4300 4350 4300 3900
Wire Wire Line
	4300 3900 5050 3900
Wire Wire Line
	3500 2800 3500 3150
Wire Wire Line
	3500 3150 3750 3150
Connection ~ 3500 2800
Wire Wire Line
	3500 2800 3550 2800
Wire Wire Line
	3950 3150 4300 3150
Wire Wire Line
	4300 3150 4300 2700
Wire Wire Line
	4300 2700 5050 2700
Connection ~ 4300 2700
Connection ~ 4300 3900
$Comp
L Device:R_Small_US R?
U 1 1 5EEF2DBC
P 3200 4000
F 0 "R?" V 3400 4000 50  0000 C CNN
F 1 "100K" V 3300 4000 50  0000 C CNN
F 2 "" H 3200 4000 50  0001 C CNN
F 3 "~" H 3200 4000 50  0001 C CNN
	1    3200 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 4000 3500 4000
$Comp
L Device:R_Small_US R?
U 1 1 5EEF3D1C
P 3200 2800
F 0 "R?" V 3400 2800 50  0000 C CNN
F 1 "100K" V 3300 2800 50  0000 C CNN
F 2 "" H 3200 2800 50  0001 C CNN
F 3 "~" H 3200 2800 50  0001 C CNN
	1    3200 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 2800 3500 2800
$Comp
L Device:R_Small_US R?
U 1 1 5EEF484A
P 5800 3150
F 0 "R?" V 5595 3150 50  0000 C CNN
F 1 "30K" V 5686 3150 50  0000 C CNN
F 2 "" H 5800 3150 50  0001 C CNN
F 3 "~" H 5800 3150 50  0001 C CNN
	1    5800 3150
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EEF4CC7
P 5800 4350
F 0 "R?" V 5595 4350 50  0000 C CNN
F 1 "30K" V 5686 4350 50  0000 C CNN
F 2 "" H 5800 4350 50  0001 C CNN
F 3 "~" H 5800 4350 50  0001 C CNN
	1    5800 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 3900 5450 4350
Wire Wire Line
	5450 4350 5700 4350
Connection ~ 5450 3900
Wire Wire Line
	5450 3900 5500 3900
Wire Wire Line
	5900 4350 6200 4350
Wire Wire Line
	6200 4350 6200 3800
Connection ~ 6200 3800
Wire Wire Line
	6200 3800 6300 3800
Wire Wire Line
	5450 2700 5450 3150
Wire Wire Line
	5450 3150 5700 3150
Connection ~ 5450 2700
Wire Wire Line
	5450 2700 5500 2700
Wire Wire Line
	5900 3150 6200 3150
Wire Wire Line
	6200 3150 6200 2600
Connection ~ 6200 2600
Wire Wire Line
	6200 2600 6300 2600
$Comp
L Device:R_Small_US R?
U 1 1 5EF3041B
P 5150 2700
F 0 "R?" V 5350 2700 50  0000 C CNN
F 1 "30K" V 5250 2700 50  0000 C CNN
F 2 "" H 5150 2700 50  0001 C CNN
F 3 "~" H 5150 2700 50  0001 C CNN
	1    5150 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 2700 5450 2700
$Comp
L Device:R_Small_US R?
U 1 1 5EF30CC5
P 5150 3900
F 0 "R?" V 5350 3900 50  0000 C CNN
F 1 "30K" V 5250 3900 50  0000 C CNN
F 2 "" H 5150 3900 50  0001 C CNN
F 3 "~" H 5150 3900 50  0001 C CNN
	1    5150 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 3900 5450 3900
Connection ~ 2400 1300
Connection ~ 2300 5350
$Comp
L Amplifier_Operational:TL074 U?
U 5 1 5EF81E4F
P 2100 7000
F 0 "U?" H 2058 7046 50  0000 L CNN
F 1 "TL074" H 2058 6955 50  0000 L CNN
F 2 "" H 2050 7100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2150 7200 50  0001 C CNN
	5    2100 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	11800 2700 11800 3400
Wire Wire Line
	11750 3400 11800 3400
Connection ~ 11800 3400
Wire Wire Line
	11800 3400 11800 3900
Wire Wire Line
	11600 3550 11600 3650
$Comp
L power:GND #PWR?
U 1 1 5EA4F96D
P 11600 3650
F 0 "#PWR?" H 11600 3400 50  0001 C CNN
F 1 "GND" H 11605 3477 50  0000 C CNN
F 2 "" H 11600 3650 50  0001 C CNN
F 3 "" H 11600 3650 50  0001 C CNN
	1    11600 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	11600 3150 11600 3250
$Comp
L power:+5V #PWR?
U 1 1 5EA4F966
P 11600 3150
F 0 "#PWR?" H 11600 3000 50  0001 C CNN
F 1 "+5V" H 11615 3323 50  0000 C CNN
F 2 "" H 11600 3150 50  0001 C CNN
F 3 "" H 11600 3150 50  0001 C CNN
	1    11600 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT_US RV?
U 1 1 5EA4F95D
P 11600 3400
F 0 "RV?" H 11532 3446 50  0000 R CNN
F 1 "100K A" H 11532 3355 50  0000 R CNN
F 2 "" H 11600 3400 50  0001 C CNN
F 3 "~" H 11600 3400 50  0001 C CNN
	1    11600 3400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
