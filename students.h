#ifndef STUDENTS_H_
#define STUDENTS_H_

#include <iostream>
#include <fstream>
#include <string>

#define BLOCKSIZE  4096                                           
#define IN_BLOCK_MAX  (BLOCKSIZE - sizeof(Students)) / sizeof(Students)
#pragma pack(1)
using namespace std;

class Students {
public:
	char name[20];
	unsigned studentID;
	float score;
	unsigned advisorID;
};

class Block {
public:
	Students Record[IN_BLOCK_MAX];
	Block();
	int  Record_Count;
	int  Bit_Num;
	char Block_Garbage[24];
};

#endif