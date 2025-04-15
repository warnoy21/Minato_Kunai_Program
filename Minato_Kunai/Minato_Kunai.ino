/* 

  Description : Arudino code that uses ESP32C3 super mini 
                that utilizes temperature sensor and multi seven segment display for Minato Kunai Temp sensor
  WARNING : This sensor is only configured for room temperature ranging from 1 to 40 degree ceslsius.
 
  Initial Creation : by Aaron Gumba               Date: December 10, 2024

1.0.0     Initial Creation
1.0.1     Flipped the seven segment display for the 3D frame
1.0.2     Changed to using delay instead of millis


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

unsigned long lastRunTime = 0; // Tracks the last time calculation() was called
const unsigned long interval = 900000; // 15 minutes = 900,000 milliseconds

//flipped seven segment display values
uint8_t zero = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F};
uint8_t one = {SEG_B|SEG_C};
uint8_t two = {SEG_A|SEG_B|SEG_D|SEG_E|SEG_G};
uint8_t three = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_G};
uint8_t four = {SEG_C|SEG_F|SEG_B|SEG_G};
uint8_t five = {SEG_A|SEG_C|SEG_D|SEG_F|SEG_G};
uint8_t six = {SEG_A|SEG_E|SEG_F|SEG_G|SEG_C|SEG_D};
uint8_t seven = {SEG_A|SEG_B|SEG_C};
uint8_t eight = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G};
uint8_t nine = {SEG_A|SEG_B|SEG_G|SEG_F|SEG_C};

//seven segment display values
// uint8_t zero = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F};
// uint8_t one = {SEG_F|SEG_E};
// uint8_t two = {SEG_D|SEG_E|SEG_G|SEG_B|SEG_A};
// uint8_t three = {SEG_D|SEG_E|SEG_F|SEG_A|SEG_G};
// uint8_t four = {SEG_C|SEG_G|SEG_E|SEG_F};
// uint8_t five = {SEG_D|SEG_C|SEG_G|SEG_F|SEG_A};
// uint8_t six = {SEG_D|SEG_C|SEG_B|SEG_A|SEG_F|SEG_G};
// uint8_t seven = {SEG_D|SEG_E|SEG_F};
// uint8_t eight = {SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G};
// uint8_t nine = {SEG_D|SEG_C|SEG_G|SEG_E|SEG_F};


void reset (void){
  uint8_t displaySegments[] = {

    //Flipped
    // SEG_C|SEG_B|SEG_D|SEG_A|SEG_G,          //E
    // SEG_C|SEG_B|SEG_A|SEG_F|SEG_E,          //V
    // SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,    //O
    // SEG_A|SEG_B|SEG_C                       //L

    
    SEG_F|SEG_E|SEG_D,                         //L
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,       //O
    SEG_F|SEG_E|SEG_D|SEG_C|SEG_B,             //V
    SEG_A|SEG_F|SEG_G|SEG_E| SEG_D             //E
             
  }; 
      display.setSegments(displaySegments);
     

}
void temperature (uint8_t tens, uint8_t ones){
      uint8_t displaySegments[] = {
        
        //Flipped
        // SEG_A|SEG_B|SEG_C|SEG_D,    //  display the Celcisus sign
        // SEG_D | SEG_G|SEG_E|SEG_C,
        // ones,
        // tens 

        tens, 
        ones,
        SEG_B | SEG_G|SEG_F|SEG_A,
        SEG_A |SEG_F |SEG_E|SEG_D   //  display the Celcisus sign
        };
      display.setSegments(displaySegments);
      
  
  }

void convert_for_display (uint8_t tens, uint8_t ones){

    uint8_t val1;
    uint8_t val2;
    if (tens == 9 ){
        val1 = nine;
    } 
    else  if (tens ==8){
        val1 = eight;
    }
    else  if (tens ==7){
        val1 = seven;
    }
    else  if (tens ==6){
        val1 = six;
    }
    else  if (tens ==5 ){
        val1 = five;
    }
    else  if (tens ==4 ){
        val1 = four;
    }
    else  if (tens ==3){
        val1 = three;
    }
    else  if (tens ==2){
        val1 = two; 
    }
    else  if (tens ==1){
        val1 = one;
    }
    else {
      val1=zero;
    } 

    if (ones == 9 ){
        val2 = nine;
    } 
    else  if (ones ==8){
        val2 = eight;
    }
    else  if (ones ==7){
        val2 = seven;
    }
    else  if (ones ==6){
        val2 = six;
    }
    else  if (ones ==5 ){
        val2 = five;
    }
    else  if (ones ==4 ){
        val2 = four;
    }
    else  if (ones ==3){
        val2 = three;
    }
    else  if (ones ==2){
        val2 = two; 
    }
    else  if (ones ==1){
        val2 = one;
    }
    else {
      val2=zero;
    } 

    temperature(val1,val2);
    
}
void calculation() {
    int values[10];  // Array to store 10 temperature readings
    float sum = 0;   // To calculate the sum for mean
    uint8_t tens;
    uint8_t ones;
    // Gather 10 temperature readings
    for (int i = 0; i < 10; i++) {
        float temp = bmp.readTemperature(); // Read temperature
        while (isnan(temp) || temp < 0 || temp > 40) {  //only gather data that ranges from 0  to 40 inclusive.
           temp = bmp.readTemperature(); 
           delay(1000); 
            
        }
        
        int rounded_temp = round(temp); // Round to the nearest integer
        values[i] = rounded_temp;      // Store the value in the array
        sum += rounded_temp;           // Add to the sum
        delay(500);                    // Optional delay between readings
    }

    // Calculate the mean
    int mean = sum / 10;
    mean -= 5;  // **Subtract 3 degrees from the mean**

    //Serial.println("mean: " );
    //Serial.print(mean);
    if (mean >= 10){
      tens = mean / 10;
      ones = mean % 10;
     // temperature(tens,ones)
    } 
    else {
      tens = 0;
      ones= mean;
    }
   
    convert_for_display(tens,ones);
   }
void setup() {
  // Set up I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

 // Serial.begin(9600);
 // while (!Serial) delay(100);
 // Serial.println(F("BMP280 test"));

  unsigned status;
  status = bmp.begin(0x76); // Change to 0x77 if needed

  display.setBrightness(5); // Set brightness (0 to 7)
  display.clear();          // Clear the display
                       
  reset();
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  calculation();  // Run once at startup
  //lastRunTime = millis(); // Initialize timer
}


void loop() {
    
    // unsigned long currentTime = millis(); // Get the current time in milliseconds
    // // Check if one hour has passed
    // if (currentTime - lastRunTime >= interval) {
    //   lastRunTime = currentTime; 
    //   calculation();         
    // }
    calculation(); 
    delay(900000);

}
