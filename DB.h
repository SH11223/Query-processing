#ifndef DB_H_
#define DB_H_

#include <iostream>
#include <fstream>
#include "students.h"

using namespace std;

class _DB {
private:
	
public:
	fstream DBFile;
	Block DB_Buffer;
	Block_prof DB_Buffer_prof;
	friend class Dynamic_Hash;
	Dynamic_Hash* H;
	void Open();
	void Open_prof();
	void Close();
	bool Insert(Students student);
	bool Insert_prof(Professors professor);
	void Print();
	Students ID_Search(unsigned int ID);
	Professors ID_Search_prof(unsigned int ID);
	int BlockNum(unsigned int ID);
};

#endif