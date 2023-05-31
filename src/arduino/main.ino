#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

#define HEATERPIN            8
#define SERVOPIN             7
#define SERVO_MAX_DEGREE   180
#define DHTPIN               6
#define DHTTYPE          DHT22
#define MAX_TEMPERATURE   38.5
#define MIN_HUMIDITY        60
#define BAUD_RATE         9600
#define REFRESH_DELAY     2000
#define LCD_COLUMNS         16
#define LCD_LINES            2
#define LCD_ADDRESS       0x27

Servo servo;

DHT sensor(DHTPIN, DHTTYPE);

int servo_position = 0;

double humidity, temperature;

LiquidCrystal_I2C display(LCD_ADDRESS, LCD_COLUMNS, LCD_LINES);

void setup()
{
  pinMode(SWITCHPIN, INPUT);

  pinMode(HEATERPIN, OUTPUT);

  display.init();

  display.setBacklight(HIGH);

  Serial.begin(BAUD_RATE);

  sensor.begin();

  servo.attach(SERVOPIN);
}

void loop()
{
  humidity = sensor.readHumidity();

  temperature = sensor.readTemperature();

  if (!is_the_sensor_working())
  {
    Serial.println("Falha ao ler o sensor DHT");

    display.print("Falha ao ler o sensor DHT");

    return;
  }

  if (humidity < MIN_HUMIDITY)
  {
    Serial.println("Baixa umidade");

    display.print("Baixa umidade");
  }

  display_data();

  update_heater();

  update_servo();
}

void display_data()
{
  display.setCursor(0, 0);

  display.print("Umidade: ");

  display.print(humidity);

  display.print("%");

  display.setCursor(0, 1);

  display.print("Temperatura: ");

  display.print(temperature);

  display.print(" °C");
}

void update_heater()
{
  int mode = (temperature < MAX_TEMPERATURE) ? HIGH : LOW;

  digitalWrite(HEATERPIN, mode);
}

void update_servo()
{
  while (servo_position++ <= SERVO_MAX_DEGREE)
  {
    servo.write(servo_position);
  }

  while (servo_position-- != 0)
  {
    servo.write(servo_position);
  }
}

bool is_the_sensor_working()
{
  return !isnan(humidity) && !isnan(temperature);
}
