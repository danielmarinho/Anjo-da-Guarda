/////////////////////////////
//PROJETO DE ARQUITETURA DE COMPUTADORES 
/////////////////////////////
//Variaveis
//tempo de calibracao do sensor de movimento (10 a 60 segundos, 30 eh suficiente)
int calibrationTime = 30;        

//momento que o sensor recebe o valor LOW
long unsigned int lowIn;         

//tempo que o sensor tem que estar LOW para assumirmos que todo movimento parou de fato
long unsigned int pause = 3000;  

boolean lockLow = true;
boolean takeLowTime;  

// menor e maior valor que os sensores podem ter:
const int sensorMin = 0;     
const int sensorMax = 1024;  


//Portas dos sensores
int pirPin = 7;
int ledPin = 13;
int buzzer = 3;




/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);//Comunicacao com o computador aberta
  pinMode(pirPin, INPUT);//Sensor de movimento modo INPUT
  pinMode(ledPin, OUTPUT);//LED modo output
  pinMode(buzzer, OUTPUT);//Buzzer modo output
  digitalWrite(pirPin, LOW);//Sensor de movimento recebe sinal LOW (pois no setup nao estaremos lendo movimento)

  //Espera calibrar o sensor de movimento
  Serial.print("Calibrando sensor de movimento ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" terminado");
    Serial.println("SENSOR ATIVO");
    delay(50);
  }

////////////////////////////
//LOOP
void loop(){
  
  
//PARTE DE LEITURA DO SENSOR DE FOGO  
  // Lendo o sensor de fogo que esta na porta analogica 0
	int sensorReading = analogRead(A0);//quanto mais 'fogo' menor o valor de leitura, variando de 0 a 1024
  // mapeia os valores do sensor para o intervalo 0-4
	int valor = map(sensorReading, sensorMin, sensorMax, 0, 4);
        
  
  if(valor == 0 || valor == 1){//Os 2 primeiros valores significam que a leitura foi mais forte entao fogo proximo
      //Serial.println("Fogo proximo!");
      Serial.println("0");
      tone(buzzer, 300, 1000);
  }
  else if(valor == 2){//No 3o valor fogo distante (ou esta captando a luz solar por engano, nao avisar)
    //Serial.println("Fogo distante ou Luz Solar");
    Serial.println("1");
  }
  
  delay(1);  // delay entre as leituras em milisegundos
  
  
  
  
  
////////////////////////////////////////////  
//PARTE DA LEITURA DO SENSOR DE MOVIMENTO
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //se detectou movimento acende o LED
       if(lockLow){  
         //esperar sensor ir para LOW antes de passar informacao
         lockLow = false;            
         //Serial.println("---");
         //Serial.print("movimento detectado em ");
         //Serial.print(millis()/1000);
         //Serial.println(" segundo(s)");
        Serial.println("2"); 
         delay(50);
         }         
         takeLowTime = true;
       }

//     if(digitalRead(pirPin) == LOW){       
//       digitalWrite(ledPin, LOW);
//
//       if(takeLowTime){
//        lowIn = millis();          //salva quanto tempo demorou para mudar de HIGH para LOW save the time of the transition from high to LOW
//        takeLowTime = false;       //faz com que seja feito somente quando comecar uma fase LOW
//        }
//       //se o sensor ficar em LOW por mais tempo que a pausa definida, nos assumimos que nao havera mais movimento
//       
//       if(!lockLow && millis() - lowIn > pause){  
//           //faz com que esse bloco de codigo seja executado somente depois que um novo movimento for detectado
//           lockLow = true;                        
//           Serial.print("movimento finalizado em ");
//           Serial.print((millis() - pause)/1000);
//           Serial.println(" segundo(s)");
//           delay(50);
//           }
//       }
  }

