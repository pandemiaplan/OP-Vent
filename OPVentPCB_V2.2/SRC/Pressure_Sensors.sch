EESchema Schematic File Version 4
LIBS:Vent-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L Sensor_Pressure:MP3V5004GP U5
U 1 1 5EC8737F
P 2500 1650
F 0 "U5" H 2000 1450 50  0000 R CNN
F 1 "MP3V5010GP" H 2400 1350 50  0000 R CNN
F 2 "Sensor_Pressure:NXP_Pressure_Sensor" H 2000 1300 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 2500 2250 50  0001 C CNN
	1    2500 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5EC8738B
P 2500 2250
F 0 "#PWR0115" H 2500 2000 50  0001 C CNN
F 1 "GND" H 2505 2077 50  0000 C CNN
F 2 "" H 2500 2250 50  0001 C CNN
F 3 "" H 2500 2250 50  0001 C CNN
	1    2500 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1650 3100 1650
Connection ~ 2500 1350
Wire Wire Line
	2500 1950 2500 2250
Connection ~ 2500 2250
$Comp
L Device:C C22
U 1 1 5EC873A0
P 1700 1650
F 0 "C22" H 1815 1696 50  0000 L CNN
F 1 "100nF" H 1815 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1738 1500 50  0001 C CNN
F 3 "~" H 1700 1650 50  0001 C CNN
	1    1700 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1500 1700 1350
Wire Wire Line
	1700 1350 2500 1350
Wire Wire Line
	1700 1800 1700 2250
Wire Wire Line
	1700 2250 2500 2250
$Comp
L Sensor_Pressure:NBPMANN150PGUNV_copy U7
U 1 1 5EC873B6
P 3300 3450
F 0 "U7" H 2850 3300 50  0000 R CNN
F 1 "NBPMANN150PGUNV" H 3200 3200 50  0000 R CNN
F 2 "Sensor_Pressure:Honeywell_NBP" H 2800 3100 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 3300 4050 50  0001 C CNN
	1    3300 3450
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Instrumentation:MCP6N11 U10
U 1 1 5EC873BC
P 4450 3450
F 0 "U10" H 4600 3750 50  0000 L CNN
F 1 "MCP6N11" H 4600 3650 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4350 3450 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD623.pdf" H 4900 3050 50  0001 C CNN
	1    4450 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3100 3300 3100
Wire Wire Line
	3300 3100 3300 3150
Connection ~ 3300 3100
Wire Wire Line
	3300 3750 3300 3850
Wire Wire Line
	3300 3850 4350 3850
Wire Wire Line
	4350 3850 4350 3800
$Comp
L power:GND #PWR0116
U 1 1 5EC873C8
P 3300 3850
F 0 "#PWR0116" H 3300 3600 50  0001 C CNN
F 1 "GND" H 3305 3677 50  0000 C CNN
F 2 "" H 3300 3850 50  0001 C CNN
F 3 "" H 3300 3850 50  0001 C CNN
	1    3300 3850
	1    0    0    -1  
$EndComp
Connection ~ 3300 3850
Wire Wire Line
	3700 3350 3800 3350
Wire Wire Line
	3800 3350 3800 3650
Wire Wire Line
	3800 3650 4150 3650
Wire Wire Line
	3700 3550 3950 3550
Wire Wire Line
	3950 3550 3950 3250
Wire Wire Line
	3950 3250 4150 3250
Wire Wire Line
	4350 3850 4450 3850
Wire Wire Line
	4450 3850 4450 3750
Connection ~ 4350 3850
Wire Wire Line
	4350 3100 4550 3100
Wire Wire Line
	4550 3100 4550 3200
Connection ~ 4350 3100
Wire Wire Line
	4950 3450 5000 3450
$Comp
L Device:R R8
U 1 1 5EC873DC
P 5000 3600
F 0 "R8" H 5070 3646 50  0000 L CNN
F 1 "220k" H 5070 3555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4930 3600 50  0001 C CNN
F 3 "~" H 5000 3600 50  0001 C CNN
	1    5000 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5EC873E2
