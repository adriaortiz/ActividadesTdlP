//#include "Board.h"
//#include "..\inc\Board.h"
#include "..\..\dep\inc\Board.h"

//Constructores de Board
#pragma region CONSTRUCTORS
Board::Board() 
{
	readBoardSize(); //Se leen las dimensiones del tablero y de los bloques seg�n lo escrito en el documento .txt
	paddle = new Paddle(rows, columns ); //Posicionamos y Escalamos la pala seg�n las dimensiones del tablero
	ball = new Ball(paddle->position[X] - 1,columns / 2); //Posicionamos la pelota seg�n las dimensiones
	initBoard(); //Llamamos la funci�n que crea el tablero seg�n lo especificado
	currentBlocks = blockRows * (columns - 2); //Inicializamos un parametro para controlorar el número de bloques

	scoreQueue = new std::queue<int>();

	initScore(blockRows*(columns - 2));
	lives = 3;
};

Board::~Board()
{
	delete []board;
	delete ball;
	delete paddle;
	delete []scoreQueue;
};

#pragma endregion


//Configuraci�n del Tablero
#pragma region CONFIG
void Board::readBoardSize() //Funci�n que lee el documento .txt
{
	std::ifstream config;
	std::string num;
	const int ROWS_READ = 0;
	const int COLUMNS_READ = 1;
	const int BLOCK_ROWS_READ = 2;
	const int MIN_SC_POINTS = 3;
	const int MAX_SC_POINTS = 4;
	
	config.open(pathConfig);
	if (config.is_open())
	{
		int i = 0;
		// recorremos el contenido del txt separandolo por puntoComa
		for (std::string value; std::getline(config, value, ';'); )
		{
			switch (i)
			{
			case ROWS_READ:
				if (std::stoi(value) < 9)
					rows = 9;
				else
				rows = std::stoi(value);
				break;
			case COLUMNS_READ:
				if (std::stoi(value) < 7)
					columns = 7;
				else
				columns = std::stoi(value);
				break;
			case BLOCK_ROWS_READ:
				if (std::stoi(value) >= rows / 2)
					blockRows = (rows / 2) - 1;
				else if (std::stoi(value) < 1)
					blockRows = 1;
				else
					blockRows = std::stoi(value);
				break;
			case MIN_SC_POINTS:
				minScore = std::stoi(value);
				break;
			case MAX_SC_POINTS:
				maxScore = std::stoi(value);
				break;
			}
			std::cout << value << std::endl;
			i++;
		}
		config.close();
	}
}
#pragma endregion

/*
//Inicializador del Tablero
*/
#pragma region BOARD INITILIZER
void Board::initScore(const int &queueRange)
{
	for (int i = 0; i < queueRange; i++)
	{
		scoreQueue->push((rand() % (maxScore - minScore)) + minScore);
	}
}

void Board::initBoard()
{
	// guardem l'array dinamic dins el HEAP
	board = new Board::Cell*[rows];

	for (int i = 0; i < rows; i++)
	{
		board[i] = new Board::Cell[columns];
	}

	// Guardem el caracter que contenen les celles
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++) // limites Horizontales
		{
			if (i == 0 || i == rows - 1)
			{
				board[i][j].cellChar = H_LIMIT;
				board[i][j].color = GREEN;
			}
			else if (j == 0 || j == columns - 1) // limites verticales
			{
				board[i][j].cellChar = V_LIMIT;
				board[i][j].color = GREEN;
			}
			else if (i == ball->position[X] && j == ball->position[Y]) // pelota
			{
				board[i][j].cellChar = BALL;
				board[i][j].color = RED;
			}
			// pala
			else if (i == paddle->position[X] && (j < paddle->position[Y] + paddle->large && j > paddle->position[Y] - paddle->large)) {
				board[i][j].cellChar = PADDLE;
				board[i][j].color = YELLOW;
			}
			// bloques
			else if (i <= blockRows)
			{
				board[i][j].cellChar = BLOCK;
				board[i][j].color = PINK;
			}
			// null
			else
			{
				board[i][j].cellChar = NULL_BLOCK;
				board[i][j].color = 0;
			}
				
		}
	}
}
#pragma endregion

