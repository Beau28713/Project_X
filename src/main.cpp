#include <Arduino.h>

const int motor_speed_pin = 3;
const int motor_enable_pin = 2;
const int man_freq_input_pin = A0;

const int auto_run_button = 5;
const int motor_jog_button = 4;
const int motor_in_hand_button = 6;

unsigned long interval = 1000;
unsigned long previous_time = 0;
unsigned long current_time;
int freq_value = 0;

const int starting_speed = 80;
const int max_speed = 255;

void jog_run()
{
  digitalWrite(motor_enable_pin, HIGH);
  while (true)
  {
    if (digitalRead(motor_jog_button) == HIGH)
    {
      analogWrite(motor_speed_pin, 0);
      digitalWrite(motor_enable_pin, LOW);
      return;
    }
    analogWrite(motor_speed_pin, starting_speed);
  }
}
void auto_run()
{
  int ramp_speed = starting_speed;
  previous_time = millis();
  digitalWrite(motor_enable_pin, HIGH);

  while (true)
  {
    current_time = millis();

    if (digitalRead(auto_run_button) == HIGH)
    {
      analogWrite(motor_speed_pin, 0);
      digitalWrite(motor_enable_pin, LOW);
      return;
    }

    if (ramp_speed <= max_speed && current_time - previous_time >= interval)
    {
      analogWrite(motor_speed_pin, ramp_speed);
      ramp_speed += 8;
      previous_time = current_time;
    }
    else if (ramp_speed > max_speed)
    {
      analogWrite(motor_speed_pin, max_speed);
    }
  }
}
void hand_run()
{
  digitalWrite(motor_enable_pin, HIGH);
  while (true)
  {
    if (digitalRead(motor_in_hand_button) == HIGH)
    {
      analogWrite(motor_speed_pin, 0);
      digitalWrite(motor_enable_pin, LOW);
      return;
    }
    freq_value = analogRead(man_freq_input_pin);
    analogWrite(motor_speed_pin, map(freq_value, 0, 1023, 0, 255));
  }
}

void setup()
{
  digitalWrite(motor_enable_pin, LOW);
  pinMode(motor_speed_pin, OUTPUT);
  pinMode(motor_enable_pin, OUTPUT);
  pinMode(motor_jog_button, INPUT_PULLUP);
  pinMode(auto_run_button, INPUT_PULLUP);
  pinMode(motor_in_hand_button, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(auto_run_button) == LOW && digitalRead(motor_in_hand_button) == HIGH)
  {
    auto_run();
  }
  else if (digitalRead(motor_in_hand_button) == LOW && digitalRead(auto_run_button) == HIGH)
  {
    hand_run();
  }
  else if (digitalRead(motor_jog_button) == LOW && digitalRead(motor_in_hand_button) == HIGH && digitalRead(auto_run_button) == HIGH)
  {
    jog_run();
  }
}
