EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
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
Text GLabel 1250 3500 0    50   Input ~ 0
AudioIn
$Comp
L Amplifier_Operational:TL074 U?
U 1 1 5E9CD291
P 2300 1300
F 0 "U?" H 2300 1667 50  0000 C CNN
F 1 "TL074" H 2300 1576 50  0000 C CNN
F 2 "" H 2250 1400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2350 1500 50  0001 C CNN
	1    2300 1300
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 2 1 5E9CE961
P 2300 2800
F 0 "U?" H 2300 3100 50  0000 C CNN
F 1 "TL074" H 2350 3000 50  0000 C CNN
F 2 "" H 2250 2900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2350 3000 50  0001 C CNN
	2    2300 2800
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 3 1 5E9CFBA0
P 2300 4000
F 0 "U?" H 2300 4300 50  0000 C CNN
F 1 "TL074" H 2350 4200 50  0000 C CNN
F 2 "" H 2250 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2350 4200 50  0001 C CNN
	3    2300 4000
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL074 U?
U 4 1 5E9CFE51
P 2300 5350
F 0 "U?" H 2350 5600 50  0000 C CNN
F 1 "TL074" H 2400 5500 50  0000 C CNN
F 2 "" H 2250 5450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 2350 5550 50  0001 C CNN
	4    2300 5350
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
P 1500 3250
F 0 "R?" H 1568 3296 50  0000 L CNN
F 1 "30K" H 1568 3205 50  0000 L CNN
F 2 "" H 1500 3250 50  0001 C CNN
F 3 "~" H 1500 3250 50  0001 C CNN
	1    1500 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E9D5FBC
P 1500 3750
F 0 "R?" H 1568 3796 50  0000 L CNN
F 1 "30K" H 1568 3705 50  0000 L CNN
F 2 "" H 1500 3750 50  0001 C CNN
F 3 "~" H 1500 3750 50  0001 C CNN
	1    1500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3150 1500 2900
Wire Wire Line
	1500 2900 1600 2900
Wire Wire Line
	1500 3350 1500 3500
Wire Wire Line
	1500 3650 1500 3500
Connection ~ 1500 3500
Wire Wire Line
	1500 3850 1500 4100
Wire Wire Line
	1500 4100 1600 4100
$Comp
L power:GND #PWR?
U 1 1 5E9DB71D
P 1950 2700
F 0 "#PWR?" H 1950 2450 50  0001 C CNN
F 1 "GND" V 1955 2572 50  0000 R CNN
F 2 "" H 1950 2700 50  0001 C CNN
F 3 "" H 1950 2700 50  0001 C CNN
	1    1950 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	1950 2700 2000 2700
$Comp
L Device:R_Small_US R?
U 1 1 5E9DC267
P 2400 3150
F 0 "R?" V 2605 3150 50  0000 C CNN
F 1 "30K" V 2514 3150 50  0000 C CNN
F 2 "" H 2400 3150 50  0001 C CNN
F 3 "~" H 2400 3150 50  0001 C CNN
	1    2400 3150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 3150 2500 3150
Wire Wire Line
	1900 3150 1900 2900
Connection ~ 1900 2900
Wire Wire Line
	1900 2900 2000 2900
$Comp
L power:GND #PWR?
U 1 1 5E9DD3B7
P 1950 3900
F 0 "#PWR?" H 1950 3650 50  0001 C CNN
F 1 "GND" V 1955 3772 50  0000 R CNN
F 2 "" H 1950 3900 50  0001 C CNN
F 3 "" H 1950 3900 50  0001 C CNN
	1    1950 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 3900 1950 3900
$Comp
L Device:R_Small_US R?
U 1 1 5E9E5CF8
P 2400 4350
F 0 "R?" V 2195 4350 50  0000 C CNN
F 1 "30K" V 2286 4350 50  0000 C CNN
F 2 "" H 2400 4350 50  0001 C CNN
F 3 "~" H 2400 4350 50  0001 C CNN
	1    2400 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 4350 2500 4350
Wire Wire Line
	1900 4350 1900 4100
Connection ~ 1900 4100
Wire Wire Line
	1900 4100 2000 4100
