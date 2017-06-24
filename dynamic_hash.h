#ifndef DYNAMIC_HASH_H_
#define DYNAMIC_HASH_H_

#include <iostream>
#include <fstream>
#include <string>

#define BLOCKSIZE 4096
#pragma pack(1)
using namespace std;

class HashTable {
public:
	HashTable();
	long Table_Block_Offset[BLOCKSIZE / sizeof(long)];		         // 1024
};

class Dynamic_Hash {
private:
	void Extend_Table(unsigned int Hash_Key, fstream& DB_File);
	fstream Hash_File;
	int Table_Bit_Num;

public:
	unsigned int _Hash(string str);
	HashTable Hash_Table;
	Dynamic_Hash(char* Hash_File_name);
	~Dynamic_Hash();
	long Get_Hash_Offset(string s_ID);		                         // ID 값을 가지고 와서 해당 Block Offset을 Return.
	void Block_Full(string s_ID, int Block_Bit_Num, fstream& DB_File);
	void Print_Hash_Table();
	void Bin(int x);
};

#endif