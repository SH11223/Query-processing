#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "bptree.h"
#include "students.h"
#include "dynamic_hash.h"
#include "DB.h"

#pragma pack(1)

using namespace std;

Block::Block()
{
	Record_Count = 0;
	Bit_Num = 0;
}

Students to_student(ifstream& fin) {
	int i = 0, pos = 0;
	string tmp;
	Students student;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strcpy(student.name, tmp.substr(i, pos - i).c_str());
	i = pos + 1;
	pos = tmp.find(",", i);
	student.studentID = stoi(tmp.substr(i, pos - i));
	i = pos + 1;
	pos = tmp.find(",", i);
	student.score = stof(tmp.substr(i, pos - i));
	i = pos + 1;
	student.advisorID = stoi(tmp.substr(i));

	return student;
}

int main() {

	int N;
	int k =0 ;
	string tmp;
	Students* students;
	_DB testDB;

	BPNode* root = new BPNode();

	ifstream fin;
	fin.open("sampleData.csv");

	fstream sco;

	getline(fin, tmp);
	N = stoi(tmp);

	testDB.Open();
	students = new Students[N];
	for (int i = 0; i < N; i++) {
		students[i] = to_student(fin);
		testDB.Insert(students[i]);
	}

	testDB.Print();

	cout << "Enter the leaf number" << endl;
	cin >> k;

	for (int i = 0; i < N; i++) {
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
		
	}
	root->Print(sco);
	root->Print(k+1);
	fin.close();
	testDB.Close();
	delete [] students;
	return 0;
}