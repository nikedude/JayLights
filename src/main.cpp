#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiClientSecureBearSSL.h>

#define INITIAL_DELAY 5000
#define HTTP_GET_DELAY 300000
#define LED_BLINK_DELAY 500
#define LED_SPECIAL_DELAY 500

#define SINGLE_LED_CHARACTERS 64
#define FLYER_LEN 8
#define METRO_LEN 6
#define TRAM_LEN 10
#define VILLAGE_LEN 4
#define TAXI_LEN 4
#define BONNIE_LEN 8
#define JET_LEN 6

#define FLYER_START 64
#define METRO_START 72
#define TRAM_START 78
#define VILLAGE_START 88
#define TAXI_START 92
#define BONNIE_START 96
#define JET_START 104


const char *ssid = "Cega-2GHz";
const char *password = "emulador4";

const int LED_PIN = D7;
const int LED_COUNT = SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN +
                      TAXI_LEN + BONNIE_LEN + JET_LEN;

uint8_t led_value[LED_COUNT]; // An array to store individual LED values

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousMillisGet = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisSpecial = 0;

int led_to_check_flyer = 0;
int led_to_check_metro = 0;
int led_to_check_tram = 0;
int led_to_check_village = 0;
int led_to_check_taxi = 0;
int led_to_check_bonnie = 0;
int led_to_check_jet = 0;

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
  for (i = SINGLE_LED_CHARACTERS; i < (SINGLE_LED_CHARACTERS + FLYER_LEN); i++)
  {
    led_value[i] = mostRecentPayload.charAt(SINGLE_LED_CHARACTERS) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN);
       i < (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN); i++)
  {
    led_value[i] = mostRecentPayload.charAt(65) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN);
       i < (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN); i++)
  {
    led_value[i] = mostRecentPayload.charAt(66) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN);
       i < (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN); i++)
  {
    led_value[i] = mostRecentPayload.charAt(67) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN);
       i < (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN + TAXI_LEN);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(68) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN + TAXI_LEN);
       i <
       (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN + TAXI_LEN + BONNIE_LEN);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(69) - '0';
  }
  for (i = (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN + TAXI_LEN +
            BONNIE_LEN);
       i < (SINGLE_LED_CHARACTERS + FLYER_LEN + METRO_LEN + TRAM_LEN + VILLAGE_LEN + TAXI_LEN +
            BONNIE_LEN + JET_LEN);
       i++)
  {
    led_value[i] = mostRecentPayload.charAt(70) - '0';
  }

  for(i = 0; i < LED_COUNT; i++)
  {
    Serial.print(led_value[i]);
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

void check_value_5(int start, int end, int &led_to_check)
{  
  if (led_value[start] != 5) // not special led found
  {
    return;
  }

  if (led_to_check == 0)
  {
    setPixelColor(start, 0, 0, 255); // Blue
    led_to_check = start + 1;
  }
  else if (led_to_check == end)
  {
    for (int i = start; i < led_to_check; i++)
    {
      setPixelColor(i, 0, 0, 0); // Off
    }
    led_to_check = 0;
  }
  else
  {
    setPixelColor(led_to_check, 0, 0, 255); // Blue
    led_to_check++;
  }
}

void update_special_leds()
{
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

  check_value_5(FLYER_START, METRO_START, led_to_check_jet);
  check_value_5(METRO_START, TRAM_START, led_to_check_flyer);
  check_value_5(TRAM_START, VILLAGE_START, led_to_check_metro);
  check_value_5(VILLAGE_START, TAXI_START, led_to_check_tram);
  check_value_5(TAXI_START, BONNIE_START, led_to_check_village);
  check_value_5(BONNIE_START, JET_START, led_to_check_taxi);
  check_value_5(JET_START, LED_COUNT, led_to_check_bonnie);
  
}

void led_to_check_reset()
{
  led_to_check_flyer = 0;
  led_to_check_metro = 0;
  led_to_check_tram = 0;
  led_to_check_village = 0;
  led_to_check_taxi = 0;
  led_to_check_bonnie = 0;
  led_to_check_jet = 0;
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
    delay(INITIAL_DELAY);
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
  if (millis() - previousMillisLED >= LED_BLINK_DELAY)
  {
    update_blinking_leds();
    pixels.show();
    previousMillisLED = millis();
  }

  if (millis() - previousMillisSpecial >= LED_SPECIAL_DELAY)
  {
    update_special_leds();
    pixels.show();
    previousMillisSpecial = millis();
  }

  // Perform GET request every 5 minutes after the initial GET
  if (WiFi.status() == WL_CONNECTED)
  {
    if (millis() - previousMillisGet >= HTTP_GET_DELAY)
    { // 5 minutes
      previousMillisGet = millis();
      performHTTPGET();
      set_led_values();
      set_solid_leds();
      led_to_check_reset();
    }
  }
}
