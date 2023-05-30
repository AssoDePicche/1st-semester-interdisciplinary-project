#include <DHT.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define SWITCHPIN 7

#define HEATERPIN 8

#define SERVOPIN 9
#define SERVO_MAX_DEGREE 60

#define DHTPIN 5
#define DHTTYPE DHT22

#define MAX_TEMPERATURE 38.5
#define MIN_HUMIDITY 180

#define BAUD_RATE 9600

#define REFRESH_DELAY 2000

#define LCD_COLUMNS 16
#define LCD_LINES 2
#define REGISTER_SELECT 12
#define ENABLE 11
#define DB4 5
#define DB5 4
#define DB6 3
#define DB7 2

Servo servo;

DHT sensor(DHTPIN, DHTTYPE);

int servo_position = 0;

double humidity, temperature;

LiquidCrystal display(REGISTER_SELECT, ENABLE, DB4, DB5, DB6, DB7);

void setup()
{
  pinMode(SWITCHPIN, INPUT);

  pinMode(HEATERPIN, OUTPUT);

  Serial.begin(BAUD_RATE);

  sensor.begin();

  display.begin(LCD_COLUMNS, LCD_LINES);

  display.clear();

  servo.attach(SERVOPIN);
}

void loop()
{
  humidity = sensor.readHumidity();

  temperature = sensor.readTemperature();

  debug();

  display_data();

  update_heater();

  update_servo();

  delay(REFRESH_DELAY);
}

void debug()
{
  if (!is_the_sensor_working())
  {
    Serial.println("Falha ao ler o sensor DHT");

    return;
  }

  Serial.print("Umidade: ");

  Serial.print(humidity);

  Serial.println("%");

  Serial.print("Temperatura: ");

  Serial.print(temperature);

  Serial.println(" °C");
}

void display_data()
{
  display.setCursor(0, 0);

  if (!is_the_sensor_working())
  {
    display.print("Falha ao ler o sensor DHT");

    return;
  }

  if (humidity < MIN_HUMIDITY)
  {
    display.println("Baixa umidade");

    return;
  }

  display.print("Umidade: ");

  display.print(humidity);

  display.print("%");

  display.setCursor(0, 1);

  display.print("Temperatura: ");

  display.print(temperature);

  Serial.println(" °C");
}

void update_heater()
{
  int mode = is_switch_on() && (temperature < MAX_TEMPERATURE) ? HIGH : LOW;

  digitalWrite(HEATERPIN, mode);
}

void update_servo()
{
  while (servo_position++ <= SERVO_MAX_DEGREE)
  {
    servo.write(servo_position);
  }

  delay(REFRESH_DELAY);

  while (servo_position-- != 0)
  {
    servo.write(servo_position);
  }
}

bool is_the_sensor_working()
{
  return !isnan(humidity) && !isnan(temperature);
}

bool is_switch_on()
{
  return digitalRead(SWITCHPIN) == HIGH;
}
