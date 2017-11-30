# ControleDigital
Projeto seguidor de Parede.<br />

# Equipe: 

  -<a href="https://github.com/edsongjr">Edson Junior 1310780/7 </a><br />
  -<a href="https://github.com/Lucasabrants">Lucas Abrantes 1320653/8 </a><br />
  -<a href="https://github.com/Rodrigogcosta">Rodrigo Costa 1320661/9 </a><br />
  -<a href="https://github.com/faidertms">Thiago Sales 1410702/9</a><br />

# 1. Introdução
  <p align="justify">
  Este projeto tem como objetivo geral a montagem e a configuração do hardware, bem como a implementação de funções de um robô 
  móvel que seja hábil a locomover-se de maneira autônoma com base na percepção de uma parede. Para a realização deste objetivo 
  foram usados motores DC, ponte H, sensor do tipo ultrassônico, arduino como controlador e para ajustar a malha PID foi usada a 
  técnica “tentativa e erro”. A figura 1 abaixo mostra como ficou a implemetação física do projeto:</p><br />
  <p align="center">
  <img src="https://i.imgur.com/11IvZEE.jpg"><br/>
  <b>Figura 1.</b>
  </p>
 
 # 1.1	Atuadores
•	2 motores DC.<br />
   <p align="justify">
   O Motor DC é um atuador rotativo ou linear que garante o controle, velocidade e precisão em aplicações de controle de posição em malha fechada.</p>
   
•	Ponte H.<br />
   <p align="justify">
   Ponte H é um circuito electrônico que permite variar velocidades de um motor DC, assim como comutar o sentido de rotação dos motores, através de um sinal PWM. Estes circuitos são muito utilizados em robótica e estão disponíveis em circuitos prontos ou podem ser construídos por componentes.</p>
   
 #   1.2	Sensores
 •	Sensor ultrassônico.<br />
   <p align="justify">
   Os sensores ultrassônicos são amplamente utilizados para a percepção do ambiente e detecção de obstáculos em muitas aplicações robóticas. Estes sensores são muito bem sucedidos em termos de eficiência de custos, tempo de processamento e precisão.</p>
 
#  1.3	Controlador
•	Arduino.<br />
   <p align="justify">
   O arduino é um microcontrolador voltado para prototipação de projetos interativos, sendo o mesmo open (hardware e software) de fácil    manuseio, baixo custo e flexível.</p>

#  1.4	Lista de Materiais
•	Corpo de chapa metálica ou plástico.<br />
•	2 Rodas para os motores.<br />
•	2 Motores DC.<br />
•	Ponte H.<br />
•	Protoboard, fiação e bateria 7~9v.<br />
•	1 Sensores Ultrassônicos(HC-SR04).<br />
•	Arduino.
<br />
					     
# 2.	Embasamento teórico/prático
# 2.1   Fluxograma
  <p align="justify">
  O fluxograma mostrado na figura 2 ilustra a cadeia de eventos que ocorre no programa desenvolvido. O cálculo do tempo de execução do     código é utilizado para encontrar “dt” para que possamos encontrar “kd” e “ki”. Posteriormente é disparado um delay de 5 milissegundos   para iniciar a leitura do sensor ultrassônico (foi necessário esse delay, pois foi observado que inicialmente estavam sendo feitas       várias leituras de valores zeros). Depois da leitura o programa define uma rota com base na distância calculada pelo sensor frontal.     Nesse caso, se for detectado um obstáculo a 12 centímetros pelo sensor frontal então o carro vira pra esquerda e volta ao primeiro       evento do programa, caso contrário ele continua o seu trajeto normal realizando as correções necessárias com base nos valores           detectados pelo sensor lateral, para mantê-lo a 20 centímetros da parede. Mais detalhes do funcionamento do sistema são mostrados no     tópico 3(Implementação).</p>
  <p align="center">
  <img src="https://i.imgur.com/kVH9Ywd.jpg"><br/>
  <b>Figura 2.</b>
  </p>
  
# 2.2   Esquemático do projeto
  <p align="justify">
  Como é visto na figura 3, foram usadas somente as portas digitais do arduino para os atuadores e sensores, porem para os atuadores as   portas estão em modo pwm. Na parte da alimentação foi usada a porta de 5v para os sensores e a porta Vin para os atuadores e um Gnd     comum para todos.</p>
  <p align="center">
  <img src="https://i.imgur.com/2NhJWR1.png"><br/>
  <b>Figura 3.</b>
  </p>

# 2.3	Diagrama de Blocos
  <p align="justify">
  Na figura 4 o controlador, ou seja o arduino utilizado no projeto manda um sinal para o atuador (servos motores), para que esse         componente possa atuar no sistema. De forma paralela o sensor retroalimenta o sistema de controle, mantendo o sistema sempre             alimentado com novos dados(distâncias), que são por sua vez processados pelo programa contido no controlador que envia um sinal de       resposta para o atuador, e assim por diante.</p>
  <p align="center">
  <img src="https://i.imgur.com/QJ9NOH3.jpg"><br/>
  <b>Figura 4.</b>
  </p>

