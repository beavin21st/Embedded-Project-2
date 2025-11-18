📌 ECU Real-Time Monitoring System Using PIC16F887

This project implements a Real-Time Engine/Fan Runtime Monitoring System similar to an automotive ECU (Electronic Control Unit).
It continuously monitors ON-time, stores runtime in EEPROM, displays a real-time clock using RTC, and provides user input via a keypad.

 Project Overview

This embedded system:

    ✔ Measures and displays fan/engine runtime (HH:MM:SS)
    ✔ Stores the runtime in internal EEPROM (non-volatile)
    ✔ Uses DS1307 RTC to show Real-Time Clock
    ✔ Allows user to set time through a keypad
    ✔ Uses Timer1 interrupt for 1-second timing
    ✔ Includes LED alert pattern (first 4 sec OFF → then 1s ON/OFF alternation)
    ✔ Works exactly like an automotive ECU runtime counter

 Hardware Components
 
      Component	                                                                    Purpose
      PIC16F887 Microcontroller                                                    	Main controller, timers, EEPROM, I2C
      DS1307 RTC Module                                                            	Provides accurate time using I²C
      16x2 LCD Display                                                             	Shows RTC time + Fan runtime
      Matrix Keypad (3×4)                                                          	Set HH:MM:SS
      Internal EEPROM                                                              	Stores runtime across power OFF
      Timer1 Module                                                                	Generates 1-second interrupts
      LEDs (RA4, RA5)                                                              	System indication pattern
      Push Button (RA0)                                                            	Fan ON/OFF toggle
      Push Button (RA1, RA2)                                                      	Set RTC / Reset EEPROM
      5V Power Supply	                                                              System power

⚙️ Features

    🔹 1. Real-Time Clock (RTC) Display
    
      Reads time from DS1307 via I²C and displays:
    
      RTC – HH:MM:SS
    
    🔹 2. Fan Runtime Monitoring
    
      Counts total runtime:
    
      Continues counting when fan is ON
    
      Stops when fan is OFF
    
      Saves current HH:MM:SS in EEPROM
    
    🔹 3. EEPROM-Based Backup
    
      Even after power restart, runtime continues from the saved value.
    
    🔹 4. LED Pattern Logic
      0–4 seconds → Both LEDs OFF  
      ≥ 4 seconds → LED1 & LED2 alternate every 1 second  
    
    🔹 5. Keypad Time Setting
    
    Format:
    
        HH * MM * SS #
    
    
    Automatically limits incorrect values:
    
    Hours > 23 → set to 23
    
    Minutes/Sec > 59 → set to 59

🧠 Software Workflow

        Initialize LCD, I²C, Timer1, interrupts
        
        Read saved time from EEPROM
        
        Check push button for Fan ON/OFF
        
        Update LCD with RTC time
        
        Update runtime every 1 second
        
        Save to EEPROM when Fan OFF
        
        Alternate LEDs after 4 sec
        
        Keypad allows manual RTC set

🖥️ Technologies Used

        C programming
        
        XC8 Compiler
        
        MPLAB X IDE
        
        Proteus Simulation
        
        I²C protocol
        
        Timer Interrupts
        
        EEPROM read/write



🛠️ How to Use

        Power ON the system
        
        Fan OFF → Runtime is saved
        
        Fan ON → Time continues from saved EEPROM value
        
        Press RA1 to set RTC time
        
        Press RA2 to reset EEPROM values to 00:00:00

🚀 Applications

        Automotive ECU runtime logging
        
        Engine ON-time tracking
        
        Usage monitoring in machines
        
        Industrial shift tracking
        
        Runtime counters for appliances

📜 License

This project is open-source. Free to modify, improve and distribute.

👤 Author

Micheal Beavin J R