Text GLabel 3000 2800 2    50   Input ~ 0
FV1LeftIn
Text GLabel 3000 4000 2    50   Input ~ 0
FV1RightIn
Wire Wire Line
	2600 4000 3000 4000
Connection ~ 2600 4000
Wire Wire Line
	2600 2800 3000 2800
Connection ~ 2600 2800
Text GLabel 5150 2800 0    50   Input ~ 0
FV1LeftOut
Text GLabel 5150 4000 0    50   Input ~ 0
FV1RightOut
$Comp
L Analog_VCA:V2164D U?
U 1 1 5E9E8869
P 3500 2100
F 0 "U?" H 3500 1835 50  0000 C CNN
F 1 "V2164D" H 3500 1926 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 4250 1850 50  0001 C CNN
F 3 "" H 3550 2250 50  0001 C CNN
	1    3500 2100
	-1   0    0    1   
$EndComp
$Comp
L Analog_VCA:V2164D U?
U 2 1 5E9EAEF1
P 3500 4850
F 0 "U?" H 3500 4585 50  0000 C CNN
F 1 "V2164D" H 3500 4676 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 4250 4600 50  0001 C CNN
F 3 "" H 3550 5000 50  0001 C CNN
	2    3500 4850
	-1   0    0    1   
$EndComp
$Comp
L Analog_VCA:V2164D U?
U 3 1 5E9EDB34
P 6650 2750
F 0 "U?" H 6650 3025 50  0000 C CNN
F 1 "V2164D" H 6650 2934 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 7400 2500 50  0001 C CNN
F 3 "" H 6700 2900 50  0001 C CNN
	3    6650 2750
	1    0    0    -1  
$EndComp
$Comp
L Analog_VCA:V2164D U?
U 4 1 5E9EE6A7
P 6650 3950
F 0 "U?" H 6650 4225 50  0000 C CNN
F 1 "V2164D" H 6650 4134 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 7400 3700 50  0001 C CNN
F 3 "" H 6700 4100 50  0001 C CNN
	4    6650 3950
	1    0    0    -1  
$EndComp
$Comp
L Analog_VCA:V2164D U?
U 5 1 5E9EF1D6
P 2300 7000
F 0 "U?" H 2258 7021 50  0000 L CNN
F 1 "V2164D" H 2258 6930 50  0000 L CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 3050 6750 50  0001 C CNN
F 3 "" H 2350 7150 50  0001 C CNN
	5    2300 7000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5E9F125A
P 5050 2050
F 0 "R?" V 5150 2050 50  0000 C CNN
F 1 "30K" V 5250 2050 50  0000 C CNN
F 2 "" H 5050 2050 50  0001 C CNN
F 3 "~" H 5050 2050 50  0001 C CNN
	1    5050 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1600 2100 1600 2900
Connection ~ 1600 2900
Wire Wire Line
	1600 2900 1900 2900
Wire Wire Line
	5150 2050 5400 2050
Wire Wire Line
	5400 2050 5400 2800
Wire Wire Line
	5400 2800 5150 2800
Wire Wire Line
	3200 2100 1600 2100
Wire Wire Line
	3800 2050 4050 2050
$Comp
L Device:R_Small_US R?
U 1 1 5EA059F3
P 5050 4800
F 0 "R?" V 5150 4800 50  0000 C CNN
F 1 "30K" V 5250 4800 50  0000 C CNN
F 2 "" H 5050 4800 50  0001 C CNN
F 3 "~" H 5050 4800 50  0001 C CNN
	1    5050 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5150 4000 5400 4000
Wire Wire Line
	5400 4000 5400 4800
Wire Wire Line
	5400 4800 5150 4800
Wire Wire Line
	4950 4800 4050 4800
Wire Wire Line
	3200 4850 1600 4850
Wire Wire Line
	1600 4850 1600 4100
Connection ~ 1600 4100
Wire Wire Line
	1600 4100 1900 4100
$Comp
L power:+5V #PWR?
U 1 1 5EA0C1FA
P 3700 3150
F 0 "#PWR?" H 3700 3000 50  0001 C CNN
F 1 "+5V" H 3715 3323 50  0000 C CNN
F 2 "" H 3700 3150 50  0001 C CNN
F 3 "" H 3700 3150 50  0001 C CNN
	1    3700 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA0D372
