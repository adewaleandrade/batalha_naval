#include "LedControl.h"
LedControl lc = LedControl(12, 10, 11, 1); // Pins: DIN,CLK,CS, # of Display connected
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

//Limite de Navios
#define MAXHIDROAVIOES 2
#define MAXSUBMARINES 2
#define MAXCRUZADORES 1
#define MAXENCOURACADOS 1
#define MAXPORTAAVIOES 1

//Tipos de Navio
#define HIDROAVIAO 0
#define SUBMARINO 1
#define CRUZADOR 2
#define ENCOURACADO 3
#define PORTAAVIAO 4

unsigned long delaytime = 100;

int cursorCol = 0;
int cursorRow = 0;

int shipStartRow = 0;
int shipEndRow = 0;
int shipStartCol = 0;
int shipEndCol = 0;

int ships[5][2] ={
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0}
};

int turnPlayer = 1;

bool isMapSet = false;
bool isGameOver = false;

int maps[2][8][8] = {
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

//Linha completa (para a pintura)
int fr[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void setup() {
  Serial.begin(9600);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 5);
  /* and clear the display */
  lc.clearDisplay(0);
  // put your setup code here, to run once:
//  mapSetup(p1Map);
  //mapSetup(p2Map);

  //Buttons and Buzzer Setup
  pinMode(btnV, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low
  pinMode(btnH, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low
  pinMode(btnC, INPUT);
  digitalWrite(btnV, LOW); // Garantir que o pino começa no estado Low

  pinMode(buzzer, OUTPUT);

	// lc.setRow(0, i, pText[i]);
   printRow(0,cursorRow,8,fr);
   printColumn(0,cursorCol,8,fr);
}


void loop() {

Serial.print("confirm btn: ");
  Serial.println(digitalRead(btnC));
  while (! isMapSet) {
  	// showCursor(0);
    setPlayerMap();
  }

  // showPlayerTurn(turnPlayer);

  /*playerTurn();
  changePlayerTurn();

  if ( isGameOver ) {
    showGameOverScreen();
  }*/

  //  Serial.print("Button State: ");
  //  Serial.println(buttonState);

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

  turnPlayer++;// Passa para a configuração do próximo jogador;
}

// Define a posição dos navios
void setShip(int player) {
	Serial.println("setShip");
  bool done = false;
  bool shipStart = false;
  bool shipEnd = false;

  updateDisplay(player, 0, true);
  while (! done) {
    blinkSelectedPoints(player - 1, shipStart);
    moveCursor(player, 0);
    
    if (digitalRead(btnC) == HIGH) {
    	Serial.print("confirm btn: ");
      Serial.println(digitalRead(btnC));
      digitalWrite(btnC, LOW); delay(500); //delay para evitar que o botão seja lido mais de uma vez;
      
    	if( ! shipStart){ // Configurar ponto de inicio do Navio
    		shipStartRow = cursorRow;
  			shipStartCol = cursorCol;
  			shipStart = true;
        Serial.println("Start Point Set");
    	}else if(! shipEnd){// Configurar ponto de fim do Navio
    		shipEndRow = cursorRow;
			shipEndCol = cursorCol;
      Serial.println("END Point Set");
			int size = getShipSize(shipStartRow, shipStartCol, shipEndRow, shipEndCol);
      Serial.print("ShipSize ");
      Serial.println(size);
			bool validShip = false;
			switch (size){ // Verifica se o tipo de navio selecionado comporta um novo navio;
				// Converter esse swtich numa função que recebe o player e o tamnaho da navio e faz as verificações.; @Lucas
				case 1: // Macro Tamanho do Navio (HIDROAVIOES)
					validShip = checkHidroavioes(player);//
					break;
				case 2: 
					validShip = checkSubmarines(player);
					break;
				case 3:
					validShip = checkCruzadores(player);
					break;
				case 4:
					validShip = checkEncouracados(player);
					break;
				case 5:
					validShip = checkPortaAvioes(player);
					break;
				default:
					shipStart = false;
 				 	// emitir sinal com o buzzer;
 				 	break;
			}
      Serial.print("isShipValid: ");
      Serial.println(validShip);
			if(validShip){ // Verifica se o navio não sobrepõe outros navios
				//ordenar os pontos
				if(shipStartRow == shipEndRow){
					if (shipStartCol >= shipEndCol){
						int i = shipStartCol;
						shipStartCol = shipEndCol;
						shipEndCol = i;
					}
				}else if(shipStartCol == shipEndCol){
					if (shipStartRow >= shipEndRow){
						int i = shipStartRow;
						shipStartRow = shipEndRow;
						shipEndRow = i;
					}
				}

				if(isShipPosValid(player, shipStartRow, shipStartCol, shipEndRow, shipEndCol)){
          Serial.println("isShipPosValid: True");
					//Salvar no mapa do jogador e atualizar contadores
					//Salva no mapa
					for (int i = shipStartRow; i <= shipEndRow; i++) {
					  for (int j = shipStartCol; j <= shipEndCol; j++) {
					    maps[player - 1][i][j] = 1;
					  }
					}

					//Atualiza tabela de navios
					ships[size][player-1] = ships[size][player-1] + 1;
					done = true;
				}else{ // Navio sobrepondo outro navio
          Serial.println("isShipPosValid: False");
					shipStart = false;
					//emitir sinal para usuário buzzer
				}
			}else{ //Atingiu o numero maximo de naios dessa classe;
				shipStart = false;
				// emitir sinal de erro com o buzzer
				// dar algum sinal pro usuário;
			}
    	}
    }
  }
}

// Faz o ponto inicial do navio piscar;
void blinkSelectedPoints(int matrix, bool show){
  if(show){
    lc.setLed(matrix, shipStartRow, shipStartCol, 1);
    delay(100);
    lc.setLed(matrix, shipStartRow, shipStartCol, 0);
    delay(100);
  }
}

int getShipSize(int shipSR, int shipSC, int shipER, int shipEC ){
	Serial.println("getShipSize");
	int size = 6;
	if(shipSR == shipER){
		size = shipSC - shipEC;
	}else if(shipSC == shipEC){
		size = shipSR - shipER;
	}

	return (size < 0) ? (size * (-1)) + 1 : size + 1;
}

//Verifica se a posição de um dado navio é válido, ou seja, um navio não sobrepõe outro.
bool isShipPosValid(int player, int sX, int sY, int eX, int eY) {
	Serial.println("isShipPosValid");
	if((eX < sX) || (eY < sY)){ //Se o navio não está da esquerda para direita e nem de cima para baixo, retorna falso
		return false;
	}

	for(int i = sX; i <= eX; i++){
		for (int j = sY; j <= eY; j++){
			if (maps[player][i][j] == 1){
				return false;
			}
		}
	}

	return true;
}

// Verifica se a quantidade de navios do jogador ja atingiu o limite
bool checkShipsCount(int player){
	Serial.println("checkShipsCount");
	if( checkHidroavioes(player) && checkSubmarines(player) && checkCruzadores(player) &&
			checkEncouracados(player) && checkPortaAvioes(player)){
		return true;
	}

	return false;
}

bool checkHidroavioes(int player){
	if(ships[HIDROAVIAO][player-1] < MAXHIDROAVIOES){
		return false;
	}
	return true;
}

bool checkSubmarines(int player){
	if(ships[SUBMARINO][player-1] < MAXSUBMARINES){
		return false;
	}
	return true;
}

bool checkCruzadores(int player){
	if(ships[CRUZADOR][player-1] < MAXCRUZADORES){
		return false;
	}
	return true;
}

bool checkEncouracados(int player){
	if(ships[ENCOURACADO][player-1] < MAXENCOURACADOS){
		return false;
	}
	return true;
}

bool checkPortaAvioes(int player){
	if(ships[PORTAAVIAO][player-1] < MAXPORTAAVIOES){
		return false;
	}
	return true;
}

//print on the matrix the player's map
void showPlayerMap(int player) {
  clearScreen();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, maps[player - 1][i][j]);
    }
  }
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

  clearScreen();

  for (int i = 0 ; i < 8; i++) {
    lc.setRow(0, i, pText[i]);
  }
}


void updateDisplay(int player, int lMatrix, bool cursor) {
  clearScreen();
  showPlayerMap(player);
  if (cursor) {
    showCursor(lMatrix);
  }
}

// Limpa a matriz de leds
void clearScreen() {
  for (int i = 0 ; i < 8; i++) {
    lc.setRow(0, i, B00000000);
  }
  // delay(500);
}

// Espera um comando do jogador e atualiza a posição do cursor
void moveCursor(int player, int matrix) {
	// Serial.println("moveCursor");
  if (digitalRead(btnV) == HIGH) {
    cursorRow = (cursorRow < 7) ? cursorRow+1 : 0;
    updateDisplay(player, matrix, true);
  }

  if (digitalRead(btnH) == HIGH) {
    cursorCol = (cursorCol < 7) ? cursorCol+1 : 0;
    updateDisplay(player, matrix, true);
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