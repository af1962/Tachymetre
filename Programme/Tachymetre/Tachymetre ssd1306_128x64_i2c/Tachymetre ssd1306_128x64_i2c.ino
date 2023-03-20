#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int pourcent;

volatile long comptageImpuls=10; // variable accessible dans la routine interruption externe 0
long timeRef=0; // variable pour temps de référence en millisecondes
long delai=1000; // variable pour délai de comptage en millisecondes

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, ComptageImpulsion, RISING); // attache l'interruption externe n°0 à la fonction gestionINT0()
  timeRef=millis(); // initialisation de la référence du comptage

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  
}

void loop() {

  if (millis()>(timeRef+delai)) 
    { 
      timeRef=timeRef+delai; // réinitialise le délai de comptage      
	    Affichage();
     comptageImpuls=0; // RAZ comptage impulsion
    }
}    

void Affichage()
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    // Rectangle
    display.drawRect(1, 39, 122,24, SSD1306_WHITE);    
    display.drawRect(60, 34, 1,5, SSD1306_WHITE);  
    display.drawRect(122, 31, 1,8, SSD1306_WHITE);    
    display.setTextSize(1);
    display.setCursor(1,30);
    display.print("0");

    // Vitesse
    pourcent = map(comptageImpuls*30,0,12000,0,118);
    display.setCursor(40,10);
    display.setTextSize(2);   
    display.print(comptageImpuls*30);

    // Bargraphe
    display.fillRect(3, 41, pourcent,20, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
  }


// ------------------- fonction de gestion l'interruption externe n°0 (broche 2) ----------------
// cette fonction est appelée à chaque fois que l'interruption a lieu selon le mode configuré (LOW, CHANGE, RISING, FALLING)

void ComptageImpulsion()
{
  comptageImpuls++;
}
     
  
