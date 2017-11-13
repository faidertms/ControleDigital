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
  setAmostra(30);
  setTunings(3,0.001,10);
  Serial.begin(9600);
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

double distancia;
double ultimaDistancia;
double erro,dt;
int x,y;
double kd,kp,ki,i,d;
double errSum, lastErr;
int cons = 15;
int amostra = 1000; // 1seg
double outMin, outMax;
void definirErro(int setPoint){ 

  calcularTempo();

  distancia = calcularDistancia(echoLateral,trigLateral);

  if(distancia > 0 && dt >= amostra){
    Serial.println(distancia);
    erro =  setPoint - distancia;
   /* if(errSum >= 50){
      errSum = 0;
    }
    errSum += erro; faz parte do sample TIME*/
    double dDistancia = (distancia - ultimaDistancia);
    i += ki * erro; //Tuning Changes
    if(i > outMax){// +90
     i = outMax;
    }else if(i < outMin){ // -90
     i = outMin;
    }
    d = kd * dDistancia;
    x = cons + ((erro*kp) - d + i);
    y = cons - ((erro*kp) - d + i);
    ultimaDistancia = distancia;
    //lastErr = error;
    dt = 0;
  }

  if(distancia == 0){
    x = 10;
    y = 15;
  }
}

void SetOutputLimits(double Min, double Max) // RESET WIDNUP
{
   if(Min < Max){
     outMin = Min;
     outMax = Max;
   }
   if(Output > outMax){
    Output = outMax;
   }else if(Output < outMin){
    Output = outMin;
   }
 
   if(i > outMax){
    i = outMax;
   }else if(i < outMin){
    i = outMin;
   }
}

void setTunings(double Kp, double Ki , double Kd){
  double amostraEmSeg = ((double)amostra/1000);
  kp = Kp;
  ki = Ki * amostra;
  kd = Kd / amostra;
}

void setAmostra(int Amostra){
  if(Amostra > 0){
    double ratio = (double)Amostra / (double)amostra;
    ki *= ratio;
    kd /= ratio;
    amostra = (unsigned long)Amostra;
  }
}

int temp ;
int tempFinal = millis();
void calcularTempo(){
  temp = tempFinal;
  tempFinal = millis();
  dt += (tempFinal - temp);
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
  definirRota();
}
