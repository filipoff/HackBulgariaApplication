#include "Table.h"

char** Table::allocate(const size_t rows, const size_t columns)
{
	char** temp = NULL;
	this->rows = rows;
	this->columns = columns;

	try
	{
		temp = new char*[rows];

		for (size_t i = 0; i < rows; i++)
		{
			temp[i] = new char[columns + 1];
		}
	}

	catch (std::bad_alloc)
	{
		free();
	}

	return temp;
}
void Table::free()
{
	for (size_t i = 0; i < rows; i++)
	{
		delete[] table[i];
	}
	delete table;
	table = NULL;
	rows = 0;
	columns = 0;
}
void Table::copyFrom(const Table& other)
{
	rows = other.rows;
	columns = other.columns;
	table = allocate(rows, columns);

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j <= columns; j++)
		{
			table[i][j] = other.table[i][j];
		}
}

Table::Table() : table(NULL), rows(0), columns(0) {}
Table::~Table()
{
	free();
}
Table::Table(const Table& other)
{
	free();
	copyFrom(other);
}

Table::Table(const char* filename) : table(NULL), rows(0), columns(0)
{
	loadFromFileName(filename);
}

Table& Table::operator=(const Table& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}
void Table::setTableFromFile(std::istream &in)
{
	in.clear();
	in.seekg(0, std::ios::beg);

	for (size_t i = 0; i < rows; i++)
	{
		in.getline(table[i], columns + 1);
	}
}

bool Table::getTableSizeFromFile(std::istream &in, size_t& newRows, size_t& newColumns) const
{
	in.clear();
	in.seekg(0, std::ios::beg);

	int rows = 0;
	int columns = 0;
	int charCounter = 0;
	char c = 0;

	while (in.get(c))
	{
		if (c == '\n')
		{
			break;
		}
		columns++;
	}

	if (columns == 0)
		return false;

	rows++;

	while (in.get(c))
	{
		if (c == '\n')
		{
			if (columns != charCounter)
				return false;
			rows++;
			charCounter = 0;

			if (in.peek() == '\n')
			{
				rows++;
				break;
			}
		}
		else
			charCounter++;
	}

	newRows = rows;
	newColumns = columns;
	return true;
}


bool Table::loadFromFileName(const char* filename)
{
	std::ifstream inFile(filename);

	if (!inFile)
		return false;
	size_t heightFromFile = 0;
	size_t widthFromFile = 0;
	if (!getTableSizeFromFile(inFile, heightFromFile, widthFromFile))
		return false;
	if (table)
		free();
	table = allocate(heightFromFile, widthFromFile);
	if (!table)
		return false;
	setTableFromFile(inFile);
	return true;
}

char* Table::reversedCopy(const char* str) const
{
	size_t str_len = strlen(str);
	char* reversed = new char[str_len + 1];
	const char* end = str + str_len - 1;
	for (size_t i = 0; end >= str; end--, i++)
	{
		reversed[i] = *end;
	}
	reversed[str_len] = '\0';
	return reversed;
}
size_t Table::countWordOccurrences(const char* word) const
{
	size_t wordOccurrences = 0;
	char* reversedWord = reversedCopy(word);

	// check the rows
	for (size_t i = 0; i < rows; i++)
	{
		const char* row = table[i];
		if (strstr(row, word) != NULL)
			wordOccurrences++;
		if (strstr(row, reversedWord) != NULL)
			wordOccurrences++;
	}


	// check the columns
	for (size_t i = 0; i < columns; i++)
	{
		char* column = new char[rows + 1];
		for (size_t j = 0; j < rows; j++)
		{
			column[j] = table[j][i];
		}
		column[rows] = '\0';
		if (strstr(column, word) != NULL)
			wordOccurrences++;
		if (strstr(column, reversedWord) != NULL)
			wordOccurrences++;
		delete[] column;
	}

	// left bottom to top right diagonal
	for (int i = strlen(word) - rows; i < (int)columns; i++)
	{
		char* diagonalString = new char[std::min(rows, columns) + 1];
		size_t diagonalStringIndex = 0;

		for (int j = 0; j < (int)rows; j++)
		{
			if ((i + j) >= 0 && (i + j) < (int)columns)
			{
				diagonalString[diagonalStringIndex] = table[j][i + j];
				diagonalStringIndex++;
			}
		}
		diagonalString[diagonalStringIndex] = '\0';
		if (diagonalStringIndex < strlen(word))
		{
			delete[] diagonalString;
			break;
		}
		if (strstr(diagonalString, word) != NULL)
			wordOccurrences++;
		if (strstr(diagonalString, reversedWord) != NULL)
			wordOccurrences++;
		delete[] diagonalString;
	}

	// must be fixed
	for (size_t i = columns + rows - 2; i >= 0; i--)
	{
		char* diagonalString = new char[std::min(rows, columns) + 1];
		size_t diagonalStringIndex = 0;

		for (size_t j = 0; j < rows; j++, i--)
		{
			if (true)
			{
				diagonalString[diagonalStringIndex] = table[j][j - i];
				diagonalStringIndex++;
			}
		}
		diagonalString[diagonalStringIndex] = '\0';
		//	if (diagonalStringIndex < strlen(word))
		//	{
		//			delete[] diagonalString;
		//		break;
		//		}
		if (strstr(diagonalString, word) != NULL)
			wordOccurrences++;
		if (strstr(diagonalString, reversedWord) != NULL)
			wordOccurrences++;
		delete[] diagonalString;
	}

	delete[] reversedWord;
	return wordOccurrences;
}