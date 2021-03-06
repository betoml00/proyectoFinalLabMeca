#include <SharpDistSensor.h>  // Librería para el sensor infrarrojo

#define trigPinIzq 52         // Disparador (Trigger)
#define echoPinIzq 53         // Eco (Echo)
#define trigPinDer 7          // Disparador (Trigger)
#define echoPinDer 6          // Eco (Echo)
#define irPin A0              // Sensor infrarrojo
#define motIzqA 12            // Motor izquierdo A
#define motIzqB 13            // Motor izquierdo B
#define motDerA 8             // Motor derecho A
#define motDerB 9             // Motor derecho B
#define enblIzq 10            // Enable izquierdo
#define enblDer 11            // Enable derecho
#define luzDer A1             // Sensor de luz izquierdo
#define luzIzq A3             // Sensor de luz derecho

// Sensor infrarrojo
const byte medianFilterWindowSize = 5;
SharpDistSensor sensorIR(irPin, medianFilterWindowSize);

// Sensores ultrasónicos
long duracion, distancia;

// Motores
int velIzq = 100; // 127
int velDer = 90; // Anterior: 127, 63, 85, 116, 100, 92, 88

// Sensores de luz
long valDer;
long valIzq;
long dif;

void setup() {
  Serial.begin (9600);

  // Sensores ultrasónicos
  pinMode(trigPinIzq, OUTPUT);
  pinMode(echoPinIzq, INPUT);
  pinMode(trigPinDer, OUTPUT);
  pinMode(echoPinDer, INPUT);

  // Sensor infrarrojo
  sensorIR.setModel(SharpDistSensor::GP2Y0A41SK0F_5V_DS);

  // Motores
  pinMode(motIzqA,OUTPUT);
  pinMode(motIzqB,OUTPUT);
  pinMode(motDerA,OUTPUT);  
  pinMode(motDerB,OUTPUT);
  pinMode(enblIzq,OUTPUT);
  pinMode(enblDer,OUTPUT);
  analogWrite(enblIzq, velIzq);
  analogWrite(enblDer, velDer);
}

void loop() {

  unsigned int distIR = sensorIR.getDist();
  long distU_Der = leeDistU_Der();  
  long distU_Izq = leeDistU_Izq();
  //Serial.println("Ult. izq.: " + String(distU_Izq));
  //Serial.println("Ult. der.: " + String(distU_Der));   
  //Serial.println("Infrarrojo: " + String(distIR));  
  valDer = analogRead(luzDer);
  valIzq = analogRead(luzIzq);
  dif = (valIzq-valDer-15)*0.6;
  long valMinLuz = 300;
  long valMaxLuz = 700;
  long valMinIR = 150;
  long valMinDist = 5;

  Serial.println("ValDer:" + String(valDer));
  
  if (valDer > valMaxLuz && valIzq > valMaxLuz) {
    detener(2000000);
  }
  else if (distIR < valMinIR) {
    //Serial.println("Infrarrojo detectó: " + String(distIR));
    atras(200);
    if (distU_Der > distU_Izq) {
      //Serial.println("US der. detectó más que el izq.: " + String(distU_Der));
      derecha(200);      
    } else {
      //Serial.println("US izq. detectó más que el der.: " + String(distU_Izq));
      izquierda(200);
    }
    adelante(200);
  } else if (distU_Der < valMinDist) {
    Serial.println("US der. detectó: " + String(distU_Der));
    izquierda(200);
  } else if (distU_Izq < valMinDist) {
    Serial.println("US izq. detectó: " + String(distU_Izq));
    derecha(200);
  } else {
    //Serial.println("No detectó nada");
    if(valDer - 15 > valMinLuz || valIzq > valMinLuz) {
      //Serial.println("Detectó luz");
      if(dif>0){
        if(velDer+dif>180){
          analogWrite(enblDer, 180);
        }
        else{
          analogWrite(enblDer, velDer+dif);
        }
        analogWrite(enblIzq, velIzq);
      }
      else{
        if(velIzq-dif>180){
          analogWrite(enblIzq, 180);
        }
        else{
          analogWrite(enblIzq, velIzq-dif*1.2);
        }
        analogWrite(enblDer, velDer+dif*1.2);
      }
      adelante(50);
    } else {
      //Serial.println("No detectó luz: " + String(valIzq) + ";" + String(valDer) + ";" + String(dif));       
      derecha(50);      
    }
  }      
}

// Función para leer el valor del sensor ultrasónico derecho
long leeDistU_Der() {
  digitalWrite(trigPinDer, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDer, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDer, LOW);
  pinMode(echoPinDer, INPUT);
  duracion = pulseIn(echoPinDer, HIGH);
  distancia = 0.0343*(duracion/2);
  
  return distancia;
}

// Función para leer el valor del sensor ultrasónico derecho
long leeDistU_Izq() {
  digitalWrite(trigPinIzq, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinIzq, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinIzq, LOW);
  pinMode(echoPinIzq, INPUT);
  duracion = pulseIn(echoPinIzq, HIGH);
  distancia = 0.0343*(duracion/2);
  
  return distancia;
}

void atras(int tiempo){
  // Motor izquierdo hacia atrás
  digitalWrite(motIzqA, HIGH);
  digitalWrite(motIzqB, LOW);

  // Motor derecho hacia atrás
  digitalWrite(motDerA, HIGH);
  digitalWrite(motDerB, LOW);
  Serial.println("Avanza hacia atrás");
  delay(tiempo);  
}

void adelante(int tiempo){
  
  // Motor izquierdo hacia adelante
  digitalWrite(motIzqA, LOW);
  digitalWrite(motIzqB, HIGH);

  // Motor derecho hacia adelante
  digitalWrite(motDerA, LOW);
  digitalWrite(motDerB, HIGH);
  //Serial.println("Avanza hacia adelante");
  delay(tiempo);  
}

void derecha(int tiempo) {
  // Motor izquierdo hacia atrás
  digitalWrite(motIzqA, HIGH);
  digitalWrite(motIzqB, LOW);

  // Motor derecho hacia adelante
  digitalWrite(motDerA, LOW);
  digitalWrite(motDerB, HIGH);
  //Serial.println("Gira hacia la derecha");
  delay(tiempo);  
}

void izquierda(int tiempo) {
  // Motor izquierdo hacia adelante
  digitalWrite(motIzqA, LOW);
  digitalWrite(motIzqB, HIGH);

  // Motor derecho hacia atrás
  digitalWrite(motDerA, HIGH);
  digitalWrite(motDerB, LOW);
  Serial.println("Gira hacia la izquierda");
  delay(tiempo);  
}

void detener(int tiempo) {
  // Freno pasivo. Detenemos ambos motores.
  digitalWrite(motIzqA, LOW);
  digitalWrite(motIzqB, LOW);
  digitalWrite(motDerA, LOW);
  digitalWrite(motDerB, LOW);
  Serial.println("Detener");
  delay(tiempo);  
}