# 3.   Implemetação
# 3.1  Melhorias no PID
<p align="justify">
  Algumas melhorias foram implementadas no algoritmo PID a fim de torná-lo mais robusto, eficiente, e de forma a 
	manter a melhor estabilidade do sistema. As melhorias adotadas foram:</p>
	•	Sample Time<br />
	•	Derivative Kick<br />
	•	On-the-fly Tuning changes<br />
	•	Reset Windup Mitigation<br />
	
# 3.1.1  Sample Time
<p align="justify">
  Essa melhoria tem como finalidade definir um tempo de amostragem no qual o controlador PID será aplicado ao sistema de forma frequente. Em outras palavras, o PID será aplicado de tempos em tempos, e esse tempo é chamado de amostra. Nesse projeto essa melhoria torna-se muito importante para garantir que a função que calcula a distância, tenha tempo para poder fazer a leitura do sensor. Como o sensor necessita de um pulso de 10us em nível alto para poder disparar o sinal sonoro, e após isso esperar o sinal de resposta para calcular essa distância, é preciso definir um tempo de amostragem bem maior que 10us, para a leitura ser relizada. O tempo de amostragem escolhido foi de 50ms. Dessa forma o controlador não será aplicado várias vezes em um curto espaço de tempo, evitando assim computações nesnecessárias, e não tardará a ser aplicado, evitando a demora na correção definida no PID. A amostra é definida no código pela seguinte função: </p>
  
  
   ```
   void setAmostra(int Amostra){

    if(Amostra > 0){

    double ratio = (double)Amostra / (double)amostra;

    ki *= ratio;

    kd /= ratio;

    amostra = (unsigned long)Amostra;

  }

}
 ```
<p align="justify">
  Na função definirErro o tempo de execução do loop(dt) é calculado pela função "calcularTempo" e esse valor é
	acumulado na varivável dt. Caso dt atinga esse tempo de amostragem então a correção é aplicada ao sistema. Após a correção ser aplicada, a variável dt é zerada e então o processo se repete.   </p>
	
   ```
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
```
<p align="justify">
 Com essa melhoria o sistema ainda encontrou-se bastante instável, principalmente por que não havia controle da 
	velocidade dos motores DC, o que fazia com que o correção do PID fosse aplicada de forma muito brusca, o que levava o sensor de distância perder o referencial. Os vídeos abaixo mostram o comportamento do seguidor de parede com essa melhoria:   </p>
	

  
  # 3.1.2  Derivative Kick
<p align="justify">
 : </p>
  
  # 3.1.3  On-the-fly Tuning changes
<p align="justify">
   </p>
  
  # 3.1.4  Reset Windup Mitigation
<p align="justify">
   </p>

# 3.2  Funcionamento
   ```
 #include <Servo.h>
   ```
  <p align="justify">
  Definindo as variáveis de controle do projeto, para os dois servos motores e os dois sensores ultrassônicos.</p>
   
   ```
   int ENA = 3;
   int ENB = 5; 
   int IN1 = 7;
   int IN2 = 8;
   int IN3 = 6;
   int IN4 = 9; 
   int echoLateral = 10;
   int trigLateral = 11
   ```
    
  <p align="justify"> 
  Na função setup( ) são configurados os pinos utilizados na placa de prototipagem Arduino, essa configuração serve para determinar o     funcionamento de cada um deles, como por exemplo os pinos de trigFrontal e trigLateral, estão configurados para a função saída de       sinal, enquanto que, echoFrontal e echoLateral estão funcionando como entrada. Logo em seguida são determinados os pinos que irão       controlar a velocidade e o sentido da rotação de cada um dos motores, através das funções rodaEsquerda.attach(5) e                       rodaDireita.attach(6).</p>

