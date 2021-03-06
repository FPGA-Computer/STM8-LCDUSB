#STM8-LCD2USB

The firmware only USB stack is an unofficial fork from STM8S-VUSB-KEYBOARD at https://github.com/BBS215/STM8S-VUSB-KEYBOARD

- Russian comments "google translated" to English.  Minimal changes to the original are in .zip file.

- Works with current special edition of Cosmic C (4.4.10) and current version of STVD (4.3.12)

STM8 LCD2USB

- LCD2USB port from ATMega8 to STM8S003. It is working with LCD Smarties.  Only 1 LCD controller supported i.e. no support for larger display. If the LCD is accessed with delays (Not implemented) instead of polling, the GPIO for R/W can be repurposed as Enable pin (E).

LCD2USB at https://github.com/harbaum/LCD2USB  (LCD2USB.dll)

LCD Smarties at http://lcdsmartie.sourceforge.net/

- USB stack logic is modified for this tech demo. Vendor Request packets (needed by LCD2USB) is working.  HID now tested (STM8 HID Dial).

- Code reformatted/refractored.  Using my own IRQ files for STM8S003 for a much cleaner organization.  

- No longer need a separate GPIO for the 1.5K pull up to connect/disconnect USB.  See schematic.

- Device library calls have been changed to bare metal to minimized dependancy and reduce code size.  stm8s_flash.c is the only library needed.

- "Port C pins can only be used as GPIO inputs" is no longer the case. They can be configured as PWM and force them Low/High via CCMRx registers.  See hardware.h

- The PCB is a general purpose single sided PCB. Serial port, I2C are broken out to headers, pads for tiny crystal and load capacitors available for oscillator. 
The buttons are also used as jumpers for Gnd.  Install 2 jumpers under buttons S1 and S2 if they are not used.  LCD mounting holes and pinouts are for common 1602A from China.

Note: 

The USB stack calibrates the HSI clock while connected to the PC. The calibration is store in EEPROM. It can take a couple of minutes the first time before it functions correctly.

Timer 1 is used by the USB stack for filtering and interrupt. It is not actually used for counting.

R7 value control brightness.  I use R7 = 330R for my green display and 2.7K for my white ones.

Changes:

- New STM8_LCD2USB.dll for 3 buttons support. I took a bit of effort for me to learn to compile a .DLL, so I have included source + solution file for vs_Community (Visual Studio Community 2019
Version 16.7.2) with C++ (2019).  I am using libusb-win32-bin-1.2.7.1.zip to create this.

- Buttons: return by $MObutton

  * Long press 'OR' 0x10 to the individual keys values.

  * Multiple keys pressed at the same time have their values 'OR' together

Action|Menu (S1)| Inc (S2)|Dec (S3)
------|---------|---------|--------
Quick press|1|4|2
Long Press|A|D|B

V1.1

- VUSB library is updated for HID.  Logic changes and more refractoring.  It is used in my other project: STM8 HID Dial.
