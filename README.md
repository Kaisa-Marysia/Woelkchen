* [Woelkchen](#woelkchen)
  * [What You need](#what-you-need)
  * [What to do](#what-to-do)
  * [Functions](#functions)
  * [How to modify the code?](#how-to-modify-the-code)
    * [Set the GPIO Data output](#set-the-gpio-data-output)
    * [Set the numbers of LEDs](#set-the-numbers-of-leds)
    * [Set the Wifi Credentials](#set-the-wifi-credentials)
    * [modify the blinking effects](#modify-the-blinking-effects)
      * [Lightning 1](#lightning-1)
      * [Lightning 2](#lightning-2)
      * [Lightning 3](#lightning-3)
      * [Rainbow effect](#rainbow-effect)
      * [turn off](#turn-off)
      * [URLs for the GET requests](#urls-for-the-get-requests)


# Woelkchen
Woelkchen is a small handicraft project with a **ESP32** and **WS2812** LEDs in a case that looks like a cloud.
The **ESP32** also run a simple web server to control the light effects of Woelkchen, by sending GET Requests. 

## What you need
For these projects you need:
  * A ESP32
  * A ws2812 strip with 15 LEDs
  * *(optional)* A piece of wood to glue the LEDs on. (Should have a smaller size the bottleneck) 
  * A PET bottle where a ESP32 should pass trough
  * filler cotton
  * Glue
  * Jumper Cable
  
Also, you need a computer, USB cable and the Arduino IDE software. 
Setup the Arduino IDE, so you can work with the ESP32. Also, you must have the libraries installed.
  * ESPAsync_WiFiManager
  * AdaFruit NeoPixel

## What to do
If you want to use a staff for the LEDs, you must glue the LEDs on it and solder the connections between the single LEDs. You can also put the **WS2812** strip into the bottle and hope it looks good. 

The data connection from the **ESP32** to the **WS28212** is on `GPIO Pin #5`. You can change it in the code (Line 6: `int neoPixelPin = 5;`).
For the 5V Output from the **ESP32** connect the `GPIO PIN 5V` with the `5V Connection` on the **WS2812** and the `GPIO GND Pin` with the `WS2812 GND Connection`. 

Cut a hole into the bottle, so you can connect the **ESP32** with a USB cable. I did it in the center of the bottle, so it would be in balance if I hang the cloud on the cable to the roof. 

Glue the stick on the lid of the bottle, so you can use it for maintenance and pull it out without problems.

Start the **Arduino IDE** Software and open the code from this repository. Set your Wifi Credentials on Line 15-21, verify/compile the code and if all is okay, upload the code to the **ESP32**. Press the Reset button on the ESP. Check the code by sending a `GET` request to the **ESP32** by browsing to `http://<cloud IPv4>/1` or send it via cURL. If the LEDs starts to light, all is working fine.

Now you just need to glue the filler cotton in the bottle and lid (separate).

## Functions
You can change the blinking mode by sending `GET` Requests.

  * `http://<cloud-IPv4>/0` to turn it off.
  * `http://<cloud-IPv4>/1` cold lightning flash
  * `http://<cloud-IPv4>/2` fast colored lightning flash
  * `http://<cloud-IPv4>/3` colored lightning flash
  * `http://<cloud-IPv4>/4` rainbow lights

## How to modify the code?
The code is commented so you can see what code part is doing. But in case..

### Set the GPIO Data output
Line 6: `int neoPixelPin = 5;`

### Set the numbers of LEDs
Line 9: `int numPixels = 15;`

### Set the Wifi Credentials
Line 15-21: 
```c
// Wifi Credentials
const char* ssidTab = {
  "<ssid>"
};
const char* passwordTab = {
  "<Password>"
};
```

### Set the web server port listing on
Line 27:  `WiFiServer server(80);`

### modify the blinking effects
#### Lightning 1
Line 70: 
```c
void LightningFlash1(uint8_t wait, uint8_t times)
    {
        if (wait == 0)
            wait = 0;
        if (times == 0)
            times = 0;
        for (uint8_t j=0; j < times; j++)
        {
            long randnr = random(0, 255);
            for(uint16_t i=0; i<strip.numPixels(); i++) 
            {
                strip.setPixelColor(random(0, numPixels), randnr, randnr, randnr);
                strip.show();
                delay(wait);
            }
        }
    }
```
#### Lightning 2
Line 89:
```c
void LightningFlash2(uint8_t wait, uint8_t times)
    {
      allOff();
        if (wait == 1)
            wait = 0;
        if (times == 1)
            times = 0;
        for (uint8_t j=0; j < times; j++)
        {
            long randnrR = random(0, 255);
            long randnrG = random(0, 255);
            long randnrB = random(0, 255);

            for(uint16_t i=0; i<strip.numPixels(); i++) 
            {
                strip.setPixelColor(random(0, numPixels), randnrR, randnrG, randnrB);
                delay(wait);
            }
        }
        allOff();
    }
```
    
#### Lightning 3
Line 112
```c
void LightningFlash3(uint8_t wait, uint8_t times)
    {
      allOff();
        if (wait == 0)
            wait = 0;
        if (times == 0)
            times = 0;
        for (uint8_t j=0; j < times; j++)
        {
            long randnrR = random(0, 255);
            long randnrG = random(0, 255);
            long randnrB = random(0, 255);

            for(uint16_t i=0; i<strip.numPixels(); i++) 
            {
                strip.setPixelColor(random(0, numPixels), randnrR, randnrG, randnrB);
                strip.show();
                delay(wait);
            }
        }
        allOff();
    }
```

#### Rainbow effect
Line: 136
```
void rainbow(int wait) {
    for(firstPixelHue; firstPixelHue < 1*65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
    strip.show(); 
    delay(wait);  
  }
}
```

#### turn off
Line: 148
```c
void allOff() 
    {
        for(uint16_t i=0; i<strip.numPixels(); i++) 
        {
            strip.setPixelColor(i, 0, 0, 0);
            strip.show();
        }
    }
```

### URLs for the GET requests
If you want to rename on which `GET` requests the web server should response, you find it on Line 220 - 239.
Just change `"GET /X"` to the value you want to use.

```c
              if (header.indexOf("GET /0") >= 0) {
                firstPixelHue = 65536;
                modeLED = 0;
              }
              if (header.indexOf("GET /1") >= 0) {
                firstPixelHue = 65536;
                modeLED = 1;
              }
              if (header.indexOf("GET /2") >= 0) {
                firstPixelHue = 65536;
                modeLED = 2;
              }
              if (header.indexOf("GET /3") >= 0) {
                firstPixelHue = 0 ;
                modeLED = 3;
              }
              if (header.indexOf("GET /4") >= 0) {
                firstPixelHue = 65536;
                modeLED = 4;
              }
              
```


    
﻿
