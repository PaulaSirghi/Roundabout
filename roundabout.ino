#include "pitches.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // latime OLED in pixels
#define SCREEN_HEIGHT 32 // Inaltime OLED in pixels
#define OLED_RESET     4 // pin de reset
#define SCREEN_ADDRESS 0x3C /// 0x3D pentru 128x64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int red_light_pin= 5;
int green_light_pin = 4;
int blue_light_pin = 3;
int button_pin = 7;
int button_state ;
int speakerPin = 2;
float resolutionADC = .0049 ; // rezoluția implicită (pentru
//referința 5V) = 0.049 [V] / unitate
float resolutionSensor = .01 ; // rezoluție senzor = 0.01V/°C

int melody[] = {
 NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16};

void setup() {
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode (speakerPin, OUTPUT);
  pinMode (A0, OUTPUT);
  pinMode (A1, OUTPUT);
  pinMode (A2, OUTPUT);
  pinMode (A3, INPUT);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  digitalWrite(2,HIGH);
  
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);      // Start din coltul stanga sus
  display.cp437(true);         // 256 char 'Code Page 437' font
  display.write("buna");
  display.display();
}

int sem=0;
void loop() {
  digitalWrite(2,HIGH);
  RGB_color(0, 255, 0); // Green
  Serial.println("Green");
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start din coltul stanga sus
  display.cp437(true);         // 256 char 'Code Page 437' font
  display.write("Verde");
  display.display();
  digitalWrite(A2,LOW);
  float light = readLight(10, A3);
  float intensitate=0;
  Serial.println(light);
  if(light<0)
     intensitate=-light;
  analogWrite(A1, intensitate*10);   // aprindem becul cu o intensitate luminoasa care poate varia
  analogWrite(A0, intensitate*10);   // aprindem becul cu o intensitate luminoasa care poate varia
 // analogWrite(A1, 1000);
  delay(50); 
  button_state = digitalRead(button_pin);
  if ( button_state == 0) //daca butonul a fost apasat incrementam o variabila 
  {
    while(sem!=10){
      sem++;
      if( sem == 5) 
      {
         RGB_color(255, 255, 0); // Yellow
         Serial.println("Yellow");
          display.clearDisplay();
          display.setTextSize(1);     
          display.setTextColor(SSD1306_WHITE); // Draw white text
          display.setCursor(0, 0);      // Start din coltul stanga sus
          display.cp437(true);         // 256 char 'Code Page 437' fon
          display.write("Galben");
        
          display.display();
          digitalWrite(A2,LOW);   
      }
       delay(1000);
    }
     RGB_color(255, 0, 0); // Red
     Serial.println("Red");
     
     display.clearDisplay();
     display.setTextSize(1);      
     display.setTextColor(SSD1306_WHITE); // Draw white text
     display.setCursor(0, 0);      // Start din coltul stanga sus
     display.cp437(true);         // 256 char 'Code Page 437' font
     display.write("Rosu");
     display.display();
    
     digitalWrite(A2,HIGH);
     for (int thisNote = 0; thisNote < 32; thisNote+=2) {
        int noteDuration = 1000/6;
        tone(2, melody[thisNote],noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
     }
  }
  sem=0; 
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

float readLight(int count, int pin) {
 float sumlight = 0;
 for (int i =0; i < count; i++) {
     int reading = analogRead(pin);
     float voltage = reading * resolutionADC;
     float light = (voltage - 0.5) / resolutionSensor ;
     sumlight = sumlight + light; 
 }
 return sumlight / (float)count; // media returnată
}
