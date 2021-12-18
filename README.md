# MPU6050ThingSpeak
This is IoT project which is based on TM4C123 MCU and TI-RTOS with the aim of:
Read acceleration, gyroscope, temperature readings from MPU6050 through I2C serial communication.
Post data to Thingspeak IoT Cloud via ESP8266 module using AT Commands through TCP connection.
Read data from Thingspeak cloud using JSON parser library to extract data from HTTP GET response.
