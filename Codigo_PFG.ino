#define FSRIndice 32 //Pin que corresponde al indice, pulgar y corazon
#define FSRAnular 33 //Pin que corresponde a los 2 dedos restantes anular y meñique
#define EMG 26 // Pin que se asigna al sensor EMG
#include <ESP32Servo.h> // Libreria para usar servomotores
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 500

Servo Indice; // crea el objeto servo para el indice
Servo Anular; // crea el objeto servo para el anular

int posIndice = 0;   // posicion del servo indice
int posAnular = 0; // posicion del servo anular

int minUs = 1000;
int maxUs = 2000; //pulsos minimos y maximos que lee el sg90

int PinIndice = 14; //pin 14 usado para servo indice
int PinAnular = 12; // pin 12 usado para servo anular

int PresionFsr = 1300;
int ValorEMG = 1000;


TaskHandle_t Tarea2;

void loopAnular(void * parametro) { //Esta funcion se encarga de todos los movimientos de los dedos anulares
  
  for(;;){
    
    int LecturaAnular = analogRead(FSRAnular);
    int LecturaEMG = analogRead(EMG);

    
    if ((LecturaAnular > PresionFsr) && (posAnular == 0)){
      while(posAnular < 180){
        posAnular++;
        Anular.write(posAnular);              
        delay(15); 
      }
    }
    if ((LecturaEMG > ValorEMG) && (posAnular == 180)){
      while(posAnular > 0){
        posAnular--;
        Anular.write(posAnular);                 
        delay(15);
     } 
   }
  }
 }

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  Indice.setPeriodHertz(50);
  Anular.setPeriodHertz(50);
  Indice.attach(PinIndice, minUs, maxUs); //A que pin se le asigna Indice
  Anular.attach(PinAnular,minUs, maxUs); // A que pin se le asigna Corazon

  xTaskCreatePinnedToCore(
    loopAnular, //mismo nombre que la funcion void loopAnular
    "MovimientoAnular", //nombre de la tarea
    1024, // memoria asignada a esta tarea
    NULL, // parametros compartidos
    1, //prioridad de la tarea
    &Tarea2, //referencia del nombre de taskhandle_t
    0); //core en el que se ejecuta

    esp_task_wdt_init(WDT_TIMEOUT, false); // Impido que el WDT reincie la esp32 (false)
}

void loop() {
  
  int LecturaIndice = analogRead(FSRIndice);
  int LecturaEMG = analogRead(EMG);
  
  if ((LecturaIndice > PresionFsr) && (posIndice == 0)){
      while(posIndice < 180){
        posIndice++;
        Indice.write(posIndice);              
        delay(15); 
      }
    }
    Serial.println(posIndice);
    Serial.println(LecturaEMG);
    if ((LecturaEMG > ValorEMG) && (posIndice == 180)){
      while(posIndice > 0){
        posIndice--;
        Indice.write(posIndice);                 
        delay(15);
    } 
   }
}
