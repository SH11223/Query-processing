#ifndef STUDENTS_H_
#define STUDENTS_H_

#include <iostream>
#include <fstream>
#include <string>

#define BLOCKSIZE  4096                                           
#define IN_BLOCK_MAX  (BLOCKSIZE - sizeof(Students)) / sizeof(Students)
#define IN_BLOCK_MAX_PRO  BLOCKSIZE / sizeof(Professors)
#pragma pack(1)
using namespace std;

class Students {
public:
	Students();
	char name[20];
	unsigned studentID;
	float score;
	unsigned advisorID;
};

class Professors{
public:
	Professors();
	char name[20];
	unsigned profID;
	int Salary;
};

class Block {
public:
	Students Record[IN_BLOCK_MAX];
	Block();
	int  Record_Count;
	int  Bit_Num;
	char Block_Garbage[24];
};

class Block_prof {
public:
	Professors Record[IN_BLOCK_MAX_PRO];
	Block_prof();
	int  Record_Count;
	int  Bit_Num;
};

#endif