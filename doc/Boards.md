# Boards
Application notes.

## Powerboard

## Motherboard
The motherboard features:

- An STM32 microcontroller featuring a Cortex-M3 CPU
- A 5V or 3.3V tolerant power supply
- 1 debug UART connector
- 1 GPS UART connector
- 1 MAVLink-clone telemetry OSD UART connector
- 1 10-DOF IMU
- 1 SRF-04 ultrasound range sensor connector
- 4 ESC PWM connectors
- 4 generic PWM connectors (e.g. for a camera gimbal servo-motors)
- 1 I2C *peripheral* connector (I2C1)
- 2 I2C *system* connectors (I2C2)

I2C1 is shared with the 10-DOF IMU board.


### PCB design
- All capacitors should be *at least* 3.3V tolerant
- C8 should be *at least* 5V tolerant (it's the power supply decoupling cap)
- C1, C2 (STM32 internal power regulator external caps) are recommended with an ESR < 2 ohms.
- C12, C13 values needs to be chosen according to Y1 load capacitance (e.g. for *LFXTAL010595*, series load capacitance of 16pF : C12, C13, the PCB and the I/O pins series capacitance must match 16pF; assuming ~10pF for the PCB trace and the I/O pin capacitance [AN4448 4.1.2] we choose C12 and C13 = 16pF*2 - 10pF = 22pF)

### Boot
The boot memory is selected through the two dip-switches alongside the reset button:

| 1   | 2   | Mode  |
|-----|-----|-------|
| OFF |  -  | Flash |
| ON  | OFF | System|
| ON  | ON  | SRAM  |

The STM32 *system* memory holds a factory-programmed bootloader.

### Input voltage
The motherboard can be configured to support a 3.3V regulated power supply *OR* a 5V regulated power supply.

In 3.3V mode, Vdd directly feeds the STM32 microcontroller and the various peripherals. The 5V external peripherals (SRF-04 ultrasound sensor, MAVLink-compatible telemetry OSD) **will need an external 5V power supply**.

In 5V mode, Vdd is regulated by a linear regulator on the board to provide 3.3V to the STM32 and 3.3V peripherals, and Vdd is fed directly to the 5V external peripherals.

#### 5V setup
- Fit C8, C16
- Fit IC1
- Leave JP23 open

#### 3.3V setup
- ***do not fit*** IC1
- Short JP23
- C8 and C16 are not mandatory but can provide additional decoupling

### Supercap
The supercap C14 supplies the STM32 Vbat when Vdd is off, to keep the RTC running and to keep the SRAM data live.

The supercap is not mandatory. If it not used:

- ***do not fit*** D1, R6, C14
- Short JP2

If the supercap is used:

- Fit D1, R6, C14
- Leave JP2 open


### JTAG scan-chain

Two additional daughter cards can be connected to the motherboard to form a single JTAG scan-chain. JP1 is the master JTAG plug where the JTAG programmer/debugger is connected and *shall be left open for the JTAG programmer*.

The daughtercards can be connecter to JP3 and JP4. If JP3 is not used, JP6 shall be short in order to close the scan-chain. Similarly, if JP4 is not used, JP7 shall be short.

Additionnally, in order to match the *firmware/opencd/* scripts, JP3 shall be connected to the Powerboard, and JP4 to the RF board.