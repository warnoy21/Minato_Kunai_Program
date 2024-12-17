/* 

  Description : Arudino code that uses ESP32C3 super mini 
                that utilizes temperaturesensor and multi seven segment display for Minato Kunai Temp sensor
 
  Initial Creation : by Aaron Gumba               Date: December 10, 2024


*/


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <TM1637Display.h>

//for the multi seven segment display
#define CLK 4 // Define the clock pin
#define DIO 3 // Define the data pin

// I2C pins for ESP32-C3 for temp seensor
#define I2C_SDA 6  // GPIO6
#define I2C_SCL 7  // GPIO7

TM1637Display display(CLK, DIO); //multi seven segment display
Adafruit_BMP280 bmp; // I2C temp sensor

//seven segment display values
uint8_t zero = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F};
uint8_t one = {SEG_B|SEG_C};
uint8_t two = {SEG_A|SEG_B|SEG_D|SEG_E|SEG_G};
uint8_t three = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_G};
uint8_t four = {SEG_B|SEG_C|SEG_F|SEG_G};
uint8_t five = {SEG_A|SEG_C|SEG_D|SEG_F|SEG_G};
uint8_t six = {SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G};
uint8_t seven = {SEG_A|SEG_B|SEG_C};
uint8_t eight = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G};
uint8_t nine = {SEG_A|SEG_B|SEG_C|SEG_F|SEG_G};

void temperature (uint8_t tens, uint8_t ones){
      uint8_t displaySegments[] = {
            tens, 
            ones,
            SEG_B | SEG_G|SEG_F|SEG_A,
            SEG_A |SEG_F |SEG_E|SEG_D   //  display the Celcisus sign
        };
      display.setSegments(displaySegments);
      delay(2000);
  
  }
void setup() {
  // Set up I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

  
 // Serial.begin(9600);
  //while (!Serial) delay(100);
  //Serial.println(F("BMP280 test"));

  unsigned status;
  status = bmp.begin(0x76); // Change to 0x77 if needed

  display.setBrightness(5); // Set brightness (0 to 7)
  display.clear();          // Clear the display
                       

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  void calculation(){

    int values [10] =[];
    for (int i =0 ; i < values(10);i++){
      float temp = bmp.readTemperature(); // Get temperature in Celsius
      int rounded_temp = round(temp);  // Round and store it as an integer
      values.append(rounded_temp)

    }
    
  int intemp =  int(temp);
  }
  
 
  temperature(zero,one);
  temperature(two,three);
  temperature(four,five);
  temperature(six,seven);
  temperature(eight,nine);
  
  
  
  
  

//display.clear();
}