P 3700 3650
F 0 "#PWR?" H 3700 3400 50  0001 C CNN
F 1 "GND" H 3705 3477 50  0000 C CNN
F 2 "" H 3700 3650 50  0001 C CNN
F 3 "" H 3700 3650 50  0001 C CNN
	1    3700 3650
	1    0    0    -1  
$EndComp
Connection ~ 4050 4800
Wire Wire Line
	4050 4800 3800 4800
$Comp
L Device:C_Small C?
U 1 1 5EA10605
P 4050 4300
F 0 "C?" H 3850 4350 50  0000 L CNN
F 1 "560pF" H 3700 4250 50  0000 L CNN
F 2 "" H 4050 4300 50  0001 C CNN
F 3 "~" H 4050 4300 50  0001 C CNN
	1    4050 4300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA0E749
P 4050 4600
F 0 "R?" H 3900 4650 50  0000 L CNN
F 1 "500R" H 3800 4550 50  0000 L CNN
F 2 "" H 4050 4600 50  0001 C CNN
F 3 "~" H 4050 4600 50  0001 C CNN
	1    4050 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	4050 4500 4050 4400
$Comp
L power:GND #PWR?
U 1 1 5EA13072
P 4050 4100
F 0 "#PWR?" H 4050 3850 50  0001 C CNN
F 1 "GND" H 4055 3927 50  0000 C CNN
F 2 "" H 4050 4100 50  0001 C CNN
F 3 "" H 4050 4100 50  0001 C CNN
	1    4050 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	4050 4200 4050 4100
Wire Wire Line
	4050 2050 4050 2150
$Comp
L Device:C_Small C?
U 1 1 5EA18B2A
P 4050 2550
F 0 "C?" H 4142 2596 50  0000 L CNN
F 1 "560pF" H 4142 2505 50  0000 L CNN
F 2 "" H 4050 2550 50  0001 C CNN
F 3 "~" H 4050 2550 50  0001 C CNN
	1    4050 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA18B30
P 4050 2250
F 0 "R?" H 4118 2296 50  0000 L CNN
F 1 "500R" H 4118 2205 50  0000 L CNN
F 2 "" H 4050 2250 50  0001 C CNN
F 3 "~" H 4050 2250 50  0001 C CNN
	1    4050 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2350 4050 2450
$Comp
L power:GND #PWR?
U 1 1 5EA18B37
P 4050 2750
F 0 "#PWR?" H 4050 2500 50  0001 C CNN
F 1 "GND" H 4055 2577 50  0000 C CNN
F 2 "" H 4050 2750 50  0001 C CNN
F 3 "" H 4050 2750 50  0001 C CNN
	1    4050 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2650 4050 2750
Wire Wire Line
	4050 2050 4950 2050
$Comp
L Device:C_Small C?
U 1 1 5EA1B3DD
P 2200 3250
F 0 "C?" V 2450 3250 50  0000 C CNN
F 1 "100pF" V 2350 3250 50  0000 C CNN
F 2 "" H 2200 3250 50  0001 C CNN
F 3 "~" H 2200 3250 50  0001 C CNN
	1    2200 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	1900 3150 1900 3250
Wire Wire Line
	1900 3250 2100 3250
Connection ~ 1900 3150
Wire Wire Line
	2300 3250 2600 3250
Wire Wire Line
	2600 3250 2600 3150
Connection ~ 2600 3150
Wire Wire Line
	2600 2800 2600 3150
Wire Wire Line
	1900 3150 2300 3150
$Comp
L Device:C_Small C?
U 1 1 5EA246C3
P 2200 4450
F 0 "C?" V 2450 4450 50  0000 C CNN
F 1 "100pF" V 2350 4450 50  0000 C CNN
F 2 "" H 2200 4450 50  0001 C CNN
F 3 "~" H 2200 4450 50  0001 C CNN
	1    2200 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 4350 1900 4350
Wire Wire Line
	2600 4000 2600 4350
Wire Wire Line
	1900 4350 1900 4450
Wire Wire Line
	1900 4450 2100 4450
Connection ~ 1900 4350
Wire Wire Line
	2300 4450 2600 4450
Wire Wire Line
	2600 4450 2600 4350
