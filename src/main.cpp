#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiClientSecureBearSSL.h>

#define SINGLE_LED_CHARACTERS 64
#define FLYER 8
#define METRO 6
#define TRAM 10
#define VILLAGE 4
#define TAXI 4
#define BONNIE 8
#define JET 6

const char *ssid = "SSID";
const char *password = "Wireless-Password";

const int LED_PIN = D7;
const int LED_COUNT = SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE +
                      TAXI + BONNIE + JET;

uint8_t led_value[LED_COUNT]; // An array to store individual LED values

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousMillisGet = 0;
unsigned long previousMillisLED = 0;

// unsigned long *previousMillisBlink = nullptr;
// unsigned long *startMillisDelay = nullptr;

// bool alternateColorFlags[LED_COUNT][2] = {
//     false};               // An array to store individual alternateColorFlag
//     for each LED
String mostRecentPayload; // Store the most recent payload globally
String staticPayload =
    "00000000000000000000000000000000000000000000000000000000000000004444444";

// New function to set system time from server
void setSystemTime()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(
      new BearSSL::WiFiClientSecure);
  client->setInsecure();

  HTTPClient https;

  Serial.print("[HTTPS] Getting timestamp...\n");
  if (https.begin(*client,
                  "https://jaypeaktrailmapled.firebaseapp.com/jay-date/"))
  {
    Serial.print("[HTTPS] GET...\n");
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String timestamp = https.getString();
        Serial.println("Timestamp received: " + timestamp);

        // Convert the timestamp to time_t
        time_t receivedTime = timestamp.toInt();
        setTime(receivedTime); // Set the system time
        Serial.println("System time set!");

        // You can print the current time to verify if it's set correctly
        Serial.print("Current Time after setting: ");
        Serial.println(now());
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET... failed, error: %s\n",
                    https.errorToString(httpCode).c_str());
    }

    https.end();
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}

void performHTTPGET()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(
      new BearSSL::WiFiClientSecure);
  client->setInsecure();

  HTTPClient https;

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client,
                  "https://jaypeaktrailmapled.firebaseapp.com/jay-ino/"))
  {
    Serial.print("[HTTPS] GET...\n");
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        mostRecentPayload = https.getString();
        Serial.println(mostRecentPayload);
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET... failed, error: %s\n",
                    https.errorToString(httpCode).c_str());
    }

    https.end();
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  pixels.clear();
  pixels.show();
}

void setPixelColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
  pixels.setPixelColor(index, pixels.Color(r, g, b));
}


void set_led_values()
{

  int i = 0;

  for (i = 0; i < SINGLE_LED_CHARACTERS; i++)
  {
    led_value[i] = mostRecentPayload.charAt(i) - '0';
  }
  for (i = SINGLE_LED_CHARACTERS; i < (SINGLE_LED_CHARACTERS + FLYER); i++)
  {
    led_value[i] = mostRecentPayload.charAt(SINGLE_LED_CHARACTERS) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER);
       i < (SINGLE_LED_CHARACTERS + FLYER + METRO); i++)
  {
    led_value[i] = mostRecentPayload.charAt(65) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER + METRO);
       i < (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM); i++)
  {
    led_value[i] = mostRecentPayload.charAt(66) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM);
       i < (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE); i++)
  {
    led_value[i] = mostRecentPayload.charAt(67) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE);
       i < (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE + TAXI);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(68) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE + TAXI);
       i <
       (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE + TAXI + BONNIE);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(69) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE + TAXI +
            BONNIE);
       i < (SINGLE_LED_CHARACTERS + FLYER + METRO + TRAM + VILLAGE + TAXI +
            BONNIE + JET);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(70) - '0';
  }
}

void set_solid_leds()
{
  int i = 0;
  for (i = 0; i < LED_COUNT; i++)
  {
    switch (led_value[i])
    {
    case 0:
      if (i < SINGLE_LED_CHARACTERS)
      {
        setPixelColor(i, 255, 0, 0); // Red
      }
      break;
    case 1:
      if (i < SINGLE_LED_CHARACTERS)
      {
        setPixelColor(i, 0, 255, 0); // Green
      }
      break;
    case 3:
      if (i >= SINGLE_LED_CHARACTERS)
      {
        setPixelColor(i, 255, 255, 0); // Yellow
      }
      break;
    case 4:
      if (i >= SINGLE_LED_CHARACTERS)
      {
        setPixelColor(i, 255, 0, 0); // Red
      }
      break;
    default:
      break;
    }
  }
}

void update_blinking_leds()
{
  static bool led_is_on = false;

  for (int i = 0; i < SINGLE_LED_CHARACTERS; i++)
  {
    switch (led_value[i])
    {
    case 2:
      if (led_is_on)
      {
        setPixelColor(i, 0, 0, 0); // Off
      }
      else
      {
        setPixelColor(i, 0, 255, 0); // Green
      }
      break;
    case 6:
      if (led_is_on)
      {
        setPixelColor(i, 0, 0, 0); // Off
      }
      else
      {
        setPixelColor(i, 255, 255, 0); // Yellow
      }
      break;
    default:
      break;
    }
  }
  led_is_on = !led_is_on;
}

int look_for_value_5()
{
  int i = 0;
  for (i = SINGLE_LED_CHARACTERS; i < LED_COUNT; i++)
  {
    if (led_value[i] == 5)
    {
      return i;
    }
  }
  return 0;
}

void update_special_leds()
{
  static int led_to_check = 0;
  static int times_called = 0;
  times_called++;
  if (times_called == 1)
  {
    return;
  }
  if (times_called == 2)
  {
    times_called = 0;
  }

  int first_special_led = look_for_value_5();
  if (first_special_led == 0) // not special led found
  {
    return;
  }

  if (led_to_check == 0)
  {
    setPixelColor(first_special_led, 0, 0, 255); // Blue
    led_to_check = first_special_led + 1;
  }
  else if (led_to_check == LED_COUNT)
  {
    for (int i = first_special_led; i < led_to_check; i++)
    {
      setPixelColor(i, 0, 0, 0); // Off
    }
    led_to_check = 0;
  }
  else if (led_value[led_to_check] == 5)
  {
    setPixelColor(led_to_check, 0, 0, 255); // Blue
    led_to_check++;
    if (led_to_check == LED_COUNT)
    {
      for (int i = first_special_led; i < led_to_check; i++)
      {
        setPixelColor(i, 0, 0, 0); // Off
      }
      led_to_check = 0;
    }
  }
  else
  {
    for (int i = first_special_led; i < led_to_check; i++)
    {
      setPixelColor(i, 0, 0, 0); // Off
    }
    led_to_check = 0;
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }

  setSystemTime();

  pixels.begin();
  pixels.show();            // Initialize all pixels to 'off'
  pixels.setBrightness(50); // Set brightness

  // Wait for 1 minute upon initial load
  delay(5000);

  // Perform the first GET request
  performHTTPGET();
  set_led_values();
  set_solid_leds();
}

void loop()
{
  // Update blinking LEDs every 500 milliseconds
  if (millis() - previousMillisLED >= 500)
  {
    update_blinking_leds();
    update_special_leds();
    pixels.show();
    previousMillisLED = millis();
  }

  // Perform GET request every 5 minutes after the initial GET
  if (WiFi.status() == WL_CONNECTED)
  {
    if (millis() - previousMillisGet >= 300000)
    { // 5 minutes
      previousMillisGet = millis();
      performHTTPGET();
      set_solid_leds();
    }
  }
}