P 5000 3900
F 0 "R9" H 5070 3946 50  0000 L CNN
F 1 "10k" H 5070 3855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4930 3900 50  0001 C CNN
F 3 "~" H 5000 3900 50  0001 C CNN
	1    5000 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 3850 4450 4050
Wire Wire Line
	4450 4050 5000 4050
Connection ~ 4450 3850
Connection ~ 5000 3450
Wire Wire Line
	5000 3450 5350 3450
Wire Wire Line
	4650 3650 4650 3750
Wire Wire Line
	4650 3750 5000 3750
Connection ~ 5000 3750
Wire Wire Line
	2500 3300 2500 3100
Wire Wire Line
	2500 3100 3300 3100
Wire Wire Line
	2500 3600 2500 3850
Wire Wire Line
	2500 3850 3300 3850
Text Notes 3900 800  0    50   ~ 0
NXP sensor barbed connector\nOn side of Pin 1 and 8 \nOrient that to edge of board
Text Label 2950 1650 0    50   ~ 0
LP1R
Text Label 9600 1650 0    50   ~ 0
LP3R
Text Label 5050 3450 0    50   ~ 0
HP
Text Notes 700  750  0    50   ~ 0
Pressure Sensors\n0-3 of 5 May be populated
$Comp
L Sensor_Pressure:2SMPP-03 U13
U 1 1 5EC8740F
P 3400 5450
F 0 "U13" H 2970 5496 50  0000 R CNN
F 1 "2SMPP-03" H 2970 5405 50  0000 R CNN
F 2 "Sensor_Pressure:2SMPP" H 2900 5100 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 3400 6050 50  0001 C CNN
	1    3400 5450
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Instrumentation:MCP6N11 U14
U 1 1 5EC87415
P 4350 5450
F 0 "U14" H 4500 5750 50  0000 L CNN
F 1 "MCP6N11" H 4500 5650 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4250 5450 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD623.pdf" H 4800 5050 50  0001 C CNN
	1    4350 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 5550 4000 5550
Wire Wire Line
	4000 5550 4000 5250
Wire Wire Line
	4000 5250 4050 5250
Wire Wire Line
	3800 5350 3850 5350
Wire Wire Line
	3850 5350 3850 5650
Wire Wire Line
	3850 5650 4050 5650
Wire Wire Line
	4450 5200 4450 5100
Wire Wire Line
	4450 5100 4250 5100
Text Label 4250 5100 0    50   ~ 0
VCC
$Comp
L Device:R R13
U 1 1 5EC87424
P 4850 5600
F 0 "R13" H 4920 5646 50  0000 L CNN
F 1 "220k" H 4920 5555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4780 5600 50  0001 C CNN
F 3 "~" H 4850 5600 50  0001 C CNN
	1    4850 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5EC8742A
P 4850 5900
F 0 "R14" H 4920 5946 50  0000 L CNN
F 1 "10k" H 4920 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4780 5900 50  0001 C CNN
F 3 "~" H 4850 5900 50  0001 C CNN
	1    4850 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 6050 4850 6050
Wire Wire Line
	4550 5750 4850 5750
Connection ~ 4850 5750
Wire Wire Line
	4850 5450 5200 5450
Connection ~ 4850 5450
Text Label 4900 5450 0    50   ~ 0
LP4
Text Label 2550 3100 0    50   ~ 0
VCC
Wire Wire Line
	4550 5750 4550 5650
Wire Wire Line
	4350 6050 4350 5750
