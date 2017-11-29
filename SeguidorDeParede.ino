int ENA = 3;
int ENB = 5; 
int IN1 = 7;
int IN2 = 8;
int IN3 = 6;
int IN4 = 9; 
int echoLateral = 10;
int trigLateral = 11;

void setup( ){

  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  pinMode(trigLateral, OUTPUT);
  pinMode(echoLateral, INPUT);

  digitalWrite(trigLateral, LOW);
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  setAmostra(50);//10
  setTunings(2,0.3,0.1);//2 ,0.2 0.08
  SetOutputLimits(110,160);//  100 - 140
  //Serial.begin(9600);
}



int dispararPulso (int pinEcho, int pinTrig){
  float tempo;
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  tempo = pulseIn(pinEcho, HIGH);//,5000);//4500
  return tempo;
}



int calcularDistancia (int pinEcho, int pinTrig){
  return (dispararPulso(pinEcho, pinTrig)/29.4)/2;
}



void frente(int velocidadeMotor1, int velocidadeMotor2){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);

  analogWrite(ENA,velocidadeMotor1);
  velocidadeMotor2 *= 1.05;
  if (velocidadeMotor2 > 255) velocidadeMotor2=255;
  analogWrite(ENB,velocidadeMotor2);

}
void esquerda(int velocidadeMotor1, int velocidadeMotor2){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENA,velocidadeMotor1);
  analogWrite(ENB,velocidadeMotor2+75);//70 65
    
}

void direita(int velocidadeMotor1, int velocidadeMotor2){
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,velocidadeMotor1 + 75);//
  analogWrite(ENB,velocidadeMotor2);
  
}

double distancia = 0;
double ultimaDistancia;
double erro, dt;
int x, y;
double kd, kp, ki, i, d;
double errSum, lastErr;
int cons = 100;
int amostra = 100; // 1seg
double outMin, outMax;

void definirErro(int setPoint){ 
  calcularTempo();
  if(dt >= amostra){
      distancia = calcularDistancia(echoLateral, trigLateral);
      if(distancia > 0 && distancia < 60){
        erro =  setPoint - distancia;
        double dDistancia = (distancia - ultimaDistancia);
        i += ki * erro; //Tuning Changes
        if(i > 15){// +15
         i = 15;
        }else if(i < -15){ // -15
         i = -15;
        }
    //Serial.println(d);
        d = kd * dDistancia;
        int p = (erro*kp);
        x = cons + (p - d + i);
        y = cons - (p - d + i);
        limiteXY();
        ultimaDistancia = distancia;
      }
      dt = 0;
  }

}



void control(){
  if(x >= 0 && y>= 0){
    frente(x,y); 
  }else if (x < 0 && y> 0){
    direita(-x,y);
  }else if(x > 0 && y< 0){
    esquerda(x,-y);
  }
}



void limiteXY(){
   if(x > outMax){ 
    x = outMax;
   }else if(x < outMin){
    x = outMin;
   }
   if(y > outMax){ 
    y = outMax;
   }else if(y < outMin){
    y = outMin;
   }
}


void SetOutputLimits(double Min, double Max){ // RESET WIDNUP{
   if(Min < Max){
     outMin = Min;
     outMax = Max;
   }
}



void setTunings(double Kp, double Ki , double Kd){
  double amostraEmSeg = ((double)amostra/1000);
  kp = Kp;
  ki = Ki * amostraEmSeg;
  kd = Kd / amostraEmSeg;
}



void setAmostra(int Amostra){

  if(Amostra > 0){

    double ratio = (double)Amostra / (double)amostra;

    ki *= ratio;

    kd /= ratio;

    amostra = (unsigned long)Amostra;

  }

}
long tempoFinal=0;
long tempo = 0 ;

void calcularTempo(){
  tempo = tempoFinal;
  tempoFinal = millis();
  dt += (double)(tempoFinal - tempo);
}


void loop() {
  definirErro(20);
  control();
}
