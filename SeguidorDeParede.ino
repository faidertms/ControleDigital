
 #include <Servo.h>
  
 Servo rodaDireita;
 Servo rodaEsquerda;
 
 int echoFrontal = 2;
 int trigFrontal = 3;
 int echoLateral = 8;
 int trigLateral = 7;
 
 
 void setup() {
   // configura pino GATILHO como saÃ­da
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
 // configura pino ECHO como entrad
}
int dispararPulso (int pinEcho, int pinTrig){
  float tempo;
  // disparar pulso ultrassÃ´nico
  
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  // medir tempo de ida e volta do pulso ultrassÃ´nico
  tempo = pulseIn(pinEcho, HIGH,4200);
  return tempo;
}

int calcularDistancia (int pinEcho, int pinTrig){
  return dispararPulso(pinEcho,pinTrig)/29.4/2;
}

void esquerda(int velocidade,int velocidade2){
  rodaEsquerda.write((90-velocidade));
  rodaDireita.write((90-velocidade2));
  
}
void direita(int velocidade, int velocidade2){
  rodaEsquerda.write((90+velocidade));
  rodaDireita.write((velocidade2));
  
}
void frente(int velocidade, int velocidade2){
  rodaEsquerda.write((90+velocidade));
  rodaDireita.write((90-velocidade2));
  
}
int distancia;
int ultimaDistancia;
int erro,x,y,dt;
int kd=10,kp=1;
float i,ki = 0.001;
int cons = 15;
void definirErro(int setPoint){
  distancia = calcularDistancia(echoLateral,trigLateral);
  if(distancia > 0){
    ultimaDistancia = distancia;
    erro =  setPoint - distancia;
    i += (erro*dt*ki);
    x = cons + ((erro*kp) + (kd*(distancia - ultimaDistancia)/dt) + i);
    y = cons - ((erro*kp) + (kd*(distancia - ultimaDistancia)/dt) + i);
  }
}
int temp ;
int tempFinal = millis();
void calcularTempo(){
  temp = tempFinal;
  tempFinal = millis();
  dt = (tempFinal - temp);
}

void loop() {
  calcularTempo();
  //Serial.println(calcularDistancia(echoFrontal,trigFrontal));
  //Serial.println(calcularDistancia(echoLateral,trigLateral));
  definirErro(20);
  frente(y,x);

}