Text Label 3800 6050 0    50   ~ 0
GND
$Comp
L Amplifier_Operational:LMV358 U?
U 1 1 5EC8743A
P 2400 5150
AR Path="/5EC8743A" Ref="U?"  Part="1" 
AR Path="/5EC3F39D/5EC8743A" Ref="U12"  Part="1" 
F 0 "U12" H 2450 5300 50  0000 C CNN
F 1 "LMV358" H 2400 5150 50  0000 C CNN
F 2 "" H 2400 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv324.pdf" H 2400 5150 50  0001 C CNN
	1    2400 5150
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LMV358 U12
U 2 1 5EC87440
P 2400 5900
F 0 "U12" H 2450 6050 50  0000 C CNN
F 1 "LMV358" H 2400 5900 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 2400 5900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv324.pdf" H 2400 5900 50  0001 C CNN
	2    2400 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5EC87446
P 3400 5900
F 0 "R12" H 3470 5946 50  0000 L CNN
F 1 "10k" H 3470 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3330 5900 50  0001 C CNN
F 3 "~" H 3400 5900 50  0001 C CNN
	1    3400 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 6050 4250 6050
Wire Wire Line
	4250 6050 4250 5800
Wire Wire Line
	2700 5150 3250 5150
Connection ~ 3250 5150
Wire Wire Line
	3250 5150 3400 5150
Wire Wire Line
	2900 5750 3400 5750
Connection ~ 3400 5750
$Comp
L Device:R R11
U 1 1 5EC87453
P 1500 5200
F 0 "R11" H 1570 5246 50  0000 L CNN
F 1 "100k" H 1570 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1430 5200 50  0001 C CNN
F 3 "~" H 1500 5200 50  0001 C CNN
	1    1500 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5EC87459
P 1500 4900
F 0 "R10" H 1570 4946 50  0000 L CNN
F 1 "330k" H 1570 4855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1430 4900 50  0001 C CNN
F 3 "~" H 1500 4900 50  0001 C CNN
	1    1500 4900
	1    0    0    -1  
$EndComp
Connection ~ 1500 5050
Text Label 1500 4750 0    50   ~ 0
VCC
Text Label 1500 5350 0    50   ~ 0
GND
Wire Wire Line
	2900 5550 2900 5750
$Comp
L Amplifier_Operational:LMV358 U12
U 3 1 5EC87463
P 2400 5150
F 0 "U12" H 2100 5450 50  0000 L CNN
F 1 "LMV358" H 2250 5150 50  0000 L CNN
F 2 "" H 2400 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv324.pdf" H 2400 5150 50  0001 C CNN
	3    2400 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 5250 2100 5550
Text Label 2300 4850 0    50   ~ 0
VCC
Text Label 2300 5450 0    50   ~ 0
GND
Wire Wire Line
	2100 5550 2900 5550
Wire Wire Line
	2100 6000 2100 6150
Wire Wire Line
	2100 6150 2700 6150
Wire Wire Line
	2700 6150 2700 5900
Wire Wire Line
	2700 6150 4350 6150
Wire Wire Line
	4350 6150 4350 6050
Connection ~ 2700 6150
Connection ~ 4350 6050
Wire Wire Line
	1500 5050 2000 5050
Text Label 1900 5050 0    50   ~ 0
V1V
Wire Wire Line
	2000 5050 2000 5800
Wire Wire Line
	2000 5800 2100 5800
Connection ~ 2000 5050
Wire Wire Line
	2000 5050 2100 5050
Text Label 3850 5650 0    50   ~ 0
DLP+
Text Label 3900 5550 0    50   ~ 0
DLP-
Text Label 3850 3650 0    50   ~ 0
HP+
Text Label 3850 3550 0    50   ~ 0
HP-
Text Label 4700 3750 0    50   ~ 0
HPM
Text Label 4600 5750 0    50   ~ 0
DLPM
Text Label 2850 6150 0    50   ~ 0
DLPREF
Text Label 2150 5550 0    50   ~ 0
DLB
Text Label 2750 5150 0    50   ~ 0
DLT
Text Notes 1150 3450 0    50   ~ 0
U12A is current source to bias \n2SMPP-03 with 100uA\nU12B is voltage source to set\nREF of U14 at 1V
Text HLabel 900  3100 0    50   UnSpc ~ 0
VCC
Text HLabel 900  3850 0    50   UnSpc ~ 0
GND
Wire Wire Line
	900  3850 2500 3850
