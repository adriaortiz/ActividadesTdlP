//#include "Ranking.h"
#include "..\..\dep\inc\Ranking.h"

std::map<std::string, int> Ranking::get()
{
	std::map<std::string, int> ranking;
	std::ifstream rFile;
	rFile.open(pathRanking);

	if (rFile.is_open())
	{
		ranking.clear();
		// recorremos el contenido del txt separandolo por puntoComa
		for (std::string line; std::getline(rFile, line, ';'); )
		{
			std::string Key = line.substr(0, line.find(','));
			int Value = std::stoi(line.substr(Key.length() + 1, line.find(';')));
			ranking[Key] = Value;
		}
	}

	return ranking;
}

void Ranking::set(std::map<std::string, int>& rank)
{
	std::ofstream rFile;
	rFile.open(pathRanking);

	if (rFile.is_open())
	{
		std::map<std::string, int>::iterator it = rank.begin();
		std::string newKey = it->first;
		it++;

		for (int i = 0; i < 5; i++)
		{
			do
			{
				if (rank[newKey] < it->second)
				{
					newKey = it->first;
				}
				it++;
			} while (it != rank.end());

			rFile << newKey << ',' << rank[newKey] << ';';
			rank.erase(newKey);
			it = rank.begin();
			newKey = it->first;
			it++;
		}
	}

	rank.clear();
	rank = Ranking::get();
}

bool Ranking::nameExist(std::string & name, std::map<std::string, int>& rank)
{
	if (rank[name]) return true;
	return false;
}

void Ranking::print(const std::map<std::string, int>& currentRank)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, PINK);
	std::cout << "-*-*-*- Ranking -*-*-*-" << std::endl;
	std::map<std::string, int> rank = currentRank;
	std::map<std::string, int>::iterator it = rank.begin();
	std::string newKey = it->first;
	it++;
	SetConsoleTextAttribute(h, YELLOW);

	for (int i = 0; i < 4; i++)
	{
		while (it != rank.end())
		{
			if (rank[newKey] < it->second)
			{
				newKey = it->first;
			}
			it++;
		}

		std::cout << i + 1 << ". " << (rank[newKey] == 0 ? "NaN" : newKey);
		int spaceNumber = RANKING_MAX_SIZE - ((rank[newKey] == 0) ? 3 : newKey.length());
		for (int i = 0; i < spaceNumber; i++)
			std::cout << " ";
		std::cout << rank[newKey] << std::endl;
		rank.erase(newKey);
		it = rank.begin();
		newKey = it->first;
		it++;
	}

	std::cout << 5 << ". " << (rank[newKey] == 0 ? "NaN" : newKey);
	int spaceNumber = RANKING_MAX_SIZE - ((rank[newKey] == 0) ? 3 : newKey.length());
	for (int i = 0; i < spaceNumber; i++)
		std::cout << " ";
	std::cout << rank[newKey] << std::endl;
}

void Ranking::resetScore(std::map<std::string, int>& currentRank)
{
	currentRank.clear();
	currentRank["e0"] = 0;
	currentRank["e1"] = 0;
	currentRank["e2"] = 0;
	currentRank["e3"] = 0;
	currentRank["e4"] = 0;

	std::map<std::string, int>::iterator it = currentRank.begin();
	std::ofstream rFile;
	rFile.open(pathRanking);
	if (rFile.is_open())
	{
		do
		{
			rFile << it->first << ',' << it->second << ';';
			it++;
		} while (it != currentRank.end());
	}
	
}
