#include <utility>
#include <string>
#include <iostream>
#include "Table.h"

std::pair<int, int> points(int currentX, int currentY, std::string input)
{
	char moveLeft = '<';
	char moveRight = '>';
	char moveUp = '^';
	char moveDown = 'v';
	char warpSymbol = '~';

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == moveRight)
			currentX++;
		else if (input[i] == moveLeft)
			currentX--;
		else if (input[i] == moveUp)
			currentY--;
		else if (input[i] == moveDown)
			currentY++;
		else if (input[i] == warpSymbol)
		{
			std::swap(moveLeft, moveRight);
			std::swap(moveUp, moveDown);
		}
	}
	return std::make_pair(currentX, currentY);
}



int main()
{
//	std::string test(">>><<<~>>>~^^^");
//	std::pair<int, int> result = points(0, 0, test);
//	std::cout << result.first << ' ' << result.second << std::endl;
	Table T("table.txt");
	std::cout << T.countWordOccurrences("ivan");

	return 0;
}