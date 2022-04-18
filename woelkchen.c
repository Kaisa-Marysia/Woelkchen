// Include required libraries like wifi support and led controll
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// Set GPIO pin to send data to the LEDs
int neoPixelPin = 5;

// Set the amout of LEDs
int numPixels = 15;

long firstPixelHue = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

// Wifi Credentials
const char* ssidTab = {
  "<ssid>"
};
const char* passwordTab = {
  "<Password>"
};

// This is the hostname of the ESP32. You will see this name in your network device list.
String hostname = "Woelkchen";

// Network port on which the webserver run
WiFiServer server(80);

String header;

// In this part, the ESP32 connect to the Wifi and use one core for the Webserver and the other one for the LEDs controll.
void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show();


  bool connectedSuccess = 0;
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str()); //define hostname
   WiFi.begin(ssidTab, passwordTab);
      if (WiFi.status() != WL_CONNECTED) {
        delay(500);
  }

  server.begin();

  xTaskCreate(
    taskLED,          
    "taskLED",        
    10000,            
    NULL,             
    2,                
    NULL);            

  xTaskCreate(
    taskWebserver,          
    "taskWebserver",        
    10000,           
    NULL,            
    1,              
    NULL);           
}

// In this part are the profiles, which set the colors and effects for the LEDs.
// All profiles are running in a loop.
// Add a new void for additional profiles. You must also add this to the cases and the webserver header configuration, further down.
// LightningFlash1 turns slow and random cold/white lights on.
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

// LightningFlash2 are turns very fast random colors and Leds on/off.
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

// LightningFlash3 is the same as LightningFlash1 but with colored LEDs instead of cold/white ones.
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

// This is a default Rainbow effect
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

// Turn the lights off
void allOff() 
    {
        for(uint16_t i=0; i<strip.numPixels(); i++) 
        {
            strip.setPixelColor(i, 0, 0, 0);
            strip.show();
        }
    }


// Set the which profile to use on a case, when the webserver get a GET request on host/0 to host/4.
// The last case is the default start case, when the ESP32 turns on. 
// Set the refresh speed of the loop for each LightningFlash profile, by setting the first value (wait) in the brackets.
uint8_t modeLED = 4;
void taskLED( void * parameter )
{
  while (1) {
    switch (modeLED) {
      case 0:
        allOff();
        break;
      case 1:
        LightningFlash1(5, 2);
        break;
      case 2:
        allOff();
        LightningFlash2(1, 2); 
        break;
      case 3:
        LightningFlash3(5, 2);
        break;
      case 4:
        rainbow(200);
        firstPixelHue = 0;
        break;
//      default:
//        allOff();
//        break;
    }
  }
  vTaskDelete( NULL );
}

// start
void loop() {
  while (1) {
    delay(1000);
  }
}

// This is the code for the webserver.
// in the if loop for the header.indexOf, you set on which GET Request (/0) the webserver responds with a 200 OK and use the modeLED,
// which select the case and run the code one of the profiles.
void taskWebserver( void * parameter ) {

  while (1) {
  WiFiClient client = server.available();
    if (client) {
      String currentLine = "";
      while (client.connected()) {

        if (client.available()) {
          char c = client.read();
          //          Serial.write(c);
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

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
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      header = "";
      client.stop();
    }
  }
}