Connection ~ 2500 3850
Wire Wire Line
	900  3100 2500 3100
Connection ~ 2500 3100
Text HLabel 3950 1950 0    50   Output ~ 0
LP1
Text HLabel 8000 1950 0    50   Output ~ 0
LP2
Text HLabel 10450 1950 0    50   Output ~ 0
LP3
Text HLabel 5350 3450 0    50   Output ~ 0
HP
Text HLabel 5200 5450 0    50   Output ~ 0
LP4
$Comp
L Device:C C15
U 1 1 5EC873F0
P 2500 3450
F 0 "C15" H 2615 3496 50  0000 L CNN
F 1 "100nF" H 2615 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2538 3300 50  0001 C CNN
F 3 "~" H 2500 3450 50  0001 C CNN
	1    2500 3450
	1    0    0    -1  
$EndComp
Text HLabel 1650 900  0    50   UnSpc ~ 0
V5
$Comp
L Device:C C13
U 1 1 5F28B3B6
P 5750 1650
F 0 "C13" H 5865 1696 50  0000 L CNN
F 1 "100nF" H 5865 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5788 1500 50  0001 C CNN
F 3 "~" H 5750 1650 50  0001 C CNN
	1    5750 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 900  2050 900 
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5F2B4025
P 2150 900
AR Path="/5F2B4025" Ref="JP?"  Part="1" 
AR Path="/5EC3F39D/5F2B4025" Ref="JP6"  Part="1" 
F 0 "JP6" H 2150 1085 50  0000 C CNN
F 1 "LP1_5V" H 2150 994 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_TrianglePad1.0x1.5mm" H 2150 900 50  0001 C CNN
F 3 "~" H 2150 900 50  0001 C CNN
	1    2150 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 1200 2050 1200
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5F2B63FA
P 2150 1200
AR Path="/5F2B63FA" Ref="JP?"  Part="1" 
AR Path="/5EC3F39D/5F2B63FA" Ref="JP7"  Part="1" 
F 0 "JP7" H 2150 1385 50  0000 C CNN
F 1 "LP1_3P3V" H 2150 1294 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged_Pad1.0x1.5mm" H 2150 1200 50  0001 C CNN
F 3 "~" H 2150 1200 50  0001 C CNN
	1    2150 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1200 2500 1350
Wire Wire Line
	2250 1200 2500 1200
Wire Wire Line
	2500 900  2500 1200
Wire Wire Line
	2250 900  2500 900 
Text Label 2350 900  0    50   ~ 0
VP1
Text Label 1700 1200 0    50   ~ 0
VCC
$Comp
L Device:R R17
U 1 1 5F2C1BF5
P 3450 1800
F 0 "R17" H 3520 1846 50  0000 L CNN
F 1 "5.1k" H 3520 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3380 1800 50  0001 C CNN
F 3 "~" H 3450 1800 50  0001 C CNN
	1    3450 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 5F2C1BFF
P 3450 2100
F 0 "R18" H 3520 2146 50  0000 L CNN
F 1 "10k" H 3520 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3380 2100 50  0001 C CNN
F 3 "~" H 3450 2100 50  0001 C CNN
	1    3450 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2250 3100 2250
Wire Wire Line
	3450 1950 3950 1950
Connection ~ 3450 1950
Text Notes 3000 1400 0    50   ~ 0
For 3.3V sensor no divider\nFor 5V sensor 1:1.94 R ratio
Connection ~ 2500 1200
$Comp
L Sensor_Pressure:MP3V5004GP U11
U 1 1 5EC873AA
P 9000 1650
F 0 "U11" H 8500 1450 50  0000 R CNN
F 1 "MP3V5010GP" H 8900 1350 50  0000 R CNN
F 2 "Sensor_Pressure:NXP_Pressure_Sensor" H 8500 1300 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 9000 2250 50  0001 C CNN
	1    9000 1650
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Pressure:MPXV5004GP U9
U 1 1 5EC87392
P 6550 1650
F 0 "U9" H 6100 1450 50  0000 R CNN
F 1 "MPXV5010GP" H 6500 1350 50  0000 R CNN
F 2 "Sensor_Pressure:NXP_Pressure_Sensor" H 6050 1300 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 6550 2250 50  0001 C CNN
	1    6550 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EA4D76D
