//#include "Menus.h"
//#include "..\inc\Menus.h"
#include "..\..\dep\inc\Menus.h"

void Menus::score(int score, std::map<std::string, int> ranking)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, RED);
	std::cout << "-*-*-*- GAME OVER -*-*-*-" << std::endl << std::endl;
	SetConsoleTextAttribute(h, GREEN);
	std::cout << "Your score is " << score << '.' << std::endl;

	std::cin.clear();
	std::string name;
	std::cout << "Write your Name:" << std::endl;
	SetConsoleTextAttribute(h, WHITE);
	std::cin >> name;

	while (Ranking::nameExist(name, ranking) || name.length() >= RANKING_MAX_SIZE - std::to_string(score).length() + 1)
	{
		system("CLS");
		SetConsoleTextAttribute(h, RED);
		std::cout << "-*-*-*- GAME OVER -*-*-*-" << std::endl << std::endl;
		SetConsoleTextAttribute(h, GREEN);
		std::cout << "Your score is " << score << '.' << std::endl;

		std::cin.clear();
		if (name.length() >= RANKING_MAX_SIZE - std::to_string(score).length() + 1)
			std::cout << name << " is to long" << std::endl;
		else
			std::cout << name << " already exists; please, enter another name." << std::endl;
		SetConsoleTextAttribute(h, WHITE);
		std::cin >> name;
	};

	ranking[name] = score;
	Ranking::set(ranking);
	ranking = Ranking::get();

	Ranking::print(ranking);

	SetConsoleTextAttribute(h, BLUE);
}

void Menus::mainMenu()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, GREEN);
	std::cout << "-*-*-*- ARKANOID -*-*-*-" << std::endl << std::endl;
	SetConsoleTextAttribute(h, CYAN);
	std::cout << "1- Play" << std::endl;
	std::cout << "2- Ranking" << std::endl;
	SetConsoleTextAttribute(h, RED);
	std::cout << "0- Exit" << std::endl;
}

void Menus::ranking()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::endl;
	SetConsoleTextAttribute(h, CYAN);
	std::cout << "1- Play" << std::endl;
	std::cout << "2- Clear Ranking" << std::endl;
	SetConsoleTextAttribute(h, RED);
	std::cout << "0- Return To Menu" << std::endl;
}

bool Menus::confirm()
{
	
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	char charCtrl;
	SetConsoleTextAttribute(h, GREEN);
	std::cout << std::endl << "Are you sure?   ";
	SetConsoleTextAttribute(h, RED);
	std::cout << "Y/";
	SetConsoleTextAttribute(h, GREEN);
	std::cout << "N" << std::endl;
	SetConsoleTextAttribute(h, WHITE);
	std::cin >> charCtrl;
	if (charCtrl == 'y' || charCtrl == 'Y')
		return true;

	return false;
}
