#include "LedControl.h"
//LedControl lc = LedControl(5,6,7,1);  // Pins: DIN,CLK,CS, # of Display connected

#define  CLEARLINE  B00000000
#define  FULLLINE  B11111111
#define  MAXPLAYERS 2

// Vetical move Button
#define  btnV 2
//Horizontal move Button
#define  btnH 3
//Confirmation Button
#define  btnC 4
// buzzer
#define buzzer 7

//Tipos de Navio
#define HIDROAVIAO 0
#define SUBMARINO 1
#define CRUZADOR 2
#define ENCOURACADO 3
#define PORTAAVIAO 4

#define MAXSHIPSIZE 5

LedControl lc = LedControl(12, 10, 11, MAXPLAYERS ); // Pins: DIN,CLK,CS, # of Display connected

int DataPin = 12;  //Ligar o pino 4 do Arduino ao DIN do modulo
int ClockPin = 10; //Ligar o pino 5 do Arduino ao CLK do módulo
int LoadPin = 11;  //Ligar o pino 6 do Arduinio ao pino CS/Load do módulo

unsigned long delaytime = 100;

int cursorCol = 0;
int cursorRow = 0;

int shipStartRow = 0;
int shipEndRow = 0;
int shipStartCol = 0;
int shipEndCol = 0;


int maxShipCounts[6] = {
  0,
  2, //MAXHIDROAVIOES
  2, //MAXSUBMARINES
  1, //MAXCRUZADORES
  1, //MAXENCOURACADOS
  1  //MAXPORTAAVIOES
};

// int globalShips[6][2] = {
//   {0,0},
//  {0,0},
//  {0,0},
//  {0,0},
//  {0,0},
//   {0,0}
// };

int globalShips[6][2] = { // Versão preenchida para testes
  {0, 0},
  {2, 2},
  {2, 2},
  {1, 1},
  {1, 1},
  {1, 1}
};

int turnPlayer = 1;

bool isMapSet = false;

// int maps[2][8][8] = {
//   {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   },
//   {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   }
// };

// int maps[2][8][8] = { // Mapa para testes de rodadas
//   {
//     {1, 0, 0, 1, 1, 1, 1, 1},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 0, 1, 1, 1, 1, 0},
//     {0, 0, 1, 1, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 1, 1, 0, 0},
//     {0, 0, 0, 1, 1, 1, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   },
//   {
//     {0, 0, 1, 0, 0, 0, 0, 0},
//     {0, 1, 0, 0, 0, 0, 0, 0},
//     {1, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 1, 1, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 0, 0, 0, 0},
//     {0, 0, 1, 0, 0, 0, 0, 0}
//   }
// };