Connection ~ 2600 4350
$Comp
L Device:R_Small_US R?
U 1 1 5EA35176
P 5800 4000
F 0 "R?" V 6000 4000 50  0000 C CNN
F 1 "30K" V 5900 4000 50  0000 C CNN
F 2 "" H 5800 4000 50  0001 C CNN
F 3 "~" H 5800 4000 50  0001 C CNN
	1    5800 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA36204
P 5800 2800
F 0 "R?" V 6000 2800 50  0000 C CNN
F 1 "30K" V 5900 2800 50  0000 C CNN
F 2 "" H 5800 2800 50  0001 C CNN
F 3 "~" H 5800 2800 50  0001 C CNN
	1    5800 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	6350 2800 6100 2800
Wire Wire Line
	5900 4000 6100 4000
Wire Wire Line
	6100 2800 6100 2700
$Comp
L Device:C_Small C?
U 1 1 5EA3D8CB
P 6100 2300
F 0 "C?" H 6192 2346 50  0000 L CNN
F 1 "560pF" H 6192 2255 50  0000 L CNN
F 2 "" H 6100 2300 50  0001 C CNN
F 3 "~" H 6100 2300 50  0001 C CNN
	1    6100 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA3D8D1
P 6100 2600
F 0 "R?" H 6168 2646 50  0000 L CNN
F 1 "500R" H 6168 2555 50  0000 L CNN
F 2 "" H 6100 2600 50  0001 C CNN
F 3 "~" H 6100 2600 50  0001 C CNN
	1    6100 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6100 2500 6100 2400
$Comp
L power:GND #PWR?
U 1 1 5EA3D8D8
P 6100 2100
F 0 "#PWR?" H 6100 1850 50  0001 C CNN
F 1 "GND" H 6105 1927 50  0000 C CNN
F 2 "" H 6100 2100 50  0001 C CNN
F 3 "" H 6100 2100 50  0001 C CNN
	1    6100 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6100 2200 6100 2100
Wire Wire Line
	6100 4000 6100 4100
$Comp
L Device:C_Small C?
U 1 1 5EA3F6CF
P 6100 4500
F 0 "C?" H 6192 4546 50  0000 L CNN
F 1 "560pF" H 6192 4455 50  0000 L CNN
F 2 "" H 6100 4500 50  0001 C CNN
F 3 "~" H 6100 4500 50  0001 C CNN
	1    6100 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA3F6D5
P 6100 4200
F 0 "R?" H 6168 4246 50  0000 L CNN
F 1 "500R" H 6168 4155 50  0000 L CNN
F 2 "" H 6100 4200 50  0001 C CNN
F 3 "~" H 6100 4200 50  0001 C CNN
	1    6100 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4300 6100 4400
$Comp
L power:GND #PWR?
U 1 1 5EA3F6DC
P 6100 4700
F 0 "#PWR?" H 6100 4450 50  0001 C CNN
F 1 "GND" H 6105 4527 50  0000 C CNN
F 2 "" H 6100 4700 50  0001 C CNN
F 3 "" H 6100 4700 50  0001 C CNN
	1    6100 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4600 6100 4700
Wire Wire Line
	5700 2800 5400 2800
Wire Wire Line
	5700 4000 5400 4000
Wire Wire Line
	6950 2750 7650 2750
Wire Wire Line
	6950 3950 7650 3950
$Comp
L Device:R_POT_US RV?
U 1 1 5EA4F95D
P 6050 3400
F 0 "RV?" H 5982 3446 50  0000 R CNN
F 1 "100K A" H 5982 3355 50  0000 R CNN
F 2 "" H 6050 3400 50  0001 C CNN
F 3 "~" H 6050 3400 50  0001 C CNN
	1    6050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3400 6200 3900
$Comp
L power:+5V #PWR?
U 1 1 5EA4F966
P 6050 3150
F 0 "#PWR?" H 6050 3000 50  0001 C CNN
F 1 "+5V" H 6065 3323 50  0000 C CNN
F 2 "" H 6050 3150 50  0001 C CNN
F 3 "" H 6050 3150 50  0001 C CNN
	1    6050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3150 6050 3250
Wire Wire Line
	6050 3550 6050 3650
