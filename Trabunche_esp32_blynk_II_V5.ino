


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "Ke9T0_T8bl2tnBv5IS2Jkh-zr81vjhS2";
char ssid[] = "Cofradia náutica de Frutillar";
char pass[] = "navegantes2019";

//char ssid[] = "WifiVH";
//char pass[] = "12265165";
//char ssid[] = "Wifi_Patio";
//char pass[] = "0987654321";


int st=LOW;
int prevState = -1;
int currState = -1;
long lastChangeTime = 0;

BlynkTimer timer;

void checkPin() //chequear sensor inundación
{
  // Invert state, since button is "Active LOW"
  int state = !digitalRead(5); //sensor de rebalse

  // Debounce mechanism
  long t = millis();
  if (state != prevState) {
    lastChangeTime = t;
  }
  if (t - lastChangeTime > 50) {
    if (state != currState) {
      currState = state;
      Blynk.virtualWrite(V1, state); //inundado o no inundado
      int ledstat=state*255;
      Blynk.virtualWrite(V2, ledstat); //prende y apaga el led de inundación
    }
  }
  prevState = state;
}

void myTimerEvent() // para sensor voltaje
{
  
  digitalWrite(15, st);
  st=!st;
   Blynk.virtualWrite(V3, millis() / 1000); //segundos desde el inicio
   float bateria1=analogRead(A0); //el conector tres pines de más abajo
   
   float volt = map(bateria1, 2000, 3400, 90, 140); //map(value, fromLow, fromHigh, toLow, toHigh)
   Blynk.virtualWrite(V4, volt/10);
    Serial.print(bateria1);
    Serial.println(volt);
   int v2=analogRead(A3); //el conector tres pines de al medio
   Blynk.virtualWrite(V5, v2);
   int v3=analogRead(A6); //el conector tres pines de arriba
   Blynk.virtualWrite(V6, v3);
   int v4=digitalRead(4); // 
   Blynk.virtualWrite(V7, v4*255); //LED el conector de izquierda segundo de abajo a arriba
   int v5=digitalRead(2); // 
   Blynk.virtualWrite(V8, v5*255); //LED el conector de izquierda tercero de abajo a arriba

 
  
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(4, INPUT_PULLUP); //no usa resistencia
  pinMode(5, INPUT_PULLUP); // sensor inundación, no usa resistencia
    

timer.setInterval(1000L, myTimerEvent); // define el intervalo en que se ejecuta todo lo que está dentro de ese bloque de código
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer. Se usa en lectura de voltaje
  checkPin();
    
   
}
