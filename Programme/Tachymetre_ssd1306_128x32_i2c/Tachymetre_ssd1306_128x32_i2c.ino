

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C 
//Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Adafruit_SSD1306 oled(128, 32, &Wire, -1);

volatile long comptageImpuls=0; // variable accessible dans la routine interruption externe 0
long timeRef=0; // variable pour temps de référence en millisecondes
long delai=1000; // variable pour délai de comptage en millisecondes


void setup() {

  if(!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  attachInterrupt(0, ComptageImpulsion, RISING); // attache l'interruption externe n°0 à la fonction gestionINT0()
  timeRef=millis(); // initialisation de la référence du comptage

  Serial.begin(9600);

  oled.display();
  delay(2000);
  // Clear the buffer
  oled.clearDisplay();
}

void loop() 
{
  if (millis()>(timeRef+delai)) 
    { 
      timeRef=timeRef+delai; // réinitialise le délai de comptage
     Affichage(comptageImpuls*30); // affiche nombre tours/sec
     comptageImpuls=0; // RAZ comptage impulsion
    }
}

// Affichage Oled
void Affichage(int tr)
{
  oled.clearDisplay();  
  oled.setTextSize(3); 
  oled.setTextColor(WHITE); 
  oled.setCursor(20,10);
  oled.print(tr);
  oled.display();
}

// ////////////////////////// FONCTIONS DE GESTION DES INTERRUPTIONS ////////////////////

// ------------------- fonction de gestion l'interruption externe n°0 (broche 2) ----------------
// cette fonction est appelée à chaque fois que l'interruption a lieu selon le mode configuré (LOW, CHANGE, RISING, FALLING)

void ComptageImpulsion()
{
  comptageImpuls++;
}