Wire Wire Line
	6200 3400 6200 2700
Wire Wire Line
	6200 2700 6350 2700
Connection ~ 6200 3400
Wire Wire Line
	6200 3900 6350 3900
Wire Wire Line
	3700 3550 3700 3650
Wire Wire Line
	3700 3150 3700 3250
Wire Wire Line
	3850 3400 3850 2150
$Comp
L Device:R_POT_US RV?
U 1 1 5EA09894
P 3700 3400
F 0 "RV?" H 3632 3446 50  0000 R CNN
F 1 "100K A" H 3632 3355 50  0000 R CNN
F 2 "" H 3700 3400 50  0001 C CNN
F 3 "~" H 3700 3400 50  0001 C CNN
	1    3700 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R?
U 1 1 5EA5C939
P 8600 4200
F 0 "R?" V 8395 4200 50  0000 C CNN
F 1 "30K" V 8486 4200 50  0000 C CNN
F 2 "" H 8600 4200 50  0001 C CNN
F 3 "~" H 8600 4200 50  0001 C CNN
	1    8600 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 4200 8700 4200
$Comp
L Device:C_Small C?
U 1 1 5EA5C941
P 8400 4300
F 0 "C?" V 8650 4300 50  0000 C CNN
F 1 "100pF" V 8550 4300 50  0000 C CNN
F 2 "" H 8400 4300 50  0001 C CNN
F 3 "~" H 8400 4300 50  0001 C CNN
	1    8400 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	8500 4200 8100 4200
Wire Wire Line
	8800 3850 8800 4200
Wire Wire Line
	8100 4200 8100 4300
Wire Wire Line
	8100 4300 8300 4300
Wire Wire Line
	8500 4300 8800 4300
Wire Wire Line
	8800 4300 8800 4200
Connection ~ 8800 4200
$Comp
L Device:R_Small_US R?
U 1 1 5EA5EE97
P 8600 3000
F 0 "R?" V 8395 3000 50  0000 C CNN
F 1 "30K" V 8486 3000 50  0000 C CNN
F 2 "" H 8600 3000 50  0001 C CNN
F 3 "~" H 8600 3000 50  0001 C CNN
	1    8600 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 3000 8700 3000
$Comp
L Device:C_Small C?
U 1 1 5EA5EE9F
P 8400 3100
F 0 "C?" V 8650 3100 50  0000 C CNN
F 1 "100pF" V 8550 3100 50  0000 C CNN
F 2 "" H 8400 3100 50  0001 C CNN
F 3 "~" H 8400 3100 50  0001 C CNN
	1    8400 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	8500 3000 8100 3000
Wire Wire Line
	8800 2650 8800 3000
Wire Wire Line
	8100 3000 8100 3100
Wire Wire Line
	8100 3100 8300 3100
Wire Wire Line
	8500 3100 8800 3100
Wire Wire Line
	8800 3100 8800 3000
Connection ~ 8800 3000
$Comp
L power:GND #PWR?
U 1 1 5EA62423
P 8150 2550
F 0 "#PWR?" H 8150 2300 50  0001 C CNN
F 1 "GND" V 8155 2422 50  0000 R CNN
F 2 "" H 8150 2550 50  0001 C CNN
F 3 "" H 8150 2550 50  0001 C CNN
	1    8150 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 2550 8200 2550
$Comp
L power:GND #PWR?
U 1 1 5EA648D1
P 8150 3750
F 0 "#PWR?" H 8150 3500 50  0001 C CNN
F 1 "GND" V 8155 3622 50  0000 R CNN
F 2 "" H 8150 3750 50  0001 C CNN
F 3 "" H 8150 3750 50  0001 C CNN
	1    8150 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 3750 8200 3750
Text GLabel 9500 2650 2    50   Input ~ 0
LeftAudioOut
Text GLabel 9500 3850 2    50   Input ~ 0
RightAudioOut
Wire Wire Line
	8800 2650 9500 2650
Connection ~ 8800 2650
Wire Wire Line
	8800 3850 9500 3850
Connection ~ 8800 3850
Wire Wire Line
	1350 3500 1350 5250
Wire Wire Line
	1500 3500 1350 3500
Wire Wire Line
	1350 3500 1250 3500
