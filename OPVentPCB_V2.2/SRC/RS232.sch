EESchema Schematic File Version 4
LIBS:Vent-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Interface_UART:MAX3232 U?
U 1 1 5EE55DBF
P 5250 3850
AR Path="/5EE55DBF" Ref="U?"  Part="1" 
AR Path="/5EE51193/5EE55DBF" Ref="U4"  Part="1" 
F 0 "U4" H 5400 5050 50  0000 C CNN
F 1 "MAX3232" H 5500 4950 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 5300 2800 50  0001 L CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX3222-MAX3241.pdf" H 5250 3950 50  0001 C CNN
	1    5250 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3950 4450 3950
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5EE55DC8
P 6800 4200
AR Path="/5EE55DC8" Ref="J?"  Part="1" 
AR Path="/5EE51193/5EE55DC8" Ref="J9"  Part="1" 
F 0 "J9" H 6750 4450 50  0000 L CNN
F 1 "RS232" H 6600 3950 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6800 4200 50  0001 C CNN
F 3 "~" H 6800 4200 50  0001 C CNN
	1    6800 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EE55DCE
P 5250 5050
AR Path="/5EE55DCE" Ref="#PWR?"  Part="1" 
AR Path="/5EE51193/5EE55DCE" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 5250 4800 50  0001 C CNN
F 1 "GND" H 5255 4877 50  0000 C CNN
F 2 "" H 5250 5050 50  0001 C CNN
F 3 "" H 5250 5050 50  0001 C CNN
	1    5250 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 4200 6600 4200
Wire Wire Line
	6600 4100 6450 4100
Wire Wire Line
	6450 4100 6450 4650
$Comp
L power:GND #PWR?
U 1 1 5EE55DD7
P 6450 5050
AR Path="/5EE55DD7" Ref="#PWR?"  Part="1" 
AR Path="/5EE51193/5EE55DD7" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 6450 4800 50  0001 C CNN
F 1 "GND" H 6455 4877 50  0000 C CNN
F 2 "" H 6450 5050 50  0001 C CNN
F 3 "" H 6450 5050 50  0001 C CNN
	1    6450 5050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5EE55DDD
P 5250 2650
AR Path="/5EE55DDD" Ref="#PWR?"  Part="1" 
AR Path="/5EE51193/5EE55DDD" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 5250 2500 50  0001 C CNN
F 1 "VCC" H 5267 2823 50  0000 C CNN
F 2 "" H 5250 2650 50  0001 C CNN
F 3 "" H 5250 2650 50  0001 C CNN
	1    5250 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5EE55DE3
P 4300 3100
AR Path="/5EE55DE3" Ref="C?"  Part="1" 
AR Path="/5EE51193/5EE55DE3" Ref="C10"  Part="1" 
F 0 "C10" H 4415 3146 50  0000 L CNN
F 1 "100nF" H 4415 3055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4338 2950 50  0001 C CNN
F 3 "~" H 4300 3100 50  0001 C CNN
	1    4300 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5EE55DE9
P 6050 3100
AR Path="/5EE55DE9" Ref="C?"  Part="1" 
AR Path="/5EE51193/5EE55DE9" Ref="C11"  Part="1" 
F 0 "C11" H 6165 3146 50  0000 L CNN
F 1 "100nF" H 6165 3055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6088 2950 50  0001 C CNN
F 3 "~" H 6050 3100 50  0001 C CNN
	1    6050 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5EE55DEF
P 6300 3700
AR Path="/5EE55DEF" Ref="C?"  Part="1" 
AR Path="/5EE51193/5EE55DEF" Ref="C12"  Part="1" 
F 0 "C12" H 6415 3746 50  0000 L CNN
F 1 "100nF" H 6415 3655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6338 3550 50  0001 C CNN
F 3 "~" H 6300 3700 50  0001 C CNN
	1    6300 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 4200 6250 4350
Wire Wire Line
	6250 4350 6050 4350
Wire Wire Line
	6050 3950 6150 3950
Wire Wire Line
	6150 3950 6150 4300
Wire Wire Line
	6150 4300 6600 4300
$Comp
L Device:C C?
U 1 1 5EE55DFA
P 6650 3700
AR Path="/5EE55DFA" Ref="C?"  Part="1" 
AR Path="/5EE51193/5EE55DFA" Ref="C14"  Part="1" 
F 0 "C14" H 6765 3746 50  0000 L CNN
F 1 "100nF" H 6765 3655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6688 3550 50  0001 C CNN
F 3 "~" H 6650 3700 50  0001 C CNN
	1    6650 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3450 6650 3450
