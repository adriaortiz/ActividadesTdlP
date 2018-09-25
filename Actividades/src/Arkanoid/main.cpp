 /*
 / Esta práctica creada al 2018 dentro de la asignatura de Tecnología de la programación del grado de 
 / Contenidos digitales interactivos de ENTI-UB ha sido cedida por los estudiantes David Auladell y 
 / Maurizio Carlotta con el único fin de ayudar a desarrollar las actividades propuestas en el curso 
 / 2018-2019 de la asignatura de Programación en lenguajes interpretados del mismo grado universitario. 
 / Ésta no es una solución definitiva y correcta de dichas actividades.
*/

//#include "Board.h"
#include "..\..\dep\inc\Board.h"
//#include "Menus.h"
#include "..\..\dep\inc\Menus.h"

#pragma region INPUTS
inputType inputReader() 
{
	//Hay que comprobar solo el mayor bit para saber si la key está down //Sacado de https://msdn.microsoft.com/en-us/library/windows/desktop/ms646293%28v=vs.85%29.aspx en "Return Value"
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0) {//Sacado de https://stackoverflow.com/questions/8468514/getasynckeystate-creating-problems-with-cin
		return inputType::RIGHT;
	}
	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0) {
		return inputType::LEFT;
	}
	else if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) {
		return inputType::EXIT;
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0) {
		return inputType::SPACE;
	}
	else if ((GetAsyncKeyState(VK_NUMPAD0) & 0x8000) != 0) {
		return inputType::ZERO;
	}
	else if ((GetAsyncKeyState(VK_NUMPAD1) & 0x8000) != 0) {
		return inputType::ONE;
	}
	else if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000) != 0) {
		return inputType::TWO;
	}
	else if (GetAsyncKeyState(VK_NUMPAD3)) {
		return inputType::THREE;
	}
	else {
		return inputType::MAX;
	}
}

/*
 /Función que esconde el cursor para hacer el refresco del tablero más limpio
*/
void clearCursor()
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursor;

	cursor.dwSize = 10;
	cursor.bVisible = false;
	SetConsoleCursorInfo(handle, &cursor);
}

#pragma endregion


int main()
{
	srand(time(NULL));
	clearCursor(); //Escondemos el cursor

	gameState currentState = gameState::MENU; // variable para controlar el estado de juego
	bool inGame = true; // variable que nos permite cerrar el juego
	Board *board = new Board(); //Creamos un puntero que apunte a una nueva "Board"
	std::map<std::string, int> ranking = Ranking::get();// leemos el ranking i lo guardamos en un map


	do {
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//Sacado de https://stackoverflow.com/questions/8468514/getasynckeystate-creating-problems-with-cin //Descartamos todos los inputs en el buffer
		inputType currentInput = inputReader();

		// comprobamos el estado de juego
		switch (currentState)
		{
		case gameState::MENU://----------------------------------------------------------------Menu
			Menus::mainMenu();
			if (currentInput == inputType::ONE)
			{
				board = new Board();
				currentState = gameState::PLAY;
			}
			else if (currentInput == inputType::TWO)
			{
				currentState = gameState::RANKING;
			}
			else if (currentInput == inputType::ZERO)
			{
				inGame = false;
			}
			break;
		case gameState::PLAY://---------------------------------------------------------------InGame
			board->clearElement();// Limpiamos la Pala y la Bola de nuestra Board
			if (currentInput == inputType::EXIT)
			{
				currentState = gameState::MENU;
			}
			else if (currentInput != inputType::MAX) //Leemos los inputs
				board->paddle->paddleMovement(currentInput, board->ball, board->getColumns()); //Movemos la pala según los inputs
			else if (currentInput == inputType::EXIT)// comprobamos si hemos pulsado escape y ponemos el juego a falso, para salir del bucle
				inGame = false;

			if (board->ball->speed != Vec2(0, 0)) //Si la velocidad de la pelota es diferente a zero actualizamos su posición 
			{
				if (board->ball->ballMovement(board->getDimensions(), board->getBlockRows(), board->paddle->position, *board))
				{
					board->minLives(1);
				}
			}

			board->refreshElement(); //Refrescamos la Pala y la Bola
			board->drawBoard(); //Volvemos a pintar el tablero actualizado

			if (board->boardEmpty() || board->getLives() < 1)//Miramos si quedan bloques en la board 
			{
				currentState = gameState::SCORE;// cambiamos el gameState a Score
			}
			break;
		case gameState::SCORE://------------------------------------------------------------Score
			Menus::score(board->getScore(), ranking);
			ranking = Ranking::get();
			currentState = gameState::RANKING; // una vez añadida la puntuacion volvemos al menu
			break;
		case gameState::RANKING://----------------------------------------------------------Ranking
			Ranking::print(ranking);
			Menus::ranking();
			if (currentInput == inputType::ONE)
			{
				board = new Board();
				currentState = gameState::PLAY;
			}
			else if (currentInput == inputType::TWO)
			{
				if (Menus::confirm())
					Ranking::resetScore(ranking);
			}
			else if (currentInput == inputType::ZERO || currentInput == inputType::EXIT)
				currentState = gameState::MENU;
			break;
		}

		if (currentState == gameState::PLAY)
		{
			int xCent = board->blocksXcent();
			Sleep(xCent > 75 ? 100 : xCent > 50 ? 85 : xCent > 25 ? 70 : 60);
		}
		else
			Sleep(100);//Esperamos 100 milisegundos a actualizar
		system("cls"); //Limpiamos la pantalla

	} while (inGame);

	return 0;
}