P 6550 2250
F 0 "#PWR02" H 6550 2000 50  0001 C CNN
F 1 "GND" H 6555 2077 50  0000 C CNN
F 2 "" H 6550 2250 50  0001 C CNN
F 3 "" H 6550 2250 50  0001 C CNN
	1    6550 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 1950 6550 2250
Connection ~ 6550 2250
Wire Wire Line
	5750 1500 5750 1350
Wire Wire Line
	5750 1350 6550 1350
Wire Wire Line
	5750 1800 5750 2250
Wire Wire Line
	5750 2250 6550 2250
Text Label 7000 1650 0    50   ~ 0
LP2R
Text HLabel 5700 900  0    50   UnSpc ~ 0
V5
Wire Wire Line
	5700 900  6100 900 
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5EA4D78D
P 6200 900
AR Path="/5EA4D78D" Ref="JP?"  Part="1" 
AR Path="/5EC3F39D/5EA4D78D" Ref="JP8"  Part="1" 
F 0 "JP8" H 6200 1085 50  0000 C CNN
F 1 "LP2_5V" H 6200 994 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_TrianglePad1.0x1.5mm" H 6200 900 50  0001 C CNN
F 3 "~" H 6200 900 50  0001 C CNN
	1    6200 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1200 6100 1200
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 5EA4D798
P 6200 1200
AR Path="/5EA4D798" Ref="JP?"  Part="1" 
AR Path="/5EC3F39D/5EA4D798" Ref="JP9"  Part="1" 
F 0 "JP9" H 6200 1385 50  0000 C CNN
F 1 "LP2_3P3V" H 6200 1294 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged_Pad1.0x1.5mm" H 6200 1200 50  0001 C CNN
F 3 "~" H 6200 1200 50  0001 C CNN
	1    6200 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 1200 6550 1350
Wire Wire Line
	6300 1200 6550 1200
Wire Wire Line
	6550 900  6550 1200
Wire Wire Line
	6300 900  6550 900 
Text Label 6400 900  0    50   ~ 0
VP2
Text Label 5750 1200 0    50   ~ 0
VCC
$Comp
L Device:R R19
U 1 1 5EA4D7A8
P 7500 1800
F 0 "R19" H 7570 1846 50  0000 L CNN
F 1 "5.1k" H 7570 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7430 1800 50  0001 C CNN
F 3 "~" H 7500 1800 50  0001 C CNN
	1    7500 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5EA4D7B2
P 7500 2100
F 0 "R20" H 7570 2146 50  0000 L CNN
F 1 "10k" H 7570 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7430 2100 50  0001 C CNN
F 3 "~" H 7500 2100 50  0001 C CNN
	1    7500 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2250 7150 2250
Wire Wire Line
	7500 1950 8000 1950
Connection ~ 7500 1950
Text Notes 7100 1100 0    50   ~ 0
For 3.3V sensor no divider\nFor 5V sensor 1:1.94 R ratio
Connection ~ 6550 1200
Connection ~ 6550 1350
Wire Wire Line
	6950 1650 7150 1650
Wire Wire Line
	6550 1350 9000 1350
Wire Wire Line
	7500 2250 9000 2250
Wire Wire Line
	9000 2250 9000 1950
Connection ~ 7500 2250
$Comp
L Device:R R21
U 1 1 5EA6A0AF
P 9950 1800
F 0 "R21" H 10020 1846 50  0000 L CNN
F 1 "5.1k" H 10020 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9880 1800 50  0001 C CNN
F 3 "~" H 9950 1800 50  0001 C CNN
	1    9950 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 5EA6A0B9
