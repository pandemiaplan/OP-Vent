EESchema Schematic File Version 4
LIBS:Vent-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L Device:C C2
U 1 1 5EBB6383
P 4600 2650
F 0 "C2" H 4715 2696 50  0000 L CNN
F 1 "4.7uF" H 4715 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4638 2500 50  0001 C CNN
F 3 "~" H 4600 2650 50  0001 C CNN
	1    4600 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B_Mini J1
U 1 1 5EBB6389
P 4350 4400
F 0 "J1" H 4407 4867 50  0000 C CNN
F 1 "USB_B_Mini" H 4407 4776 50  0000 C CNN
F 2 "Connector_USB:USB_Mini-B_Lumberg_2486_01_Horizontal" H 4500 4350 50  0001 C CNN
F 3 "~" H 4500 4350 50  0001 C CNN
	1    4350 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5EBB638F
P 4350 4800
F 0 "#PWR0102" H 4350 4550 50  0001 C CNN
F 1 "GND" H 4355 4627 50  0000 C CNN
F 2 "" H 4350 4800 50  0001 C CNN
F 3 "" H 4350 4800 50  0001 C CNN
	1    4350 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4400 4950 4400
Wire Wire Line
	4650 4500 4950 4500
Wire Wire Line
	4650 4200 4800 4200
Wire Wire Line
	4900 4200 4900 4300
Wire Wire Line
	4900 4300 4950 4300
Text Label 4700 4200 0    50   ~ 0
VBUS
$Comp
L Device:C C3
U 1 1 5EBB639B
P 4800 5250
F 0 "C3" H 4915 5296 50  0000 L CNN
F 1 "100nF" H 4915 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4838 5100 50  0001 C CNN
F 3 "~" H 4800 5250 50  0001 C CNN
	1    4800 5250
	1    0    0    -1  
$EndComp
Connection ~ 4800 4200
Wire Wire Line
	4800 4200 4900 4200
Text Label 6200 3800 0    50   ~ 0
RXD
Text Label 6200 3700 0    50   ~ 0
TXD
$Comp
L power:GND #PWR0103
U 1 1 5EBB63A5
P 5550 4900
F 0 "#PWR0103" H 5550 4650 50  0001 C CNN
F 1 "GND" H 5555 4727 50  0000 C CNN
F 2 "" H 5550 4900 50  0001 C CNN
F 3 "" H 5550 4900 50  0001 C CNN
	1    5550 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5EBB63AB
P 5650 4900
F 0 "#PWR0104" H 5650 4650 50  0001 C CNN
F 1 "GND" H 5655 4727 50  0000 C CNN
F 2 "" H 5650 4900 50  0001 C CNN
F 3 "" H 5650 4900 50  0001 C CNN
	1    5650 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4200 4800 4000
Wire Wire Line
	4800 4000 4950 4000
$Comp
L power:GND #PWR0105
U 1 1 5EBB63B5
P 4800 5400
F 0 "#PWR0105" H 4800 5150 50  0001 C CNN
F 1 "GND" H 4805 5227 50  0000 C CNN
F 2 "" H 4800 5400 50  0001 C CNN
F 3 "" H 4800 5400 50  0001 C CNN
	1    4800 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5EBB63BB
P 4350 5250
F 0 "C1" H 4465 5296 50  0000 L CNN
F 1 "4.7uF" H 4465 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4388 5100 50  0001 C CNN
F 3 "~" H 4350 5250 50  0001 C CNN
	1    4350 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5EBB63C1
P 4350 5450
F 0 "#PWR0106" H 4350 5200 50  0001 C CNN
F 1 "GND" H 4355 5277 50  0000 C CNN
F 2 "" H 4350 5450 50  0001 C CNN
F 3 "" H 4350 5450 50  0001 C CNN
	1    4350 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EBB63CD
P 4300 3150
F 0 "R1" H 4370 3196 50  0000 L CNN
F 1 "2.2K" H 4370 3105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4230 3150 50  0001 C CNN
F 3 "~" H 4300 3150 50  0001 C CNN
	1    4300 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3400 4300 3300
Text Label 6150 3500 0    50   ~ 0
GND
$Comp
L Device:C C4
U 1 1 5EBB63D5
P 5050 2650
F 0 "C4" H 5150 2700 50  0000 L CNN
F 1 "100nF" H 5150 2600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5088 2500 50  0001 C CNN
F 3 "~" H 5050 2650 50  0001 C CNN
	1    5050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2500 5050 2500
Wire Wire Line
	5050 2500 5450 2500
Wire Wire Line
	5450 2500 5450 3000
