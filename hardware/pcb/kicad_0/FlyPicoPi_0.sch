EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Fly Pico Pi"
Date "6.4.2021"
Rev "1.0"
Comp "Flipotronics"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Isolator:6N138 U5
U 1 1 606D2399
P 1900 1050
F 0 "U5" H 1900 1517 50  0000 C CNN
F 1 "6N138" H 1900 1426 50  0000 C CNN
F 2 "" H 2190 750 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/HCPL2731-D.pdf" H 2190 750 50  0001 C CNN
	1    1900 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 606ECFB5
P 1450 950
F 0 "R1" V 1657 950 50  0000 C CNN
F 1 "220" V 1566 950 50  0000 C CNN
F 2 "" V 1380 950 50  0001 C CNN
F 3 "~" H 1450 950 50  0001 C CNN
	1    1450 950 
	0    -1   -1   0   
$EndComp
$Comp
L Connector:DIN-5_180degree J5
U 1 1 606CCA60
P 850 950
F 0 "J5" H 850 675 50  0000 C CNN
F 1 "DIN-5_180degree" H 850 584 50  0000 C CNN
F 2 "" H 850 950 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 850 950 50  0001 C CNN
	1    850  950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 950  1300 550 
Wire Wire Line
	1300 550  550  550 
Wire Wire Line
	550  550  550  850 
Wire Wire Line
	1150 850  1250 850 
Wire Wire Line
	1250 850  1250 1150
Wire Wire Line
	1250 1150 1600 1150
$Comp
L power:GND #PWR0101
U 1 1 606F70CF
P 2750 1250
F 0 "#PWR0101" H 2750 1000 50  0001 C CNN
F 1 "GND" H 2755 1077 50  0000 C CNN
F 2 "" H 2750 1250 50  0001 C CNN
F 3 "" H 2750 1250 50  0001 C CNN
	1    2750 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 1250 2750 1250
$Comp
L power:+5V #PWR0102
U 1 1 606F836D
P 2750 850
F 0 "#PWR0102" H 2750 700 50  0001 C CNN
F 1 "+5V" H 2765 1023 50  0000 C CNN
F 2 "" H 2750 850 50  0001 C CNN
F 3 "" H 2750 850 50  0001 C CNN
	1    2750 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 850  2750 850 
$Comp
L power:+3.3V #PWR0103
U 1 1 606F90A9
P 3100 850
F 0 "#PWR0103" H 3100 700 50  0001 C CNN
F 1 "+3.3V" H 3115 1023 50  0000 C CNN
F 2 "" H 3100 850 50  0001 C CNN
F 3 "" H 3100 850 50  0001 C CNN
	1    3100 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 606FA1B6
P 2350 950
F 0 "R2" V 2557 950 50  0000 C CNN
F 1 "220" V 2466 950 50  0000 C CNN
F 2 "" V 2280 950 50  0001 C CNN
F 3 "~" H 2350 950 50  0001 C CNN
	1    2350 950 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2500 950  3100 950 
$Comp
L Device:R R3
U 1 1 606FB7D2
P 2350 1150
F 0 "R3" V 2557 1150 50  0000 C CNN
F 1 "4.7K" V 2466 1150 50  0000 C CNN
F 2 "" V 2280 1150 50  0001 C CNN
F 3 "~" H 2350 1150 50  0001 C CNN
	1    2350 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2500 1150 2750 1150
Wire Wire Line
	2750 1150 2750 1250
Connection ~ 2750 1250
Wire Wire Line
	3100 750  3100 850 
Connection ~ 3100 850 
Wire Wire Line
	3100 850  3100 950 
Wire Wire Line
	2750 850  2750 1000
Connection ~ 2750 850 
Wire Wire Line
	8300 4200 8500 4200
$Comp
L Connector:AudioJack2 J1
U 1 1 6071C2BF
P 750 5700
F 0 "J1" H 782 6025 50  0000 C CNN
F 1 "CV 1" H 782 5934 50  0000 C CNN
F 2 "" H 750 5700 50  0001 C CNN
F 3 "~" H 750 5700 50  0001 C CNN
	1    750  5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J2
