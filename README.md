# ECEN 2440: Applications of Embedded Systems

Final Project: Buff Bikes
    
    -A bike share system that can be integrated for a university or business campus
    
    -Many universities and businesses use student/employee ID numbers and cards with student/employee specific data
    
    -After a user registers properly on the station, they can set a time for how long they want to use the bike
    
    -When the user has checked out a bike, the lock on the bike they have been given will only work for their RFID card


Lab 1: Embedded Systems Tools Introduction 

    -Declare pointers to registers
  
    -Use bitwise operators to turn on and off LED on MSP432
  
    -Generate square wave output on a pin
  
  
Lab 2: Interrupts and Timing Systems

    -Enable button interrupts to toggle an LED
    
    -Have a button interrupt run through 8 colors on an RGB LED at 500ms per color
    
    -Calculate latency when calling a button interrupt
    
    -Use a timer to create a square wave on a pin
    
Lab 3: UART and Circular Buffers

    -Use UART to send data to a serial terminal and receive data back
    
    -Use UART to pring loggin information to a sermial terminal
    
    -Implement circular buffers to store what is going to be received and trasmitted
    
    -Count how many times different types of characters are in a sent message and show that data on a serial terminal
    
    -Print what the RX buffer has received to the serial terminal with a right button press
    
    -Clear the RX buffer with a left button press

Lab 4: Analog to Digital Conversion

    -Use the ADC read value to approximate temperature data from the onboard temperature sensor
    
    -Use the joystick on the MKII BoosterPack and collect data about where it is being pushed using ADC
    
    -Use the accelerometer on the MKII BoosterPack to indicate if the device is being moved forwards or backwards
