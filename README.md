# FreeRTOS-Cortex-M0
A [FreeRTOS](http://www.freertos.org/) port for the [Arduino Zero](https://www.arduino.cc/en/Main/ArduinoBoardZero).

## USAGE ##
Place the FreeRtos folder to your arduino libraries directory. Don't use delay nor delayMicroseconds within your projects that use the scheduler. Only use vTaskDelay() in thread functions. 