U 1 1 6071D6F4
P 750 6300
F 0 "J2" H 782 6625 50  0000 C CNN
F 1 "Gate 1" H 782 6534 50  0000 C CNN
F 2 "" H 750 6300 50  0001 C CNN
F 3 "~" H 750 6300 50  0001 C CNN
	1    750  6300
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J3
U 1 1 6071E907
P 750 6950
F 0 "J3" H 782 7275 50  0000 C CNN
F 1 "CV 2" H 782 7184 50  0000 C CNN
F 2 "" H 750 6950 50  0001 C CNN
F 3 "~" H 750 6950 50  0001 C CNN
	1    750  6950
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J4
U 1 1 6071F847
P 750 7500
F 0 "J4" H 782 7825 50  0000 C CNN
F 1 "Gate 2" H 782 7734 50  0000 C CNN
F 2 "" H 750 7500 50  0001 C CNN
F 3 "~" H 750 7500 50  0001 C CNN
	1    750  7500
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U1
U 1 1 60723EC7
P 1700 5700
F 0 "U1" H 1700 5333 50  0000 C CNN
F 1 "LM358" H 1700 5424 50  0000 C CNN
F 2 "" H 1700 5700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1700 5700 50  0001 C CNN
	1    1700 5700
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:LM358 U2
U 1 1 607311DF
P 1700 6300
F 0 "U2" H 1700 5933 50  0000 C CNN
F 1 "LM358" H 1700 6024 50  0000 C CNN
F 2 "" H 1700 6300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1700 6300 50  0001 C CNN
	1    1700 6300
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:LM358 U3
U 1 1 60733838
P 1700 6950
F 0 "U3" H 1700 6583 50  0000 C CNN
F 1 "LM358" H 1700 6674 50  0000 C CNN
F 2 "" H 1700 6950 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1700 6950 50  0001 C CNN
	1    1700 6950
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:LM358 U4
U 1 1 60736A1A
P 1700 7500
F 0 "U4" H 1700 7133 50  0000 C CNN
F 1 "LM358" H 1700 7224 50  0000 C CNN
F 2 "" H 1700 7500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1700 7500 50  0001 C CNN
	1    1700 7500
	-1   0    0    1   
$EndComp
Wire Wire Line
	950  5700 1400 5700
Wire Wire Line
	950  6300 1400 6300
Wire Wire Line
	950  6950 1400 6950
Wire Wire Line
	950  7500 1400 7500
Wire Wire Line
	950  5600 950  5150
Wire Wire Line
	950  5150 2250 5150
Wire Wire Line
	2250 5150 2250 6050
Wire Wire Line
	2250 6050 950  6050
Wire Wire Line
	950  6050 950  6200
Wire Wire Line
	2250 6050 2250 6700
Wire Wire Line
	2250 6700 950  6700
Wire Wire Line
	950  6700 950  6850
Connection ~ 2250 6050
Wire Wire Line
	2250 6700 2250 7250
Wire Wire Line
	2250 7250 950  7250
Wire Wire Line
	950  7250 950  7400
Connection ~ 2250 6700
Wire Wire Line
	2250 7250 2750 7250
Wire Wire Line
	2750 7250 2750 1250
Connection ~ 2250 7250
$Comp
L Analog_DAC:MCP4725xxx-xCH U6
U 1 1 60756F68
P 3900 5650
F 0 "U6" H 4344 5696 50  0000 L CNN
F 1 "MCP4725xxx-xCH" H 4344 5605 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 3900 5400 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/22039d.pdf" H 3900 5650 50  0001 C CNN
	1    3900 5650
	1    0    0    -1  
$EndComp
$Comp
L Analog_DAC:MCP4725xxx-xCH U7
U 1 1 6075796E
P 3900 6900
F 0 "U7" H 4344 6946 50  0000 L CNN
F 1 "MCP4725xxx-xCH" H 4344 6855 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 3900 6650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/22039d.pdf" H 3900 6900 50  0001 C CNN
	1    3900 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 5800 3200 5800
Wire Wire Line
	3200 5800 3200 6150
Wire Wire Line
	3200 6150 4300 6150
Wire Wire Line
	4300 6150 4300 5650
Wire Wire Line
	4300 6900 4300 7400
Wire Wire Line
	4300 7400 3200 7400
Wire Wire Line
	3200 7400 3200 7050
Wire Wire Line
	3200 7050 2000 7050
$Comp
L MCU_RaspberryPi_and_Boards:Pico U10
U 1 1 606CF40D
P 6900 2300
F 0 "U10" H 6900 3605 50  0000 C CNN
F 1 "Pico" H 6900 3514 50  0000 C CNN
F 2 "footprints:RPi_Pico_SMD_TH" H 6900 3423 50  0000 C CNN
F 3 "" H 6900 2300 50  0001 C CNN
	1    6900 2300
	1    0    0    -1  
$EndComp
$Comp
L MCU_RaspberryPi_Audio:Pico_Audio U8
U 1 1 606CFC48
P 4700 2350
F 0 "U8" H 4700 3565 50  0000 C CNN
F 1 "Pico_Audio" H 4700 3474 50  0000 C CNN
F 2 "footprints:RPi_Pico_SMD_TH_AUDIO" V 4700 2350 50  0001 C CNN
F 3 "" H 4700 2350 50  0001 C CNN
	1    4700 2350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
