/*
  Trabalho de ENGC50
  Nailane
  Taian
*/

//biblioteca do display LCD
#include <LiquidCrystal.h>

//definindo as variáveis de tempo
int relSegundo = 0;
int relMinuto = 0;
int relHora = 0;
int croSegundo = 0;
int croMinuto = 0;
int croHora = 0;
int croMilSeg = 0;
int desSegundo = 0;
int desMinuto = 0;
int desHora = 0;
int despertador = 0;
int tempoDelay = 150; //alterar aqui para tornar a resposta das teclas mais rápida ou mais lenta
int selecaoD = 0; //indica se o despertador está ativo
unsigned long previousMillis = 0; // última vez que o segundo foi atualizado para manter o relógio atualizado enquanto estiver em outros modos de operação.

//variável para seleção do modo de operação
int modo = 0;

//nomeando os pinos
const int Botao1 = A5;
const int Botao2 = A4;
const int Botao3 = A3;
const int Botao4 = A2;
const int Buzzer = A1;

//setando os pinos que vão conectar no display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//setando a leitura dos pinos
void setup() {
  pinMode(Botao1, INPUT);
  pinMode(Botao2, INPUT);
  pinMode(Botao3, INPUT);
  pinMode(Botao4, INPUT);
  pinMode(Buzzer, OUTPUT);
  lcd.begin(16, 2); //tamanho do lcd
  setRelogio(); //coloca o horário no relógio pois ao ligar ele virá zerado
  modo = 0; //começa no relógio
}

//o programa principal vai selecionar o procedimento a ser executado
void loop() {
  if (digitalRead(Botao4) == HIGH) {
    delay(tempoDelay);
    if (digitalRead(Botao4) == HIGH) {
      selecionarModo(); //chama a função para selecionar o modo de operação
    }
  }
  //irá executar o modo de operação definido pelo procedimento
  switch (modo) {
    case 0:
      exibirRelogio();
      break;
    case 1:
      setRelogio();
      break;
    case 2:
      setCronometro();
      break;
    case 3:
      setDespertador();
      break;
  }
  //tocar alarme quando der a hora definida no despertador
  if ((desHora == relHora) && (desMinuto == relMinuto) && (desSegundo == relSegundo) && (despertador == 1)) {
    tocarAlarme();
  }
}