int maps[2][8][8] = {
  {
    {1, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

int playerAttacks[2][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

byte dispP1[] = { // Escreve P1
  B00000000,
  B00000000,
  B01100100,
  B01011100,
  B01100100,
  B01001110,
  B00000000,
  B00000000
};

byte dispP2[] = {// Escreve P2
  B00000000,
  B00000000,
  B01101110,
  B01011010,
  B01100100,
  B01001110,
  B00000000,
  B00000000
};

byte gameOver[] = {// Escreve P2
  B00000000,
  B00000000,
  B00000000,
  B00011000,
  B00011000,
  B00000000,
  B00000000,
  B00000000
};

int hit[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int miss[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// int letterH[8][8] = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   };

// int letterI[8][8] = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   };
// int letterT[8][8] = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 1, 1, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   };
// int letterM[8][8] = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 0, 0, 0, 0, 1, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 1, 1, 1, 1, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 1, 1, 0, 0, 1, 1, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   };
// int letterS[8][8] = {
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 1, 1, 1, 1, 1, 0},
//     {0, 1, 0, 0, 0, 0, 0, 0},
//     {0, 0, 1, 1, 1, 1, 0, 0},
//     {0, 0, 0, 0, 0, 0, 1, 0},
//     {0, 0, 1, 1, 1, 1,0 , 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
//   };

//Linha completa (para a pintura)
int fr[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void setup() {
  Serial.begin(9600);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  for (int i = 0; i < MAXPLAYERS; i++) {
    lc.shutdown(i, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i, 5);
    /* and clear the display */
    lc.clearDisplay(i);
  }

  //Buttons and Buzzer Setup
  pinMode(btnV, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low
  pinMode(btnH, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low
  pinMode(btnC, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low

  pinMode(buzzer, OUTPUT);

  // lc.setRow(0, i, pText[i]);
  // printRow(0,cursorRow,8,fr);
  // printColumn(0,cursorCol,8,fr);
}

void loop() {

  Serial.print("confirm btn: ");
  Serial.println(digitalRead(btnC));
  while (! isMapSet) {
    setPlayerMap();
  }

  showPlayerTurn(turnPlayer);

  for (int i = 1; i <= MAXPLAYERS; i++) {
    updateDisplay(i, true, false);
  }

  playerTurn(turnPlayer);
  updateMaps();
  changePlayerTurn();

  int winner = isGameOver();

  if (winner) {
    Serial.println("The Game is Over!");
    Serial.print("The Winner is: P");
    Serial.println(winner);
    // pisca 3 vezes
    showGameOverScreen();
    showGameOverScreen();
    showGameOverScreen();
    resetGame();
  }
}

// Verifica se o jogo terminou. Caso tenha terminado, retorna o jogador vencedor. Caso negativo, retorna 0;
int isGameOver() {
  Serial.println("isGameOver");

  if (isPlayerMapEmpty(0)) return 2;
  if (isPlayerMapEmpty(1)) return 1;

  return 0;
}

// Verifica se todos os navios de um jogador ja foram afundados
bool isPlayerMapEmpty(int player) {
  Serial.println("isPlayerMapEmpty");

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Serial.print("player Map: P"); Serial.println(player + 1);
      Serial.print("maps[k]["); Serial.print(i); Serial.print("]["); Serial.print(j); Serial.print("] = "); Serial.println(maps[player][i][j]);
      if (maps[player][i][j] != 0) {
        return false;
      }
    }
  }
  return true;
}

void showGameOverScreen() {
  for (int i = 0; i < MAXPLAYERS; ++i) {
    lc.clearDisplay(i);
    delay(250);

    for (int j = 0 ; j < 8; j++) {
      lc.setRow(i, j, gameOver[j]);
    }
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
  }
}

void resetGame() {
  // reset Maps
  for (int p = 0; p < MAXPLAYERS; p++) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        playerAttacks[p][i][j] = 0;
        maps[p][i][j] = 0;
      }
    }
  }

  // reset GlobalShips
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 2; ++j) {
      globalShips[i][j] = 0;
    }
  }

  turnPlayer = 1;
  isMapSet = false;
}


void playerTurn(int player) {
  Serial.print("player turn: P"); Serial.println(player);
  bool done = false;

  updateDisplay(player, false, true);
  while (! done) {
    moveCursor(player, false);
    if (digitalRead(btnC) == HIGH) {
      Serial.print("confirm btn: ");
      Serial.println(digitalRead(btnC));
      digitalWrite(btnC, LOW); delay(500); //delay para evitar que o botão seja lido mais de uma vez;

      // Salva o ataque
      playerAttacks[player - 1][cursorRow][cursorCol] = 1;

      if (isHitSuccessfull(player - 1, cursorRow, cursorCol)) {
        showAcertou(player - 1);
      } else {
        showErrou(player - 1);
      }
      done = true;
    }
  }
}

// Passa a vez para o próximo jogador
void changePlayerTurn() {
  Serial.println("changePlayerTurn");
  if (turnPlayer == MAXPLAYERS) {
    turnPlayer = 1;
  } else {
    turnPlayer++;
  }
  resetCursors();
}

void updateMaps() {
  Serial.println("updateMaps");
  for (int p = 0; p < MAXPLAYERS; p++) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        int target = 1;
        if (p == MAXPLAYERS - 1) {
          target =  0;
        }

        if (playerAttacks[p][i][j] == 1) {
          maps[target][i][j] = 0;
        }
      }
    }
  }
}

void resetCursors() {
  Serial.println("resetCursors");
  cursorCol = 0;
  cursorRow = 0;
}

// Função que Administra a etapa de configuração dos mapas dos jogadores
void setPlayerMap() {
  Serial.println("setPlayerMap");
  if ( turnPlayer > MAXPLAYERS ) {
    isMapSet = true;
    turnPlayer = 1;
    return;
  }

  showPlayerTurn(turnPlayer);
  while ( ! checkShipsCount(turnPlayer) ) {
    setShip(turnPlayer);
  }
  turnPlayer++;
}

