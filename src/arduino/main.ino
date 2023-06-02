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
#define REFRESH_DELAY      300
#define LCD_COLUMNS         16
#define LCD_LINES            2
#define LCD_ADDRESS       0x27

Servo servo;

DHT sensor(DHTPIN, DHTTYPE);

LiquidCrystal_I2C display(LCD_ADDRESS, LCD_COLUMNS, LCD_LINES);

void setup()
{
  pinMode(HEATERPIN, OUTPUT);

  digitalWrite(HEATERPIN, LOW);

  display.init();

  display.setBacklight(HIGH);

  sensor.begin();

  servo.attach(SERVOPIN);
}

void loop()
{
  int heaterMode, servoPosition = 0;

  double humidity, temperature;

  display.setCursor(0, 0);

  display.clear();

  humidity = sensor.readHumidity();

  temperature = sensor.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    display.print("Falha ao ler o sensor DHT");

    return;
  }

  if (humidity < MIN_HUMIDITY)
  {
    display.print("Baixa umidade");

    return;
  }

  display.print("Umidade: ");

  display.print(humidity);

  display.print("%");

  display.setCursor(0, 1);

  display.print("Temp: ");

  display.print(temperature);

  display.print(" C");

  // the heating modes are reversed, so low means high and high means low

  heaterMode = (temperature < MAX_TEMPERATURE) : LOW : HIGH;

  digitalWrite(HEATERPIN, heaterMode);

  while (servoPosition++ <= SERVO_MAX_DEGREE)
  {
    servo.write(servoPosition);

    delay(REFRESH_DELAY);
  }

  delay(REFRESH_DELAY);

  while (servoPosition-- != 0)
  {
    servo.write(servoPosition);

    delay(REFRESH_DELAY);
  }
}
