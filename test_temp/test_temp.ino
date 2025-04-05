/*
  Description: Arduino code for ESP32C3 Super Mini
               Uses a BMP280 temperature sensor to measure room temperature (1-40°C)
  
  Initial Creation: Aaron Gumba - December 10, 2024
*/

#include <Wire.h>
#include <Adafruit_BMP280.h>

// I2C pins for ESP32-C3
#define I2C_SDA 6  // GPIO6
#define I2C_SCL 7  // GPIO7

Adafruit_BMP280 bmp; // I2C temp sensor

void setup() {
  // Set up I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.begin(9600);
  while (!Serial) delay(100);
  Serial.println(F("BMP280 test"));

  // Initialize BMP280 sensor
  if (!bmp.begin(0x76)) { // Change to 0x77 if needed
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from the datasheet */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time */
}

void loop() {
  float temperature = bmp.readTemperature(); // Read temperature

  // Ensure temperature is within 1-40°C range
  if (!isnan(temperature) && temperature >= 1 && temperature <= 40) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Invalid temperature reading");
  }

  delay(2000); // Wait 2 seconds before next reading
}