// Define a posição dos navios
void setShip(int player) {
  Serial.println("setShip");
  bool done = false;
  bool shipStart = false;
  bool shipEnd = false;

  updateDisplay(player, true, true);
  while (! done) {
    blinkSelectedPoints(player - 1, shipStart);
    moveCursor(player, true);

    if (digitalRead(btnC) == HIGH) {
      Serial.print("confirm btn: ");
      Serial.println(digitalRead(btnC));
      digitalWrite(btnC, LOW); delay(500); //delay para evitar que o botão seja lido mais de uma vez;

      if ( ! shipStart) { // Configurar ponto de inicio do Navio
        shipStartRow = cursorRow;
        shipStartCol = cursorCol;
        shipStart = true;
        Serial.println("Start Point Set");
      } else if (! shipEnd) { // Configurar ponto de fim do Navio
        shipEndRow = cursorRow;
        shipEndCol = cursorCol;
        Serial.println("END Point Set");
        int size = getShipSize(shipStartRow, shipStartCol, shipEndRow, shipEndCol);
        Serial.print("ShipSize ");
        Serial.println(size);
        Serial.print("ShipClassCount:  ");
        Serial.println(globalShips[size][player - 1]);
        bool maxShipClassCount = true;

        if (size <= MAXSHIPSIZE) {
          if (globalShips[size][player - 1] < maxShipCounts[size]) {
            maxShipClassCount = false;
          }
        }

        Serial.print("isShipCoutFull: ");
        Serial.println(maxShipClassCount);
        if ( ! maxShipClassCount) { // Verifica se o navio não sobrepõe outros navios
          //ordenar os pontos
          if (shipStartRow == shipEndRow) {
            if (shipStartCol >= shipEndCol) {
              int i = shipStartCol;
              shipStartCol = shipEndCol;
              shipEndCol = i;
            }
          } else if (shipStartCol == shipEndCol) {
            if (shipStartRow >= shipEndRow) {
              int i = shipStartRow;
              shipStartRow = shipEndRow;
              shipEndRow = i;
            }
          }

          if (isShipPosValid(player, shipStartRow, shipStartCol, shipEndRow, shipEndCol, maps)) {
            Serial.println("isShipPosValid: True");
            //Salvar no mapa do jogador e atualizar contadores
            //Salva no mapa
            for (int i = shipStartRow; i <= shipEndRow; i++) {
              for (int j = shipStartCol; j <= shipEndCol; j++) {
                maps[player - 1][i][j] = size;
              }
            }
            buzzerConfirm();
            Serial.print("Quantidade do Navio: ");
            Serial.println(globalShips[size][player - 1]);
            //Atualiza tabela de navios
            globalShips[size][player - 1] = globalShips[size][player - 1] + 1;
            Serial.print("Quantidade do Navio Apos insert: ");
            Serial.println(globalShips[size][player - 1]);
            done = true;
          } else { // Navio sobrepondo outro navio
            Serial.println("isShipPosValid: False");
            shipStart = false;
            //emitir sinal para usuário buzzer
            buzzerAlert();
          }
        } else { //Atingiu o numero maximo de naios dessa classe;
          shipStart = false;
          // emitir sinal de erro com o buzzer
          buzzerAlert();
          // dar algum sinal pro usuário;
        }
      }
    }
  }
}

void buzzerConfirm(){
   digitalWrite(buzzer, HIGH);
   delay(500);
   digitalWrite(buzzer,LOW);
}

void buzzerAlert(){
   digitalWrite(buzzer, HIGH);
   delay(250);
   digitalWrite(buzzer,LOW);
   delay(250);
   digitalWrite(buzzer, HIGH);
   delay(250);
   digitalWrite(buzzer,LOW);
   delay(250);
   digitalWrite(buzzer, HIGH);
   delay(250);
   digitalWrite(buzzer,LOW);
}

// Verifica se um ataque foi bem sucedido. Em caso positivo, atualiza o mapa do alvo removendo o ponto atacado.
bool isHitSuccessfull(int attacker, int row, int col) {
  Serial.println("isHitSuccessfull");

  int target = 0;
  if (attacker < MAXPLAYERS - 1) {
    target = attacker + 1;
  }

  Serial.print("targetPlayer: P");
  Serial.println(target + 1);
  Serial.print("targetPoint: ");
  Serial.println(maps[target][row][col]);
  if (maps[target][row][col] != 0) {
    return true;
  }

  return false;
}

void showAcertou(int attacker) {
  Serial.println("showAcertou");
  for (int t = 0; t < 3; t++) { // blink the message 3 times
    for (int p = 0; p < MAXPLAYERS; p++ ) {
      lc.clearDisplay(p);
      delay(250);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          lc.setLed(p, i, j, hit[i][j]);
        }
      }
    }
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}

void showErrou(int attacker) {
  Serial.println("showErrou");
  for (int t = 0; t < 3; t++) { // blink the message 3 times
    for (int p = 0; p < MAXPLAYERS; p++ ) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          lc.setLed(p, i, j, miss[i][j]);
        }
      }
    }
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}

// Faz o ponto inicial do navio piscar;
void blinkSelectedPoints(int matrix, bool show) {
  if (show) {
    lc.setLed(matrix, shipStartRow, shipStartCol, 1);
    delay(100);
    lc.setLed(matrix, shipStartRow, shipStartCol, 0);
    delay(100);
  }
}

