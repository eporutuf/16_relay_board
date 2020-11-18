# 16_relay_board
Communication:
-UDP Message-	-Details-
FF0101	Relay 1 ON command (ASCII encoding)
FF0100	Relay 1 OFF command (ASCII encoding)
...
FF1600	Relay 16 OFF command (ASCII encoding)
FF1601	Relay 16 ON command (ASCII encoding)
FFE000	All relays OFF command (ASCII encoding)
FFE0FF	All relays ON command (ASCII encoding)
FFT000	Get temperature value (only available with "sensors" board option)
FFC000	Get current value (only available with sensors relay board option)

Features:
STM32F411RE Microprocessor, 32-Bit ARM Cortex
W5500 Ethernet module
16 Relays Output
1x Jack connector
ACS711 current sensors
2x16 Terminals fast connector 24 AWG

Specification:
DC Current 100μA/MHz
Clock speed up to 100 MHz
512KB Flash Memory
128KB SRAM
Ethernet 802.3 10/100 Mbit/s
Relays outputs: 16 x 3A maximum
Input Voltage: (Vin) 16-30V
Dimension: 200mm x 80mm x 35mm
Weight: 250 g