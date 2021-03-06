#pragma once
#include <fstream>
#include <vector>
#include <algorithm>

class Table
{
private:
	size_t rows;
	size_t columns;
	char** table;

private:
	char** allocate(const size_t height, const size_t width);
	void free();
	void copyFrom(const Table& other);
	void setTableFromFile(std::istream &in);
	bool getTableSizeFromFile(std::istream &in, size_t& newHeight, size_t& newWidth) const;
	char* reversedCopy(const char* str) const;
public:
	Table();
	Table(const char* filename);
	~Table();
	Table(const Table& other);
	Table& operator=(const Table& other);
	bool loadFromFileName(const char* filename);
	size_t countWordOccurrences(const char* word) const;
};