int getShipSize(int shipSR, int shipSC, int shipER, int shipEC ) {
  Serial.println("getShipSize");
  int size = 6;
  if (shipSR == shipER) {
    size = shipSC - shipEC;
  } else if (shipSC == shipEC) {
    size = shipSR - shipER;
  }

  return (size < 0) ? (size * (-1)) + 1 : size + 1;
}

//Verifica se a posição de um dado navio é válido, ou seja, um navio não sobrepõe outro.
bool isShipPosValid(int player, int sX, int sY, int eX, int eY, int maps[2][8][8]) {
  Serial.println("isShipPosValid");
  if ((eX < sX) || (eY < sY)) { //Se o navio não está da esquerda para direita e nem de cima para baixo, retorna falso
    return false;
  }

  for (int i = sX; i <= eX; i++) {
    for (int j = sY; j <= eY; j++) {
      Serial.print("Player: "); Serial.println(player - 1);
      Serial.print("i: "); Serial.println(i);
      Serial.print("j: "); Serial.println(j);
      Serial.print("maps[player][i][j]: "); Serial.println(maps[player - 1][i][j]);

      if (maps[player - 1][i][j] == 1) {

        return false;
      }
    }
  }

  return true;
}

// Verifica se a quantidade de navios do jogador ja atingiu o limite
bool checkShipsCount(int player) {
  Serial.println("checkShipsCount");
  for (int i = 0; i <= MAXSHIPSIZE; i++) {
    Serial.print("checkShipsCount - ");
    Serial.print(i);
    if (globalShips[i][player - 1] < maxShipCounts[i]) {
      Serial.println(": False");
      return false;
    }
  }
  Serial.println(": True;");
  return true;
}

//Mostra qual o jogador atual
void showPlayerTurn(int player) {
  Serial.println("showPlayerTurn");
  // Mostra na matriz o jogador
  lightPlayerTurn(player);
  bool confirmed = false;
  while ( ! confirmed) { // Aguarda a confirmação do Jogador
    if (digitalRead(btnC) == HIGH) {
      confirmed = true;
      digitalWrite(btnC, LOW); delay(500); //delay para evitar que o botão seja lido mais de uma vez;
    }
  }
}

// Mostra na matriz - P1 ou P2
void lightPlayerTurn(int turn) {
  byte* pText = dispP1;
  if ( turn == 2) {
    pText = dispP2;
  }

  for (int i = 0; i < MAXPLAYERS; ++i) {
    lc.clearDisplay(i);

    for (int j = 0 ; j < 8; j++) {
      lc.setRow(i, j, pText[j]);
    }
  }

}

void updateDisplay(int player, bool playerMap, bool cursor) {
  lc.clearDisplay(player - 1);
  showPlayerMap(player, playerMap);

  if (cursor) {
    showCursor(player - 1);
  }
}

//print on the matrix the player's map
void showPlayerMap(int player, bool playerMap) {
  lc.clearDisplay(player - 1);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (playerMap) {
        lc.setLed(player - 1, i, j, maps[player - 1][i][j]);
      } else {
        lc.setLed(player - 1, i, j, playerAttacks[player - 1][i][j]);
      }
    }
  }
}

// Espera um comando do jogador e atualiza a posição do cursor
void moveCursor(int player, bool playerMap) {
  // Serial.println("moveCursor");
  if (digitalRead(btnV) == HIGH) {
    cursorRow = (cursorRow < 7) ? cursorRow + 1 : 0;
    updateDisplay(player, playerMap, true);
  }

  if (digitalRead(btnH) == HIGH) {
    cursorCol = (cursorCol < 7) ? cursorCol + 1 : 0;
    updateDisplay(player, playerMap, true);
  }
}

//Mostra na matriz o cursor do jogador
void showCursor(int lMatrix) {
  printRow(lMatrix, cursorRow, 8, fr);
  printColumn(lMatrix, cursorCol, 8, fr);
}


// Função para pintar uma linha na matriz de Leds
void printRow(int displayN, int rowNumber, int rowSize, int* row) {
  for (int i = 0; i < rowSize; i++) {
    lc.setLed(displayN, rowNumber, i, row[i]);
  }
}

// Função para pintar uma Coluna na matriz de Leds
void printColumn(int displayN, int colNumber, int colSize, int* col) {
  for (int i = 0; i < colSize; i++) {
    lc.setLed(displayN, i, colNumber, col[i]);
  }
}

// Função para a Inicialização do Mapa
void mapSetup(int map[8][8]) {
  for (int i = 0; i < 8 ; i++) {
    for (int j = 0; j < 8 ; j++) {
      map[i][j] = 0;
    }
  }
}