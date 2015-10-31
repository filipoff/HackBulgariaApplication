#include "Table.h"
#include <iostream>


int main()
{
	Table table("table.txt");
	std::cout << table.countWordOccurrences();
	return 0;
}