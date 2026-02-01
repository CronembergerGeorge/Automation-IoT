#include <DHT.h> /* Importa biblioteca para manipulação do módulo temperatura e humidade*/
#include <Servo.h> /* Importa biblioteca para manipulação do módulo do Servo Motor SG90*/

#define DHTPIN 2 /* Define a porta do módulo*/
#define DHTTYPE DHT11 /* Define o modelo do módulo*/
DHT dht(DHTPIN, DHTTYPE); /* Cria variável para receber dados do modulo*/

Servo meuServo; /* Cria variavel para Servo Motor*/
const int ledVerde = 8; /* Define porta para Led Verde*/
const int ledVermelho = 9; /* Define porta para Led Vermelho*/

const int numLeituras = 10; /* Define o numero de leituras para media movel*/
float leiturasTemp[numLeituras]; /* Cria a lista da media*/
int indice = 0; 
 
void setup(){
  Serial.begin(9600); /* Define a taxa de tranferencia*/
  dht.begin(); /* Inicializa o módulo para leitura de temperatura e humidade*/
  meuServo.attach(31); /* Define porta para o Servo Motor*/
  pinMode(ledVerde, OUTPUT); /* Define padrão de  leds*/
  pinMode(ledVermelho, OUTPUT);

  meuServo.write(0); /* Define o angulo inicializado do servo*/
  digitalWrite(ledVerde, HIGH); /* Define o inicio dos leds (Ligado | Desligado)*/
  digitalWrite(ledVermelho, LOW);

  for(int i = 0; i < numLeituras; i++){ 
    leiturasTemp[i] = 0; /* Preenche o array com 0 para que o calculo nao inicie calculo com numeros lixos da memoria do arduino*/
  }

}

void loop(){

  // 1. Leitura do Sensor
  float temp_imediata = dht.readTemperature(); /* Cria a variável para receber dados de temperatura*/
  // 2. Lógica da Média Móvel
  if(!isnan(temp_imediata)){ /* Cria condicional que se receber dados de temperatura entra na condição*/
    leiturasTemp[indice] = temp_imediata; /*indice da lista recebe dados de temperatura*/
    indice = (indice + 1) % numLeituras; /* indice incrementa de 0 a 9(volta para 0 após o 9)*/

  }
  // 3. Cálculo da Média
  float somaTemp = 0; /* variavel da soma das temperaturas*/
  for(int i = 0; i < numLeituras; i++){ /*loop para preencher lista de leituras de temperatura*/
    somaTemp += leiturasTemp[i]; /* Soma valores das leituras de temperatura*/
  }

  float tempMedia = somaTemp / numLeituras;
  float humi = dht.readHumidity(); /* variavel que recebe dados de humidade*/
  int luz = analogRead(A0); /* variavel que recebe dados de intensidade da luz em porta analogica*/
  // 4. Envio de Dados (Protocolo CSV para o Python)
  if (!isnan(humi) && !isnan(temp_imediata)){ /* Condição para se receber dados de temperatura AND humidade, entra na condição*/
    Serial.print(tempMedia); /* Envia os dados via Serial separando por virgulas*/
    Serial.print(",");
    Serial.print(humi);
    Serial.print(",");
    Serial.println(luz);
  }
  // 5. Recebimento de Comandos do Software (Python)
  if(Serial.available() > 0){ /* Condição para saber se está recebendo dados via Serial do software*/
    char comando = Serial.read(); /* variavel para receber instruções*/

    if(comando == 'Q'){ /* Se software enviar Q via serial*/
      digitalWrite(ledVerde, LOW); /* Luz verde DESLIGADA*/
      digitalWrite(ledVermelho, HIGH); /* Luz vermelha LIGADA*/
      meuServo.write(180); /* Move o servo para posição de exaustão máxima */
      

    }
    else if(comando == 'F'){ /* Se software enviar F via Serial*/
      digitalWrite(ledVerde, HIGH); /* LED verde LIGADO*/
      digitalWrite(ledVermelho, LOW); /* LED Vermelho DESLIGADO*/
      meuServo.write(0); /* Retorna o servo para posição Inicial*/

    }

  }

  delay(500); /* Intervalo entre as leituras*/
}