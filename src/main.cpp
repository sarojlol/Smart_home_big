#include <Arduino.h>
#include <pins_define.h>

#define BLYNK_TEMPLATE_ID "TMPL6b4ap6ORG"
#define BLYNK_TEMPLATE_NAME "Smart Home"
//#define BLYNK_AUTH_TOKEN             "2oZEri2JfLLGI_s5OhaAA4ph4nSZrS4P"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#include <BlynkEdgent.h>

#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#define APP_DEBUG

byte light_sw_pin[3] = {Lsw1, Lsw2, Lsw3};
bool light_sw_status[3];
bool light_sw_flag[3];
byte led_pins[3] = {LED1, LED2, LED3};
bool led_status[3];

byte fan_sw_pin[3] = {Fsw1, Fsw2, Fsw3};
bool fan_sw_status[3];
bool fan_sw_flag[3];
byte fan_pin[3] = {fan1, fan2, fan3};
bool fan_status[3];

int temperature;
int last_temp;

int gas_percent;
int last_gas_per;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  led_status[0] = param.asInt();
  digitalWrite(led_pins[0], !led_status[0]);
}
BLYNK_WRITE(V1)
{
  led_status[1] = param.asInt();
  digitalWrite(led_pins[1], !led_status[1]);
}
BLYNK_WRITE(V2)
{
  led_status[2] = param.asInt();
  digitalWrite(led_pins[2], !led_status[2]);
}
BLYNK_WRITE(V3)
{
  fan_status[0] = param.asInt();
  digitalWrite(fan_pin[0], fan_status[0]);
}
BLYNK_WRITE(V4)
{
  fan_status[1] = param.asInt();
  digitalWrite(fan_pin[1], fan_status[1]);
}
BLYNK_WRITE(V5)
{
  fan_status[2] = param.asInt();
  digitalWrite(fan_pin[2], fan_status[2]);
}


void setup()
{
  for (int i = 0; i < 3; i++)
  {
    pinMode(led_pins[i], OUTPUT);
    pinMode(light_sw_pin[i], INPUT_PULLUP);
    pinMode(fan_pin[i], OUTPUT);
    pinMode(fan_sw_pin[i], INPUT_PULLUP);
  }
  
  Serial.begin(115200);
  BlynkEdgent.begin();
}

void loop()
{
  BlynkEdgent.run();

  //light button hanndle
  static unsigned long light_sw_filter;
  if ((millis() - light_sw_filter) > 10)
  {
    for (int i = 0; i < 3; i++)
    {
      light_sw_status[i] = digitalRead(light_sw_pin[i]);
      if ((light_sw_status[i] == LOW) &! light_sw_flag[i])
      {
        led_status[i] =! led_status[i];
        digitalWrite(led_pins[i], !led_status[i]);
        switch (i)
        {
        case 0:
          Blynk.virtualWrite(V0, led_status[0]);
          break;
        case 1:
          Blynk.virtualWrite(V1, led_status[1]);
          break;
        case 2:
          Blynk.virtualWrite(V2, led_status[2]);
          break;
        }
        light_sw_flag[i] == true;
      }

      else if ((light_sw_status[i] == HIGH) && light_sw_flag[i])
      {
        light_sw_flag[i] = false;
      }
    }
    light_sw_filter = millis();
  }

  //fan button hanndle
  static unsigned long fan_sw_filter;
  if ((millis() - fan_sw_filter) > 10)
  {
    for (int i = 0; i < 3; i++)
    {
      fan_status[i] = digitalRead(fan_pin[i]);
      if ((fan_status[i] == LOW) &! fan_sw_flag[i])
      {
        fan_status[i] =! fan_status[i];
        digitalWrite(fan_pin[i], !fan_status[i]);
        switch (i)
        {
        case 0:
          Blynk.virtualWrite(V3, fan_status[0]);
          break;
        case 1:
          Blynk.virtualWrite(V4, fan_status[1]);
          break;
        case 2:
          Blynk.virtualWrite(V5, fan_status[2]);
          break;
        }
        fan_sw_flag[i] == true;
      }

      else if ((fan_status[i] == HIGH) && fan_sw_flag[i])
      {
        fan_sw_flag[i] = false;
      }
    }
    fan_sw_filter = millis();
  }

  //temperature sensor handel
  static unsigned long temp_fillter;
  if ((millis() - temp_fillter) > 1000)
  {
    if (temperature != last_temp)
    {
      Blynk.virtualWrite(V6, temperature);
      last_temp = temperature;
    }
    temp_fillter = millis();
  }

  //gas sensor handdle
  static unsigned long gas_fillter;
  if ((millis() - gas_fillter) > 500)
  {
    if (gas_percent != last_gas_per)
    {
      Blynk.virtualWrite(V6, gas_percent);
      last_gas_per = gas_percent;
    }
    gas_fillter = millis();
  }
}