//setando o relógio
void setRelogio() {
  int selecao = 0;
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  if (relHora < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(relHora);
  lcd.print(":");
  if (relMinuto < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(relMinuto);
  lcd.print(":");
  if (relSegundo < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(relSegundo);
  while (digitalRead(Botao4) == LOW) { //enquanto o botão MODO não for pressionado, continua recebendo valores
    delay(tempoDelay);
    if (digitalRead(Botao4) == LOW) { //delay para verificar se não foi interferência na rede
      if (digitalRead(Botao3) == HIGH) { //botão de seleção de hora/minuto/segundo
        selecao++;
        if (selecao == 3) {
          selecao = 0;
        }
      }
      switch (selecao) {
        case 0: //seleciona hora
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            relHora--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            relHora++;
          }
          if (relHora >= 24) { //zera a hora quando chega em 24
            relHora = 0;
          }
          if (relHora < 0) { //evita valor negativo
            relHora = 23;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga as horas   ");
          lcd.setCursor(0, 1);
          if (relHora < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(relHora);
          break;
        case 1: //seleciona minuto
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            relMinuto--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            relMinuto++;
          }
          if (relMinuto >= 60) { //zera o minuto quando chega em 60
            relMinuto = 0;
          }
          if (relMinuto < 0) { //evita valor negativo
            relMinuto = 59;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga os minutos ");
          lcd.setCursor(3, 1);
          if (relMinuto < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(relMinuto);
          break;
        case 2: //seleciona segundo
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            relSegundo--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            relSegundo++;
          }
          if (relSegundo >= 60) { //zera o segundo quando chega em 60
            relSegundo = 0;
          }
          if (relSegundo < 0) { //evita valor negativo
            relSegundo = 59;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga os segundos ");
          lcd.setCursor(6, 1);
          if (relSegundo < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(relSegundo);
          break;
      }
    }
  }
  lcd.clear();
  modo = 0; //voltar ao relógio ao terminar
  previousMillis = millis(); //garantir que vai começar a contar depois do ok
}

//setando o cronômetro
void setCronometro() {
  //variáveis para contar o tempo pelo relógio interno do arduino
  unsigned long currentMillisC = 0;
  unsigned long prevousMillisC = 0;
  int start = 0;
  int pause = 1;
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Cronometro ");
  lcd.setCursor(0, 1);
  lcd.print("00:00:00.000 ");
  while (digitalRead(Botao4) == LOW) { //enquanto não apertar MODO continua no cronômetro
    delay(tempoDelay);
    if (digitalRead(Botao4) == LOW) { //verificar se não foi acidente
      if (digitalRead(Botao3) == HIGH) { //botão para iniciar o cronômetro
        if (digitalRead(Botao3) == HIGH) { //verificar se não foi acidente
          start = 1;
          pause = 0;
          currentMillisC = millis();
          prevousMillisC = currentMillisC;
          croSegundo = 0;
          croMinuto = 0;
          croHora = 0;
          croMilSeg = 0;
        }
      }
      if (digitalRead(Botao2) == HIGH) { //parar o cronômetro
        if (digitalRead(Botao2) == HIGH) { //verificar se não foi acidente
          if (pause == 1) {
            pause = 0;
          }
          else {
            pause = 1;
          }
        }
      }
      if (digitalRead(Botao1) == HIGH) { //zerar o cronômetro
        if (digitalRead(Botao1) == HIGH) { //verificar se não foi acidente
          start = 0;
          pause = 1;
          croSegundo = 0;
          croMinuto = 0;
          croHora = 0;
          croMilSeg = 0;
          lcd.setCursor(0, 1);
          lcd.print("00:00:00.000 ");
        }
      }
      if (start == 1) { //se iniciado, realiza a contagem
        currentMillisC = millis();
        croMilSeg += currentMillisC - prevousMillisC;
        prevousMillisC = currentMillisC;
        if (croMilSeg >= 1000) { //se já tem mais de 1 segundo, atualizar
          croSegundo ++; //incrementa segundos pela diferença de tempo desde que foi atualizado pela última vez
          croMilSeg -= 1000;
          if (croSegundo >= 60) {
            croSegundo -= 60;
            croMinuto++;
            if (croMinuto >= 60) {
              croMinuto -= 60;
              croHora++;
              if (croHora >= 24) {
                croHora -= 24;
              }
            }
          }
        }
        if (pause == 0) {
          lcd.setCursor(0, 1);
          if (croHora < 10) { //mantem o formato 00:00:00.000
            lcd.print(0);
          }
          lcd.print(croHora);
          lcd.print(":");
          if (croMinuto < 10) { //mantem o formato 00:00:00.000
            lcd.print(0);
          }
          lcd.print(croMinuto);
          lcd.print(":");
          if (croSegundo < 10) { //mantem o formato 00:00:00.000
            lcd.print(0);
          }
          lcd.print(croSegundo);
          lcd.print(".");
          if (croMilSeg < 100) { //mantem o formato 00:00:00.000
            lcd.print(0);
            if (croMilSeg < 10) { //mantem o formato 00:00:00.000
              lcd.print(0);
            }
          }
          lcd.print(croMilSeg);
          lcd.print(" ");
        }
      }
    }
  }
  modo = 0; //voltar ao relógio ao terminar
  lcd.clear();
}

//setando o despertador
void setDespertador() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  if (desHora < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(desHora);
  lcd.print(":");
  if (desMinuto < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(desMinuto);
  lcd.print(":");
  if (desSegundo < 10) { //mantem o formato 00:00:00
    lcd.print(0);
  }
  lcd.print(desSegundo);
  if (despertador == 1) { //indicar se o despertador está ativo
    lcd.print(" S ");
  }
  else {
    lcd.print(" N ");
  }
  while (digitalRead(Botao4) == LOW) { //enquanto o botão de Ok não for pressionado, continua recebendo valores
    delay(tempoDelay);
    if (digitalRead(Botao4) == LOW) { //delay para verificar se não foi interferência na rede
      if (digitalRead(Botao3) == HIGH) { //botão de seleção de hora/minuto/segundo
        selecaoD++;
        if (selecaoD == 4) {
          selecaoD = 0;
        }
      }
      switch (selecaoD) {
        case 0: //seleciona hora
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            desHora--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            desHora++;
          }
          if (desHora >= 24) { //zera a hora quando chega em 24
            desHora = 0;
          }
          if (desHora < 0) { //evita valor negativo
            desHora = 23;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga as horas   ");
          lcd.setCursor(0, 1);
          if (desHora < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(desHora);
          break;
        case 1: //seleciona minuto
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            desMinuto--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            desMinuto++;
          }
          if (desMinuto >= 60) { //zera o minuto quando chega em 60
            desMinuto = 0;
          }
          if (desMinuto < 0) { //evita valor negativo
            desMinuto = 59;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga os minutos ");
          lcd.setCursor(3, 1);
          if (desMinuto < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(desMinuto);
          break;
        case 2: //seleciona segundo
          if (digitalRead(Botao1) == HIGH) { //se apertar o botão - diminui o valor
            desSegundo--;
          }
          if (digitalRead(Botao2) == HIGH) { //se apertar o botão + aumenta o valor
            desSegundo++;
          }
          if (desSegundo >= 60) { //zera o segundo quando chega em 60
            desSegundo = 0;
          }
          if (desSegundo < 0) { //evita valor negativo
            desSegundo = 59;
          }
          lcd.setCursor(0, 0);
          lcd.print("Diga os segundos ");
          lcd.setCursor(6, 1);
          if (desSegundo < 10) { //mantem o formato 00:00:00
            lcd.print(0);
          }
          lcd.print(desSegundo);
          break;
        case 3: //ativar/desativar despertador
          if ((digitalRead(Botao1) == HIGH) || (digitalRead(Botao2) == HIGH)) { //se apertar o botão + ou - muda o valor
            if (despertador == 0) {
              despertador = 1;
            }
            else {
              despertador = 0;
            }
          }
          lcd.setCursor(0, 0);
          lcd.print("Ativar o alarme? ");
          lcd.setCursor(9, 1);
          if (despertador == 1) {
            lcd.print("S ");
          }
          else {
            lcd.print("N ");
          }
          break;
      }
    }
  }
  lcd.clear();
  modo = 0; //voltar ao relógio ao terminar
}

//exibindo o relógio no display LCD
void exibirRelogio() {
  atualizarRelogio(); //atualizar a hora antes de exibir
  lcd.setCursor(0, 0);
  lcd.print("Relogio ");
  lcd.setCursor(0, 1);
  if (relHora < 10) { //para ficar no formato de hora 00:00:00
    lcd.print(0);
  }
  lcd.print(relHora);
  lcd.print(":");
  if (relMinuto < 10) { //para ficar no formato de hora 00:00:00
    lcd.print(0);
  }
  lcd.print(relMinuto);
  lcd.print(":");
  if (relSegundo < 10) { //para ficar no formato de hora 00:00:00
    lcd.print(0);
  }
  lcd.print(relSegundo);
  lcd.print(" ");
}

//atualiza as variáveis do relogio
void atualizarRelogio() {
  unsigned long currentMillis = millis(); //recebe o tempo atual do clock interno do arduino
  if (currentMillis - previousMillis >= 1000) { //se já tem mais de 1 segundo, atualizar
    relSegundo += (currentMillis - previousMillis) / 1000; //incrementa segundos pela diferença de tempo desde que foi atualizado pela última vez
    previousMillis = currentMillis;
    if (relSegundo >= 60) {
      relSegundo -= 60;
      relMinuto++;
      if (relMinuto >= 60) {
        relMinuto -= 60;
        relHora++;
        if (relHora >= 24) {
          relHora -= 24;
        }
      }
    }
  }
}

//tocar alarme
void tocarAlarme() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALARME! ");
  lcd.setCursor(0, 1);
  lcd.print("MODO para sair ");
  while (digitalRead(Botao4) == LOW) {
    delay(tempoDelay);
    if (digitalRead(Botao4) == LOW) {
      if (digitalRead(Buzzer) == LOW) {
        digitalWrite(Buzzer, HIGH);
      }
      else {
        digitalWrite(Buzzer, LOW);
      }
    }
  }
  digitalWrite(Buzzer, LOW);
  lcd.clear();
}

//acionado quando apertar o botão Modo
void selecionarModo() {
  lcd.begin(16, 2);
  while (digitalRead(Botao3) == LOW) {
    delay(tempoDelay);
    if (digitalRead(Botao3) == LOW) { //verificar se a tecla não foi acionada por acidente
      if (digitalRead(Botao4) == HIGH) {
        modo++;
        if (modo == 4) {
          modo = 0;
        }
      }
      lcd.setCursor(0, 0);
      lcd.print("Selecione o modo ");
      lcd.setCursor(0, 1);
      switch (modo) {
        case 0:
          lcd.print("Relogio        ");
          break;
        case 1:
          lcd.print("Mudar a hora   ");
          break;
        case 2:
          lcd.print("Cronometro     ");
          break;
        case 3:
          lcd.print("Despertador    ");
          break;
      }
    }
  }
  lcd.clear();
}