Wire Wire Line
	6650 3450 6650 3550
Wire Wire Line
	6300 3550 6050 3550
Wire Wire Line
	6050 3550 6050 3750
$Comp
L power:GND #PWR?
U 1 1 5EE55E04
P 6650 3850
AR Path="/5EE55E04" Ref="#PWR?"  Part="1" 
AR Path="/5EE51193/5EE55E04" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 6650 3600 50  0001 C CNN
F 1 "GND" H 6655 3677 50  0000 C CNN
F 2 "" H 6650 3850 50  0001 C CNN
F 3 "" H 6650 3850 50  0001 C CNN
	1    6650 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3850 6650 3850
Connection ~ 6650 3850
Text Notes 5800 2700 0    50   ~ 0
RS232 Interface - Optional\nShort jumper to connect on\nUSART0 or 1\n
Wire Wire Line
	3950 4150 4450 4150
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5EE55E1D
P 6800 4750
AR Path="/5EE55E1D" Ref="J?"  Part="1" 
AR Path="/5EE51193/5EE55E1D" Ref="J10"  Part="1" 
F 0 "J10" H 6750 5000 50  0000 L CNN
F 1 "RS232B" H 6600 4500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6800 4750 50  0001 C CNN
F 3 "~" H 6800 4750 50  0001 C CNN
	1    6800 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4650 6600 4650
Connection ~ 6450 4650
Wire Wire Line
	6450 4650 6450 5050
Wire Wire Line
	6050 4550 6150 4550
Wire Wire Line
	6150 4550 6150 4750
Wire Wire Line
	6150 4750 6600 4750
Wire Wire Line
	6050 4150 6350 4150
Wire Wire Line
	6350 4150 6350 4850
Wire Wire Line
	6350 4850 6600 4850
Text Label 6300 3450 0    50   ~ 0
VS+
Text Label 6100 3550 0    50   ~ 0
VS-
Text Label 6100 3950 0    50   ~ 0
RT1
Text Label 6200 4150 0    50   ~ 0
RT2
Text Label 6100 4350 0    50   ~ 0
RR1
Text Label 6150 4750 0    50   ~ 0
RR2
Text Label 6050 2950 0    50   ~ 0
C2+
Text Label 6050 3250 0    50   ~ 0
C2-
Text Label 4300 2950 0    50   ~ 0
C1+
Text Label 4300 3250 0    50   ~ 0
C1-
Wire Wire Line
	4300 3250 4450 3250
Wire Wire Line
	4300 2950 4450 2950
Text HLabel 4150 2650 0    50   UnSpc ~ 0
VCC
Wire Wire Line
	4150 2650 5250 2650
Connection ~ 5250 2650
Wire Wire Line
	5250 5000 4000 5000
Text HLabel 4000 5000 0    50   UnSpc ~ 0
GND
Text HLabel 3950 3950 0    50   Input ~ 0
TXD
Text HLabel 3950 4150 0    50   Input ~ 0
TXD1
Text HLabel 3950 4350 0    50   Output ~ 0
RXD
Text HLabel 3950 4550 0    50   Output ~ 0
RXD1
Wire Wire Line
	3950 4350 4450 4350
Wire Wire Line
	3950 4550 4450 4550
Text Label 4100 3950 0    50   ~ 0
STXD
Text Label 4100 4150 0    50   ~ 0
STXD1
Text Label 4100 4350 0    50   ~ 0
SRXD
Text Label 4100 4550 0    50   ~ 0
SRXD1
Text Notes 7950 1850 0    50   ~ 0
Copyright 2020 Bill Dally\n \n\nLicensed under the Ventilator Open Source License (the "License");\nyou may not use this file except in compliance with the License.\nYou may obtain a copy of the License at\n\n./LICENSE\n\nUnless required by applicable law or agreed to in writing, software\ndistributed under the License is distributed on an "AS IS" BASIS,\nWITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\nSee the License for the specific language governing permissions and\nlimitations under the License.
Text Notes 7450 7500 0    50   ~ 0
RS232 Interface
Text Notes 8150 7650 0    50   ~ 0
4/28/2020
Text Notes 10600 7650 0    50   ~ 0
1.0\n
$EndSCHEMATC