P 9950 2100
F 0 "R22" H 10020 2146 50  0000 L CNN
F 1 "10k" H 10020 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9880 2100 50  0001 C CNN
F 3 "~" H 9950 2100 50  0001 C CNN
	1    9950 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2250 9600 2250
Wire Wire Line
	9950 1950 10450 1950
Connection ~ 9950 1950
Wire Wire Line
	9400 1650 9600 1650
Connection ~ 9000 2250
Text Label 9050 3350 0    50   ~ 0
GND
Text Notes 8350 3600 0    50   ~ 0
Additional footprint option for LP4 sensor MPX10GP or DP.  \nNeeds U14 for amplification.\n
$Comp
L Sensor_Pressure:MPX10GP U17
U 1 1 5EA7A0BE
P 9050 3050
F 0 "U17" H 8600 2900 50  0000 R CNN
F 1 "MX10GP" H 8950 2800 50  0000 R CNN
F 2 "DigiKey:SIP-4_W3.81mm" H 8550 2700 50  0001 C CNN
F 3 "http://www.nxp.com/files/sensors/doc/data_sheet/MPXA6115A.pdf" H 9050 3650 50  0001 C CNN
	1    9050 3050
	1    0    0    -1  
$EndComp
Text Label 9050 2750 0    50   ~ 0
VCC
Text Label 9450 2950 0    50   ~ 0
DLP+
Text Label 9450 3150 0    50   ~ 0
DLP-
Text Notes 8000 5900 0    50   ~ 0
Copyright 2020 Bill Dally\n \n\nLicensed under the Ventilator Open Source License (the "License");\nyou may not use this file except in compliance with the License.\nYou may obtain a copy of the License at\n\n./LICENSE\n\nUnless required by applicable law or agreed to in writing, software\ndistributed under the License is distributed on an "AS IS" BASIS,\nWITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\nSee the License for the specific language governing permissions and\nlimitations under the License.
Text Notes 8150 7650 0    50   ~ 0
4/28/2020
Text Notes 10600 7650 0    50   ~ 0
2.1
$Comp
L Device:C C24
U 1 1 5EB3E0A8
P 3100 1950
F 0 "C24" H 2900 1950 50  0000 L CNN
F 1 "470pF" H 2800 1850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3138 1800 50  0001 C CNN
F 3 "~" H 3100 1950 50  0001 C CNN
	1    3100 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 2100 3100 2250
Connection ~ 3100 2250
Wire Wire Line
	3100 2250 3450 2250
Wire Wire Line
	3100 1800 3100 1650
Connection ~ 3100 1650
Wire Wire Line
	3100 1650 3450 1650
$Comp
L Device:C C25
U 1 1 5EB43E29
P 7150 1950
F 0 "C25" H 6950 1950 50  0000 L CNN
F 1 "470pF" H 6850 1850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7188 1800 50  0001 C CNN
F 3 "~" H 7150 1950 50  0001 C CNN
	1    7150 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 2100 7150 2250
Wire Wire Line
	7150 1800 7150 1650
Connection ~ 7150 1650
Wire Wire Line
	7150 1650 7500 1650
Connection ~ 7150 2250
Wire Wire Line
	7150 2250 7500 2250
$Comp
L Device:C C26
U 1 1 5EB476AE
P 9600 1950
F 0 "C26" H 9400 1950 50  0000 L CNN
F 1 "470pF" H 9300 1850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9638 1800 50  0001 C CNN
F 3 "~" H 9600 1950 50  0001 C CNN
	1    9600 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2100 9600 2250
Wire Wire Line
	9600 1800 9600 1650
Connection ~ 9600 1650
Wire Wire Line
	9600 1650 9950 1650
Connection ~ 9600 2250
Wire Wire Line
	9600 2250 9950 2250
$EndSCHEMATC
