# MPU6050 ThingSpeak
IoT project that is based on TM4C123 MCU and TI-RTOS with the aim of:
1) Getting acceleration, gyroscope, temperature readings from MPU6050 through I2C serial communication.
2) Posting data to Thingspeak IoT Cloud via ESP8266 module using AT Commands through TCP connection.
3) Reading data from Thingspeak cloud using JSON parser library to extract data from HTTP GET response.