$Comp
L power:GND #PWR0107
U 1 1 5EBB63DE
P 4600 2800
F 0 "#PWR0107" H 4600 2550 50  0001 C CNN
F 1 "GND" H 4605 2627 50  0000 C CNN
F 2 "" H 4600 2800 50  0001 C CNN
F 3 "" H 4600 2800 50  0001 C CNN
	1    4600 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5EBB63E4
P 5050 2800
F 0 "#PWR0108" H 5050 2550 50  0001 C CNN
F 1 "GND" H 5055 2627 50  0000 C CNN
F 2 "" H 5050 2800 50  0001 C CNN
F 3 "" H 5050 2800 50  0001 C CNN
	1    5050 2800
	1    0    0    -1  
$EndComp
Connection ~ 5050 2500
Wire Wire Line
	4300 3400 4950 3400
Connection ~ 5450 2500
Wire Wire Line
	4800 4200 4800 5100
Wire Wire Line
	4350 5100 4800 5100
Connection ~ 4800 5100
Text Notes 5850 5050 0    39   ~ 0
Net TXD \noutput of uP\nis input RXD \nof U1
Text Label 4550 3400 0    50   ~ 0
URST_N
Wire Wire Line
	5450 3000 5550 3000
Wire Wire Line
	5550 3000 5550 3100
Connection ~ 5450 3000
Wire Wire Line
	5450 3000 5450 3100
Text Label 4700 4400 0    50   ~ 0
D+
Text Label 4700 4500 0    50   ~ 0
D-
$Comp
L power:GND #PWR0109
U 1 1 5EBB63F8
P 4250 4800
F 0 "#PWR0109" H 4250 4550 50  0001 C CNN
F 1 "GND" H 4255 4627 50  0000 C CNN
F 2 "" H 4250 4800 50  0001 C CNN
F 3 "" H 4250 4800 50  0001 C CNN
	1    4250 4800
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:CP2102N-A01-GQFN24 U1
U 1 1 5EBB63FE
P 5550 4000
F 0 "U1" H 5550 5081 50  0000 C CNN
F 1 "CP2102N-A01-GQFN24" H 6000 5200 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 6000 3200 50  0001 L CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/cp2102n-datasheet.pdf" H 5600 2950 50  0001 C CNN
	1    5550 4000
	1    0    0    -1  
$EndComp
NoConn ~ 6150 3400
NoConn ~ 6150 3600
NoConn ~ 6150 3900
NoConn ~ 6150 4000
NoConn ~ 6150 4100
NoConn ~ 6150 4300
NoConn ~ 6150 4400
NoConn ~ 6150 4500
NoConn ~ 6150 4600
NoConn ~ 4950 3700
NoConn ~ 4950 3800
NoConn ~ 4650 4600
$Comp
L Device:Jumper_NO_Small JP1
U 1 1 5EBB6410
P 5550 2500
F 0 "JP1" H 5550 2685 50  0000 C CNN
F 1 "USB Power Jumper" H 5550 2594 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5550 2500 50  0001 C CNN
F 3 "~" H 5550 2500 50  0001 C CNN
	1    5550 2500
	1    0    0    -1  
$EndComp
Text Label 4650 2500 0    50   ~ 0
USB_VCC
Wire Wire Line
	4300 3000 4300 2500
Wire Wire Line
	4300 2500 4600 2500
Connection ~ 4600 2500
Text HLabel 3850 1950 0    50   UnSpc ~ 0
VCC
Wire Wire Line
	3850 1950 6250 1950
Wire Wire Line
	6250 1950 6250 2500
Wire Wire Line
	6250 2500 5650 2500
Text Label 5750 2500 0    50   ~ 0
VCC
Text HLabel 3000 5400 0    50   UnSpc ~ 0
GND
Wire Wire Line
	3000 5400 4350 5400
Wire Wire Line
	4350 5400 4350 5450
Connection ~ 4350 5400
Text HLabel 6650 3800 0    50   Output ~ 0
RXD
Text HLabel 6650 3700 0    50   Input ~ 0
TXD
Wire Wire Line
	6150 3700 6650 3700
Wire Wire Line
	6150 3800 6650 3800
Text Notes 8150 7650 0    50   ~ 0
4/28/2020
Text Notes 10650 7650 0    50   ~ 0
1.0
Text Notes 8000 1800 0    50   ~ 0
Copyright 2020 Bill Dally\n \n\nLicensed under the Ventilator Open Source License (the "License");\nyou may not use this file except in compliance with the License.\nYou may obtain a copy of the License at\n\n./LICENSE\n\nUnless required by applicable law or agreed to in writing, software\ndistributed under the License is distributed on an "AS IS" BASIS,\nWITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\nSee the License for the specific language governing permissions and\nlimitations under the License.
$EndSCHEMATC