/*
//Update del Tablero
*/
#pragma region PRINT & REFRESH
void Board::drawBoard()
{
	// imprimi cada element amb el seu color corresponent
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, CYAN);
	drawInfo();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			SetConsoleTextAttribute(h, board[i][j].color);
			std::cout << board[i][j].cellChar;
		}
		std::cout << std::endl;
	}
}

/*
//Printamos el score
*/
void Board::drawInfo()
{
	std::cout << "SCORE--> " << score << std::endl << std::endl;
	std::cout << "LIVES--> " << lives << std::endl << std::endl;
}


/*
//Refresco del tablero
*/
void Board::refreshBoard()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (i == 0 || i == rows - 1 || j == 0 || j == columns - 1) //Dado que los margenes quedar�n igual no se vuelven a pintar, se ignoran
			{
				continue;
			}
			//pilota
			else if (i == ball->position[X] && j == ball->position[Y])
			{
				board[i][j].cellChar = BALL;
				board[i][j].color = RED;
			}
			//pala
			else if (i == paddle->position[X] && (j == paddle->position[Y] || j - 1== paddle->position[Y] || j - 2== paddle->position[Y] || j + 1== paddle->position[Y] || j + 2 == paddle->position[Y] ||
				((paddle->position[Y] == 1 && (j == columns - 2 || j == columns -3)) || (paddle->position[Y] == 2 && j == columns - 2) || (paddle->position[Y] == columns- 2 && (j == 1 || j == 2)) || (paddle->position[Y] == columns - 3 && j == 1 ))))
			{
				board[i][j].cellChar = PADDLE;
				board[i][j].color = YELLOW;
			}
			// blocks
			else if (i <= blockRows)
			{
					board[i][j].cellChar = BLOCK;
					board[i][j].color = PINK;
			}
			//null
			else
			{
				board[i][j].cellChar = NULL_BLOCK;
				board[i][j].color = 0;
			}
		}
	}
}

/*
//Borrado de la pala y la pelota
*/
void Board::clearElement()
{
	board[ball->position[X]][ball->position[Y]].cellChar = NULL_BLOCK; //Borramos el char que representa el bloque

	for (int i = 0; i < columns; i++)
	{
		if (i != 0 && i != columns-1)
			board[paddle->position[X]][i].cellChar = NULL_BLOCK;
	}
}

/*
//Refresco de la pala y la pelota
*/
void Board::refreshElement() 
{
	board[ball->position[X]][ball->position[Y]].cellChar = BALL;
	board[ball->position[X]][ball->position[Y]].color = RED;

	if (paddle->large <= 1)
	{
		board[paddle->position[X]][paddle->position[Y]].cellChar = PADDLE;
		board[paddle->position[X]][paddle->position[Y]].color = YELLOW;
	}
	else
	{
		int halfPaddleLarge = std::floor((paddle->large - 1) *0.5f);
		for (int i = -halfPaddleLarge; i <= halfPaddleLarge; i++)
		{
			board[paddle->position[X]][paddle->position[Y] + i < 1 ? (columns - 1) - (1 - (paddle->position[Y] + i)) : paddle->position[Y] + i >= columns - 1 ? ((paddle->position[Y] + i) - (columns - 2)) : paddle->position[Y] + i].cellChar = PADDLE;
			board[paddle->position[X]][paddle->position[Y] + i < 1 ? (columns - 1) - (1 - (paddle->position[Y] + i)) : paddle->position[Y] + i >= columns - 1 ? ((paddle->position[Y] + i) - (columns - 2)) : paddle->position[Y] + i].color = YELLOW;
		}
	}
}
#pragma endregion

