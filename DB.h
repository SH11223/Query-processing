#ifndef DB_H_
#define DB_H_

#include <iostream>
#include <fstream>
#include "students.h"

using namespace std;

class _DB {
private:
	fstream DBFile;
	Block DB_Buffer;
public:
	friend class Dynamic_Hash;
	Dynamic_Hash* H;
	void Open();
	void Close();
	bool Insert(Students student);
	void Print();
	// void ID_Search(unsigned int ID);
	int BlockNum(unsigned int ID);
};

#endif