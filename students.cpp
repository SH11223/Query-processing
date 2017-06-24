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

Professors to_prof(ifstream& fin){
	int i = 0, pos = 0;
	string tmp;
	Professors prof;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strcpy(prof.name, tmp.substr(i, pos - i).c_str());
	i = pos + 1;
	pos = tmp.find(",", i);
	prof.profID = stoi(tmp.substr(i, pos - i));
	i = pos + 1;
	prof.Salary = stoi(tmp.substr(i));

	return prof;
}

void Exact_Prof(char*, char*){
}

void Exact_Stud(char*, char*){
}

void Range_Prof(char*, char*, char*){
}

void Range_Stud(char*, char*, char*){
}
int main(int argc, char** argv) {

	int N1, N2;
	int k =0 ;
	string tmp;
	Professors* professors;
	Students* students;
	_DB testDB;

	if(strcmp(argv[0],"Search-Exact")==0){
		if(argc!= 4)
			cout << "error" << endl;
		else if(strcmp(argv[1],"Professors")==0)
			Exact_Prof(argv[2], argv[3]);
		else if(strcmp(argv[1],"Students")==0)
			Exact_Stud(argv[2], argv[3]);
		else
			cout<<"error" << endl;
	}
	else if(strcmp(argv[0],"Search-Range")==0){
		if(argc!= 5)
			cout << "error" << endl;
		else if(strcmp(argv[1],"Professors")==0)
			Range_Prof(argv[2], argv[3],argv[4]);
		else if(strcmp(argv[1],"Students")==0)
			Range_Stud(argv[2], argv[3],argv[4]);
		else
			cout<<"error" << endl;
	}
	else if(strcmp(argv[0],"Join")==0){
		if(argc!= 3)
			cout << "error" << endl;
	}
	else
		cout << "error" << endl;
	
	BPNode* root = new BPNode();

	ifstream fin1, fin2;
	fin1.open("student_data.csv");
	fin2.open("prof_data.csv");

	fstream sco;
	ofstream fout1, fout2;
	fout1.open("Students_score.idx");
	fout2.open("Professor_Salary.idx");
	getline(fin1, tmp);
	N1 = stoi(tmp);

	testDB.Open();
	students = new Students[N1];
	for (int i = 0; i < N1; i++) {
		students[i] = to_student(fin1);
		testDB.Insert(students[i]);
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
	}
	cout << sizeof(BPNode);
	testDB.Print();

	professors = new Professors[N2];
	for (int i = 0; i < N2; i++) {
		professors[i] = to_prof(fin2);
	}
	cout << "Enter the leaf number" << endl;
	cin >> k;
/*
	for (int i = 0; i < N; i++) {
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
		
	}*/
	root->Print(fout2);
	root->Print(k);
	fin1.close();
	fin2.close();
	fout1.close();
	fout2.close();
	testDB.Close();
	delete [] students;
	delete [] professors;
	return 0;
}