Connection ~ 1350 3500
$Comp
L Device:R_Small_US R?
U 1 1 5EA72101
P 7650 4200
F 0 "R?" H 7850 4150 50  0000 R CNN
F 1 "30K" H 7850 4250 50  0000 R CNN
F 2 "" H 7650 4200 50  0001 C CNN
F 3 "~" H 7650 4200 50  0001 C CNN
	1    7650 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 4100 7650 3950
Wire Wire Line
	7650 3950 8100 3950
Connection ~ 7650 3950
Wire Wire Line
	8100 4200 8100 3950
Connection ~ 8100 4200
Connection ~ 8100 3950
Wire Wire Line
	8100 3950 8200 3950
Wire Wire Line
	8100 3000 8100 2750
Connection ~ 8100 3000
Connection ~ 8100 2750
Wire Wire Line
	8100 2750 8200 2750
Connection ~ 7650 2750
Wire Wire Line
	7650 2750 8100 2750
$Comp
L Device:R_Small_US R?
U 1 1 5EA87FD7
P 7650 2500
F 0 "R?" H 7850 2450 50  0000 R CNN
F 1 "30K" H 7850 2550 50  0000 R CNN
F 2 "" H 7650 2500 50  0001 C CNN
F 3 "~" H 7650 2500 50  0001 C CNN
	1    7650 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 2600 7650 2750
$Comp
L Amplifier_Operational:TL072 U?
U 1 1 5EA89205
P 8500 2650
F 0 "U?" H 8500 2950 50  0000 C CNN
F 1 "TL072" H 8550 2850 50  0000 C CNN
F 2 "" H 8500 2650 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8500 2650 50  0001 C CNN
	1    8500 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1300 2700 1300
Connection ~ 6100 2800
Wire Wire Line
	6100 2800 5900 2800
Connection ~ 6100 4000
Wire Wire Line
	6100 4000 6350 4000
Connection ~ 5400 2800
Connection ~ 5400 4000
Connection ~ 4050 2050
Wire Wire Line
	7650 1300 7650 2400
$Comp
L Amplifier_Operational:TL072 U?
U 2 1 5EAF97FA
P 8500 3850
F 0 "U?" H 8500 4150 50  0000 C CNN
F 1 "TL072" H 8550 4050 50  0000 C CNN
F 2 "" H 8500 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8500 3850 50  0001 C CNN
	2    8500 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 4800 4050 4700
Wire Wire Line
	3800 4900 3850 4900
Wire Wire Line
	3800 2150 3850 2150
Wire Wire Line
	3850 3400 3850 4900
Connection ~ 3850 3400
Wire Wire Line
	1950 5450 2000 5450
Wire Wire Line
	2600 5350 7650 5350
Connection ~ 2600 5350
Wire Wire Line
	7650 5350 7650 4300
Wire Wire Line
	1950 1400 1950 1650
Wire Wire Line
	1950 1400 2000 1400
Wire Wire Line
	2700 1650 2700 1300
Connection ~ 2700 1300
Wire Wire Line
	2700 1300 7650 1300
$Comp
L Amplifier_Operational:TL072 U?
U 3 1 5EB81983
P 3350 7000
F 0 "U?" H 3308 7046 50  0000 L CNN
F 1 "TL072" H 3308 6955 50  0000 L CNN
F 2 "" H 3350 7000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 3350 7000 50  0001 C CNN
	3    3350 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1650 2700 1650
Wire Wire Line
	1350 1200 1350 3500
Wire Wire Line
	1350 1200 2000 1200
Wire Wire Line
	1350 5250 2000 5250
Wire Wire Line
	1950 5450 1950 5700
Wire Wire Line
	2600 5350 2600 5700
Wire Wire Line
	2600 5700 1950 5700
$Comp
L power:-5V #PWR?
U 1 1 5EA69D79
P 6050 3650
F 0 "#PWR?" H 6050 3750 50  0001 C CNN
F 1 "-5V" H 6065 3823 50  0000 C CNN
F 2 "" H 6050 3650 50  0001 C CNN
F 3 "" H 6050 3650 50  0001 C CNN
	1    6050 3650
	-1   0    0    1   
$EndComp
$EndSCHEMATC