/*
//Condición de victoria
*/
bool Board::boardEmpty()
{
	return currentBlocks == 0;
}

//Movimiento Pala y Pelota
#pragma region ElementMovement
Board::Ball::~Ball()
{
}

/*
//Movimiento de la pelota, aqui se cambia su posici�n seg�n sus colisiones
*/
bool Board::Ball::ballMovement(Vec2 boardDimentions,int blockRows, Vec2 paddlePos, Board &myBoard) 
{
	

	int blockCollision = 0; //Número de colisiones

	//comprobamos si el proximo frame colisionamos con un bloque
	// vertical Recto
	if (myBoard.board[position[X]-1][position[Y]].cellChar == BLOCK && speed[X] < 0)
	{
		blockCollision++;
		myBoard.board[position[X]-1][position[Y]].cellChar = NULL_BLOCK;
		speed[X] = speed[X] * -1;
	}
	else if (myBoard.board[position[X] + 1][position[Y]].cellChar == BLOCK && speed[X] > 0)
	{
		blockCollision++;
		myBoard.board[position[X] + 1][position[Y]].cellChar = NULL_BLOCK;
		speed[X] = speed[X] * -1;
	}
	
	//HorizontalRecto
	if (myBoard.board[position[X]][position[Y]-1].cellChar == BLOCK && speed[Y] < 0)
	{
		blockCollision++;
		myBoard.board[position[X]][position[Y]-1].cellChar = NULL_BLOCK;
		speed[Y] = speed[Y] * -1;
	}
	else if (myBoard.board[position[X]][position[Y]+1].cellChar == BLOCK && speed[Y] > 0)
	{
		blockCollision++;
		myBoard.board[position[X]][position[Y] +1].cellChar = NULL_BLOCK;
		speed[Y] = speed[Y] * -1;
	}

	// Diagonals Superiores
	if (myBoard.board[position[X] - 1][position[Y] - 1].cellChar == BLOCK && speed[X] < 0 && speed[Y] < 0)
	{
		blockCollision++;
		myBoard.board[position[X] - 1][position[Y] - 1].cellChar = NULL_BLOCK;
		speed[X] *= -1; speed[Y] *= -1;
	}
	else if (myBoard.board[position[X] - 1][position[Y] + 1].cellChar == BLOCK && speed[X] < 0 && speed[Y] > 0)
	{
		blockCollision++;
		myBoard.board[position[X] - 1][position[Y] + 1].cellChar = NULL_BLOCK;
		speed[X] *= -1; speed[Y] *= -1;
	}
	//Diagonales Inferiores
	else if (myBoard.board[position[X] + 1][position[Y] - 1].cellChar == BLOCK && speed[X] > 0 && speed[Y] < 0)
	{
		blockCollision++;
		myBoard.board[position[X] + 1][position[Y] - 1].cellChar = NULL_BLOCK;
		speed[X] *= -1; speed[Y] *= -1;
	}
	else if (myBoard.board[position[X] + 1][position[Y] + 1].cellChar == BLOCK && speed[X] > 0 && speed[Y] > 0)
	{
		blockCollision++;
		myBoard.board[position[X] + 1][position[Y] + 1].cellChar = NULL_BLOCK;
		speed[X] *= -1; speed[Y] *= -1;
	}

	while (blockCollision > 0) //Sumamos score según el número de colisiones con los bloques
	{
		myBoard.currentBlocks--;
		blockCollision--;
		myBoard.score += myBoard.scoreQueue->front();
		myBoard.scoreQueue->pop();
	}
	/*
	// comprobamos si en el proximo frame podemos golpear a la pala
	// el jugador puede decidir la direccion segun en que posicion impacte la bola
	*/
	if (position[X] + speed[X] == paddlePos[X])
	{
		// posicion central
		if (position[Y] == paddlePos[Y])
		{
			speed[X] = speed[X] * -1;
			speed[Y] = 0;
		}
		// izquierda pala
		else if (position[Y] == paddlePos[Y] - 1 || (position[Y] == paddlePos[Y] - 2 && speed[Y] > 0))
		{
			speed[Y] = -1;
			speed[X] = -1;
		}
		// derecha pala
		else if (position[Y] == paddlePos[Y] + 1 || (position[Y] == paddlePos[Y] + 2 && speed[Y] < 0))
		{
			speed[Y] = 1;
			speed[X] = -1;
		}
		// si la pala se encuentra en el limite Derecho
		else if (paddlePos[Y] == boardDimentions[Y] - 2)
		{
			if (position[Y] == 1 || ( position[Y] == 2  && speed[Y] < 0))
			{
				speed[Y] = -1;
				speed[X] = speed[X] * -1;
			}
		}
		// si la pala se encuentra en el limite izquierdo
		else if (paddlePos[Y] == 1)
		{
			if (position[Y] == boardDimentions[Y] -2 || (position[Y] == boardDimentions[Y] - 3 && speed[Y] > 0))
			{
				speed[Y] = -1;
				speed[X] = speed[X] * -1;
			}
		}

	}


	//comprobamos si en el proximo frame impactamos con el muro
	if ((position[Y] == 1 && speed[Y] < 0) || (position[Y] == boardDimentions[Y] - 2 && speed[Y] > 0))
		speed[Y] = speed[Y] * -1;
	
	if (position[X] == 1 && speed[X] < 0)
		speed[X] = speed[X] * -1;
	else if (position[X] == boardDimentions[X] - 2 && speed[X] > 0)
	{
		position[X] = blockRows + 1;
		position[Y] = boardDimentions[Y] / 2;
		speed[Y] = (rand()) % 10 < 5 ? -1 : 1;
		return true;
	}


	// actualizamos la posicion de la pala incrementano por su velocidad
	if (myBoard.board[position[X] + speed[X]][position[Y] + speed[Y]].cellChar != BLOCK) //Condición debido a que a veces se comia dos bloques seguidos
	{
		position[X] += speed[X];
		position[Y] += speed[Y];
	}

	return false;
}

