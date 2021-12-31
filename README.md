# Multi Function Board
IoT project that is based on TM4C123 MCU and TI-RTOS and uses code composer studio IDE with the aim of:
1) Getting acceleration, gyroscope, temperature readings from MPU6050 through I2C serial communication.
2) Displaying digital sensor measurements on serial monitor after conversion to be in metric units.
3) Posting data to Thingspeak IoT Cloud via ESP8266 module using AT Commands through TCP connection.
4) Reading data from Thingspeak cloud using JSON parser library to extract data from HTTP GET response.

Prerequisites 
=============
You must install TIRTOS version 2.16.01.14 to compile code successfully
