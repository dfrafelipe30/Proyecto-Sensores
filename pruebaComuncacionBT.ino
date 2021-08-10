//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial


// debido a que el adc del esp32 soporta maximo 3.3v, se necesita un divisor de tension
// para reducir el voltaje maximo de las baterias (4.2)

const float  R1 = 220.0; // valor en ohms de la primer resistencia del divisor de tension
const float R2 = 220.0; // valor en ohms de la segunda resistencia del divisor de tension

const float MaxBatteryVin = 4.2; // valor maximo de voltaje que la bateria

const float MaxBatteryVout = (R1 / (R1+R2)) * MaxBatteryVin; // valor maximo de voltaje leido despues de pasar por el divisor de tension

const float maxMap = (4095.0*MaxBatteryVout) / 3.33; // valor maximo que puede tomar el ADC en funcion del divisor de tension


int led0to25 = 14;
int led25to50 = 27;
int led50to75 = 26;
int led75to100 = 25;

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


float medirVoltaje(int muestras){
  int mediciones = 0;
  for(int i=0; i<muestras; i++){
    mediciones = mediciones + analogRead(34);
  }

  int media = mediciones / muestras;
  int voltaje = map(media*100, 0, maxMap*100, 0, MaxBatteryVin*100); 

  int nivel = map(voltaje, 300,  MaxBatteryVin*100, 1, 4);

  if(nivel == 1){
    digitalWrite(led0to25,HIGH);
    digitalWrite(led25to50,LOW);
    digitalWrite(led50to75,LOW);
    digitalWrite(led75to100,LOW);    
  }
  else if(nivel == 2){
    digitalWrite(led0to25,HIGH);
    digitalWrite(led25to50,HIGH);
    digitalWrite(led50to75,LOW);
    digitalWrite(led75to100,LOW);    
    
  }
  else if(nivel == 3){
    digitalWrite(led0to25,HIGH);
    digitalWrite(led25to50,HIGH);
    digitalWrite(led50to75,HIGH);
    digitalWrite(led75to100,LOW);     
  }
  else{
    digitalWrite(led0to25,HIGH);
    digitalWrite(led25to50,HIGH);
    digitalWrite(led50to75,HIGH);
    digitalWrite(led75to100,HIGH);   
  }


  return voltaje/100.0;
}


void setup() {
  Serial.begin(115200);

  pinMode(led0to25, OUTPUT);
  pinMode(led25to50, OUTPUT);
  pinMode(led50to75, OUTPUT);
  pinMode(led75to100, OUTPUT);

  pinMode(23,OUTPUT);
  digitalWrite(23,HIGH);
  
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Serial.print("El valor maximo de voltaje es: ");
  Serial.println(MaxBatteryVout);
}

void loop() {
  float voltaje = medirVoltaje(1000);
  
  String voltajeBt = String(voltaje);

  Serial.println(voltaje);
  
  if (true) {
    for(int i=0; i<voltajeBt.length(); i++){
      SerialBT.write(voltajeBt[i]);
    }
    SerialBT.write('\n');
    
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(100);
}