Board::Paddle::~Paddle(){}

/*
//Movimiento de la pala seg�n los inputs recibidos
*/
void Board::Paddle::paddleMovement(inputType currentInput, Ball *ball, int columns)
{
	// Input derecha
	if (currentInput == (inputType::RIGHT))
	{
		if (position[Y] == columns - 2)
			position[Y] = 1;
		else
			position[Y] += 1;
	}
	//Input Izquierda
	else if (currentInput == (inputType::LEFT))
	{
		if (position[Y] == 1)
			position[Y] = columns - 2;
		else
			position[Y] -= 1;
	}
	// Espacio
	else if (currentInput == (inputType::SPACE) && ball->speed == Vec2(0,0))
	{
		ball->speed[X] = -1;
		ball->speed[Y] = (rand()) % 10 < 5 ? -1 : 1;
	}

	// Si la pelota se encuentra en reposo (solo inicio)
	if (ball->speed == Vec2(0, 0))
		ball->position[Y] = position[Y];

}
#pragma endregion 

//Getters de Board
#pragma region GETTERS
Vec2 Board::getDimensions() //Get que retorna las dimensiones del tablero
{
	return Vec2(rows, columns);
}

int Board::getRows() //Get que retorna el n�mero de filas
{
	return rows;
}
int Board::getBlockRows()
{
	return blockRows;
}
int Board::getColumns() //Get que retorna el n�mero de columnas
{
	return columns;
}
int Board::getScore()
{
	return score;
}
int Board::getLives()
{
	return lives;
}
void Board::minLives(int value)
{
	lives-= value;
}
int Board::blocksXcent()
{
	return (currentBlocks * 100) / (blockRows * (columns - 2));
}
#pragma endregion

Board::Cell::~Cell()
{
}