```
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
  setAmostra(50);
  setTunings(2,0.3,0.1);
  SetOutputLimits(110,160);
}
```
  <p align="justify">
  Como o próprio nome já diz a função dispararPulso (int pinEcho, int pinTrig) é responsável por disparar um pulso de sinal digital por   10µs, através dos comandos digitalWrite(pinTrig, HIGH), delayMicroseconds(10) e digitalWrite(pinTrig, LOW) no pino de trig, essa         sequência de comandos dá início a leitura do sensor ultrassônico, que funciona da seguinte forma, o emissor dispara uma onda sonora     de alta frequência que esbarra em algum obstáculo e reporta para o receptor, enquanto isso é medido o tempo total desse percurso e       esse intervalo é estimado em µs, esse valor é retornado através do pino de echo do sensor e é armazenado na variável tempo.</p>
  
  ```
  int dispararPulso (int pinEcho, int pinTrig){
  float tempo;
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  tempo = pulseIn(pinEcho, HIGH);
  return tempo;
}
```
  <p align="justify">
  Para calcular a distância entre o sensor e o objeto a sua frente precisamos calcular o tempo em µs que o som leva para percorrer um 1   cm sabendo que a velocidade do som é de 34000 cm/s:</p>
  <p align="center">
  <img src="https://i.imgur.com/y1faDqW.png"><br/>
  </p>
  
  <p align="justify">
  Resolvendo essa regra de três temos que o som percorre 29,4 cm/µs, mas o valor retornado pela função dispararPulso( ) é o tempo de       ida e volta do som, então devemos dividir tudo isso ainda por dois para obtermos a distância correta.</p>
  
  ```
 int calcularDistancia (int pinEcho, int pinTrig){
  return (dispararPulso(pinEcho, pinTrig)/29.4)/2;
}
```

  <p align="justify">
  Essa função é utilizada para rotacionar o carro em seu próprio eixo para a frente, e recebe como parãmetros
	as velocidades das duas rodas.</p>

```
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
```

  <p align="justify">
  Essa função é utilizada para rotacionar o carro em seu próprio eixo para o lado esquerdo a uma velocidade predeterminada pelos           parâmetros recebidos pela função.</p>
   
```
void esquerda(int velocidadeMotor1, int velocidadeMotor2){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENA,velocidadeMotor1);
  analogWrite(ENB,velocidadeMotor2+75);//70 65
    
}
```

 <p align="justify">
  Essa função é utilizada para rotacionar o carro em seu próprio eixo para o lado direito a uma velocidade predeterminada pelos           parâmetros recebidos pela função.</p>
   
```
void direita(int velocidadeMotor1, int velocidadeMotor2){
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,velocidadeMotor1 + 75);//
  analogWrite(ENB,velocidadeMotor2);
  
}
```

  <p align="justify">
  Aqui estão declaradas as variáveis utilizadas no cálculo do controlador PID e as que armazenam os valores durante a execução das         medições e as constantes iniciais.</p>

```
double distancia = 0;
double ultimaDistancia;
double erro, dt;
int x, y;
double kd, kp, ki, i, d;
double errSum, lastErr;
int cons = 100;
int amostra = 100; // 1seg
double outMin, outMax
```
  <p align="justify">
  A função definirErro(int setPoint) calcula o erro através da diferença entre o setPoint e a distância do carro à parede, caso o sensor   lateral encontre uma barreira, o controlador irá calcular esse erro e estabelecer um valor para x e y para que o carro permaneça         sempre a uma distância da parede igual ao valor de setPoint, caso contrário será estabelecido valores fixos para x e y para que o       carro mantenha um percurso retilíneo.</p>

```
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
```
 <p align="justify">
  Para determinar os valores do integrativo e do derivativo da função PID precisamos calcular o tempo em que o programa leva para         executar um loop completo, esse valor será o dt da função.</p>

```
long tempoFinal=0;
long tempo = 0 ;

void calcularTempo(){
  tempo = tempoFinal;
  tempoFinal = millis();
  dt += (double)(tempoFinal - tempo);
}
```
  <p align="justify">
  Esse é o laço principal do programa onde chamamos as funções responsáveis pelo funcionamento do carro.</p>

```
void loop() {
  definirErro(20);
  control();
}
```

# 4.	Conclusão
  <p align="justify">
  Aplicações desse tipo de projeto são bem difíceis de serem realizadas, já que a funcionalidade do robô seguidor de parede é apenas       manter uma distância fixa de uma parede enquanto se locomove para frente. Nas pesquisas realizadas não foram achadas aplicações desse   projeto, porém é notável que esse tipo de aplicação tem um potencial bastante elevado, e portando pode ser evoluído para a construção   de um produto. Tal produto poderia ser um robô capaz de mapear a planta interna de uma residência e calcular suas dimensões, afim de     que não necessite mais de um trabalho humano braçal para realizar tal atividade. Outra aplicação possível seria a utilização desse       sistema de controle em automóveis, para que os mesmos mantenham uma distância segura das barreiras de contenção em pontes, podendo       evitar portando graves acidentes automobilísticos. Tal controle poderia ser utilizado também para manter uma distância segura do         veículo que está na frente e do que está atrás, e diminuir bastante o número de acidentes ocasionados por freadas bruscas. Esse tipo     de sistema já pode ser encontrado nos carros autônomos produzidos pela Tesla Motors, porém com um custo bastante elevado, já que os     automóveis produzidos são quase ou completamente autônomos. Dessa maneira é possível notar uma grande quantidade de aplicações           possíveis que podem ser desenvolvidas apenas com a princípio básico do controle de distância utilizando sensores ultrassônicos.         Portanto criar um produto que seja comercializável e que alcance uma grande massa de consumidores, se torna a tarefa mais desafiadora.   </p>






  
                                        
                                
