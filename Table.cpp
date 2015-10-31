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

const char* Table::rstrstr(const char* string, const char* substring) const
{
	size_t string_len = strlen(string);
	size_t substring_len = strlen(substring);
	if (substring_len > string_len)
		return NULL;

	for (const char* pointer = string + string_len - substring_len; pointer >= string; pointer--)
	{
		if (strncmp(pointer, substring, substring_len) == 0)
		{
			return pointer;
		}
	}
	return NULL;
}
size_t Table::countWordOccurrences(const char* word) const
{
	size_t counter = 0;
	for (size_t i = 0; i < rows; i++)
	{
		const char* row = table[i];
		if (strstr(row, word) != NULL)
			counter++;
		if (rstrstr(row, word) != NULL)
			counter++;
	}

	for (size_t i = 0; i < columns; i++)
	{
		char* column = new char[rows + 1];
		for (int j = 0; j < rows; j++)
		{
			column[j] = table[j][i];
		}
		column[rows] = '\n';
		if (strstr(column, word) != NULL)
			counter++;
		if (rstrstr(column, word) != NULL)
			counter++;
		delete[] column;
	}

	// TODO: implement diagonals
	return counter;
}