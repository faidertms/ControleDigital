#include <Servo.h>

Servo rodaDireita;
Servo rodaEsquerda;
 
int echoFrontal = 2;
int trigFrontal = 3;
int echoLateral = 8;
int trigLateral = 7;

void setup( ){
  pinMode(trigFrontal,OUTPUT); 
  pinMode(echoFrontal,INPUT);
  pinMode(trigLateral,OUTPUT);
  pinMode(echoLateral,INPUT);
  digitalWrite(trigFrontal,LOW);
  digitalWrite(trigLateral,LOW);
  rodaEsquerda.attach(5);
  delay(1);
  rodaDireita.attach(6);
  rodaEsquerda.write(90);
  rodaDireita.write(90);
}

int dispararPulso (int pinEcho, int pinTrig){
  float tempo;
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  tempo = pulseIn(pinEcho, HIGH,4500);
  return tempo;
}

int calcularDistancia (int pinEcho, int pinTrig){
  return dispararPulso(pinEcho,pinTrig)/29.4/2;
}

void esquerda(int velocidadeD,int velocidadeE){
  rodaEsquerda.write((90-velocidadeD));
  rodaDireita.write((90-velocidadeE));
}

void frente(int velocidadeD, int velocidadeE){
  velocidadeE = (velocidadeE / 1.5);
  rodaEsquerda.write((90+velocidadeE));
  rodaDireita.write((90-velocidadeD));
}

int distancia;
int ultimaDistancia;
int erro,x,y,dt;
int kd=10,kp=3;
double errSum, lastErr;
float i,d,ki = 0.001;
int cons = 15;

void definirErro(int setPoint){ 
  ultimaDistancia = distancia;
  distancia = calcularDistancia(echoLateral,trigLateral);
  if(distancia > 0){
    erro =  setPoint - distancia;
    errSum += (erro * dt);
    double dErr = (error - lastErr) / dt;
    i = ki * errSum;
    d = kd * dErr;
    x = cons + ((erro*kp) + d + i);
    y = cons - ((erro*kp) + d + i);
    lastErr = error;
  }
  if(distancia == 0){
    x = 10;
    y = 15;
  }
}

int temp ;
int tempFinal = millis();
void calcularTempo(){
  temp = tempFinal;
  tempFinal = millis();
  dt = (tempFinal - temp);
}

int distanciaFrontal;
void definirRota(){
  distanciaFrontal = calcularDistancia(echoFrontal,trigFrontal);
  if(distanciaFrontal > 0 && distanciaFrontal <= 12){
    esquerda(20,20);
    delay(1000);
    i=0;
    distancia = 0;
    distanciaFrontal = 0;
  }else{
    definirErro(20);
    frente(x,y);
  }
}

void loop() {
  calcularTempo();
  delay(5);
  definirRota();
}
